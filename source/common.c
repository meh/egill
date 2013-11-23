#include <egill/common.h>

WL_EXPORT void*
el_malloc (size_t size)
{
	assert(size != 0);

	void* result = malloc(size);

	if (result == NULL) {
		abort();
	}

	return result;
}

WL_EXPORT void
el_free (void* pointer)
{
	assert(pointer != NULL);

	free(pointer);
}

#ifndef HAVE_MKOSTEMP
	static int
	set_cloexec_or_close (int fd)
	{
		long flags;

		if (fd < 0) {
			return fd;
		}

		flags = fcntl(fd, F_GETFD);

		if (flags < 0) {
			goto fail;
		}

		if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) < 0) {
			goto fail;
		}

		return fd;

	fail:
		close(fd);

		return -1;
	}
#endif

static int
tempfile_create (char* name)
{
	int fd;

	#ifdef HAVE_MKOSTEMP
		fd = mkostemp(name, O_CLOEXEC);

		if (fd >= 0) {
			unlink(name);
		}
	#else
		fd = mkstemp(name);

		if (fd >= 0) {
			fd = set_cloexec_or_close(fd);
			unlink(name);
		}
	#endif
	
	return fd;
}

WL_EXPORT int
el_create_anonymous_file (off_t size)
{
	static const char  template[] = "/egill-shared-XXXXXX";
	       const char* path;
	             char* name;

	path = getenv("XDG_RUNTIME_DIR");

	if (path == NULL) {
		errno = ENOENT;

		return -1;
	}

	name = el_malloc(strlen(path) + sizeof(template));
	strcpy(name, path);
	strcat(name, template);

	int fd = tempfile_create(name);

	if (fd < 0) {
		return fd;
	}

	if (ftruncate(fd, size) < 0) {
		close(fd);

		return -1;
	}

	return fd;
}

WL_EXPORT void
el_empty_region (pixman_region32_t* region)
{
	pixman_region32_fini(region);
	pixman_region32_init(region);
}

WL_EXPORT void
el_move_resources (wl_list_t destination, wl_list_t source)
{
	wl_list_insert_list(destination, source);
	wl_list_init(source);
}

WL_EXPORT void
el_move_resources_for_client (wl_list_t destination, wl_list_t source,
                              wl_client_t client)
{
	wl_resource_t resource;
	wl_resource_t tmp;

	wl_resource_for_each_safe(resource, tmp, source) {
		if (wl_resource_get_client(resource) == client) {
			wl_list_remove(wl_resource_get_link(resource));
			wl_list_insert(destination, wl_resource_get_link(resource));
		}
	}
}

WL_EXPORT void
el_resource_unbind (wl_resource_t resource)
{
	wl_list_remove(wl_resource_get_link(resource));
}

WL_EXPORT void
el_resource_release (wl_client_t client, wl_resource_t resource)
{
	(void) client;

	wl_resource_destroy(resource);
}
