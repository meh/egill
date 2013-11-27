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

#include <egill/compositor.h>
#include <egill/backend.h>
#include <egill/renderer.h>

WL_EXPORT el_compositor_t
el_compositor_create (const char* socket)
{
	el_compositor_t self    = el_create(compositor);
	wl_display_t    display = wl_display_create();

	if (socket == NULL) {
		socket = "wayland-0";
	}

	self->socket = socket;

	if (wl_display_add_socket(display, socket)) {
		goto fail;
	}

	self->display = display;
	wl_display_init_shm(self->display);

	wl_list_init(&self->processes);

	return self;

fail:
	el_compositor_destroy(self);

	return NULL;
}

WL_EXPORT void
el_compositor_destroy (el_compositor_t self)
{
	if (self->backend) {
		el_backend_destroy(self->backend);
	}

	if (self->renderer) {
		el_renderer_destroy(self->renderer);
	}

	if (self->display) {
		wl_display_destroy(self->display);
	}

	if (self->x) {
		el_x_stop(self->x);
	}

	el_destroy(self);
}

WL_EXPORT bool
el_compositor_define_renderer (el_compositor_t self, const char* name, ...)
{
	va_list       args;
	el_renderer_t renderer;

	va_start(args, name);
	renderer = el_renderer_create(self, name, args);
	va_end(args);

	if (renderer == NULL) {
		return false;
	}

	self->renderer = renderer;

	return true;
}

WL_EXPORT bool
el_compositor_define_backend (el_compositor_t self, const char* name, ...)
{
	va_list       args;
	el_backend_t backend;

	va_start(args, name);
	backend = el_backend_create(self, name, args);
	va_end(args);

	if (backend == NULL) {
		return false;
	}

	self->backend = backend;

	return true;
}

WL_EXPORT void
el_compositor_start_x (el_compositor_t self)
{
	self->x = el_x_start(self);
}

WL_EXPORT bool
el_compositor_run (el_compositor_t self)
{
	if (!self->renderer) {
		if (!el_compositor_define_renderer(self, NULL, NULL)) {
			return false;
		}
	}

	if (!self->backend) {
		if (!el_compositor_define_backend(self, NULL, NULL)) {
			return false;
		}
	}

	setenv("WAYLAND_DISPLAY", self->socket, 1);

	wl_display_run(self->display);

	return true;
}

WL_EXPORT void
el_compositor_watch_process (el_compositor_t self, el_process_t process)
{
	wl_list_insert(&self->processes, &process->link);
}
