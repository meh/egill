/**
 * 2013 (ɔ) meh. [http://meh.schizfreni.co | meh@schizofreni.co]
 *
 * This file is part of egill
 *
 * Parsepples is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Parsepples is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with egill. If not, see <http://www.gnu.org/licenses/>.
 */

#include <egill/x.h>
#include <egill/compositor.h>

#include "xserver.h"

#include <stdio.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/un.h>

#ifndef XWAYLAND_PATH
#define XWAYLAND_PATH "/usr/bin/X"
#endif

static void
set_window_id (wl_client_t client, wl_resource_t resource,
               wl_resource_t surface_resource, uint32_t id)
{
}

WL_IMPLEMENTATION(xserver) {
	set_window_id
};

static const char*
get_path (void)
{
	const char* path = getenv("XWAYLAND_PATH");

	if (path == NULL || *path == '\0') {
		return XWAYLAND_PATH;
	}
	else {
		return path;
	}
}

static char*
create_lock (int display, int *out)
{
	char* path = NULL;
	int   fd   = -1;

	do {
		asprintf(&path, "/tmp/.X%d-lock", display);

		int fd = open(path, O_WRONLY | O_CLOEXEC | O_CREAT | O_EXCL, 0444);

		if (fd < 0 && errno == EEXIST) {
			fd = open(path, O_CLOEXEC, O_RDONLY);

			char pid[11];

			if (fd < 0 || read(fd, pid, 11) != 11) {
				goto again;
			}

			char* end   = NULL;
			pid_t other = strtol(pid, &end, 0);

			if (end != pid + 10) {
				goto again;
			}

			if (kill(other, 0) < 0 && errno == ESRCH) {
				goto stale;
			}

		again:
			free(path);
			display++;
			continue;

		stale:
			unlink(path);
			free(path);
			continue;
		}
		else if (fd < 0) {
			free(path);

			return NULL;
		}

		break;
	} while (true);

	char pid[11];
	int  size = snprintf(pid, 11, "%10d\n", getpid());

	if (size != 11 || write(fd, pid, 11) != 11) {
		unlink(path);
		free(path);

		return NULL;
	}

	close(fd);
	*out = display;

	return path;
}

static int
bind_to_abstract_socket (int display)
{
	int fd = socket(PF_LOCAL, SOCK_STREAM | SOCK_CLOEXEC, 0);

	if (fd < 0) {
		return -1;
	}

	struct sockaddr_un addr = {
		.sun_family = AF_LOCAL
	};

	socklen_t name = snprintf(addr.sun_path, sizeof(addr.sun_path),
		"%c/tmp/.X11-unix/X%d", 0, display);

	socklen_t size = offsetof(struct sockaddr_un, sun_path) + name;

	if (bind(fd, (struct sockaddr*) &addr, size) < 0) {
		goto fail;
	}

	if (listen(fd, 1) < 0) {
		goto fail;
	}

	return fd;

fail:
	close(fd);

	return -1;
}

static int
bind_to_unix_socket (int display)
{
	int fd = socket(PF_LOCAL, SOCK_STREAM | SOCK_CLOEXEC, 0);

	if (fd < 0) {
		return -1;
	}

	struct sockaddr_un addr = {
		.sun_family = AF_LOCAL
	};

	socklen_t name = snprintf(addr.sun_path, sizeof(addr.sun_path),
		"/tmp/.X11-unix/X%d", display) + 1;

	socklen_t size = offsetof(struct sockaddr_un, sun_path) + name;
	unlink(addr.sun_path);

	if (bind(fd, (struct sockaddr*) &addr, size) < 0) {
		goto fail;
	}

	if (listen(fd, 1) < 0) {
		goto fail;
	}

	return fd;

fail:
	close(fd);

	return -1;
}

static void
bind_server (wl_client_t client, void* data, uint32_t version, uint32_t id)
{
	(void) version;

	el_x_t x = data;

	if (client != x->client) {
		return;
	}

	x->resource = wl_resource_create(client, &xserver_interface, 1, id);
	wl_resource_set_implementation(x->resource, &xserver_implementation, x, NULL);

	x->window_manager = el_x_window_manager_create(x);

	xserver_send_listen_socket(x->resource, x->abstract.fd);
	xserver_send_listen_socket(x->resource, x->unix.fd);
}

static void
cleanup_process (el_process_t process, int status)
{
	(void) status;

	el_x_stop(process->private);
}

EL_IMPLEMENTATION(process_cleanup) {
	.cleanup = cleanup_process
};

WL_EXPORT el_x_t
el_x_start (el_compositor_t compositor)
{
	el_x_t self = el_create(x);

	self->compositor  = compositor;
	self->abstract.fd = -1;
	self->unix.fd     = -1;

	do {
		int   display = 0;
		char* lock    = NULL;

		lock = create_lock(display, &display);

		if (lock == NULL) {
			goto fail;
		}

		self->abstract.fd = bind_to_abstract_socket(display);

		if (self->abstract.fd < 0) {
			unlink(lock);

			if (errno == EADDRINUSE) {
				display++;
				continue;
			}
			else {
				goto fail;
			}
		}

		self->unix.fd = bind_to_unix_socket(display);

		if (self->unix.fd < 0) {
			unlink(lock);
			close(self->abstract.fd);

			return NULL;
		}

		self->display = display;
		self->lock    = lock;

		break;
	} while (true);

	int pair[2];
	if (socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0, pair) < 0) {
		goto fail;
	}

	char* display;
	asprintf(&display, ":%d", self->display);
	setenv("DISPLAY", display, 1);

	pid_t pid;
	switch ((pid = fork())) {
		case -1:
			goto fail;

		case 0: {
			int flags = fcntl(pair[1], F_GETFD);

			if (flags != -1) {
				fcntl(pair[1], F_SETFD, flags & ~FD_CLOEXEC);
			}

			char* fd;
			asprintf(&fd, "%d", pair[1]);
			setenv("WAYLAND_SOCKET", fd, 1);
			free(fd);

			execl(get_path(), get_path(), "-wayland", "-rootless",
			      "-retro", "-nolisten", "all", "-terminate", NULL);

			exit(EXIT_FAILURE);
			goto fail;
		}

		default:
			close(pair[1]);

			self->client = wl_client_create(self->compositor->display, pair[0]);

			self->process.private = self;
			self->process.pid     = pid;
			self->process.method  = &process_cleanup;
	}

	wl_global_create(compositor->display, &xserver_interface, 1, self,
		bind_server);

	return self;

fail:
	el_x_stop(self);

	return NULL;
}

WL_EXPORT void
el_x_stop (el_x_t self)
{
	if (self->lock) {
		unlink(self->lock);
		free(self->lock);
	}

	if (self->abstract.fd >= 0) {
		close(self->abstract.fd);
	}

	if (self->unix.fd >= 0) {
		close(self->unix.fd);
	}

	el_destroy(self);
}
