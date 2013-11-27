#include <stdio.h>
#include <stdlib.h>
#include <egill.h>

int
main (int argc, char* argv[])
{
	(void) argc;
	(void) argv;

	el_compositor_t compositor = el_compositor_create(NULL);

	if (compositor == NULL) {
		fprintf(stderr, "fatal: could not create the compositor\n");
		
		return EXIT_FAILURE;
	}

	if (!el_compositor_define_renderer(compositor, "pixman", NULL)) {
		fprintf(stderr, "fatal: could not create the renderer\n");

		return EXIT_FAILURE;
	}

	{
		bool defined = false;

		if (getenv("WAYLAND_DISPLAY")) {
			defined = el_compositor_define_backend(compositor, "wayland", NULL);
		}
		else if (getenv("DISPLAY")) {
			defined = el_compositor_define_backend(compositor, "x11", NULL);
		}
		else {
			defined = el_compositor_define_backend(compositor, "drm", NULL);
		}

		if (!defined) {
			fprintf(stderr, "fatal: could not create the backend\n");

			return EXIT_FAILURE;
		}
	}

	if (!el_compositor_run(compositor)) {
		fprintf(stderr, "fatal: failed to start the compositor\n");

		return EXIT_FAILURE;
	}

	el_compositor_destroy(compositor);

	return EXIT_SUCCESS;
}

