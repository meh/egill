#include <egill/compositor.h>
#include <egill/backend.h>
#include <egill/renderer.h>

WL_EXPORT el_compositor_t
el_compositor_create (const char* socket)
{
	el_compositor_t self    = el_create(el_compositor);
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

	el_destroy(self);
}
