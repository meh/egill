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

#include <egill/common.h>
#include <egill/renderer.h>

#ifdef USE_RENDERER_PIXMAN
#include <egill/renderer/pixman.h>
#endif

static struct {
	const char* name;

	el_renderer_t (*constructor)(el_compositor_t, va_list);
	void          (*destructor)(el_renderer_t);
} renderers[] = {
	#ifdef USE_RENDERER_PIXMAN
	{ "pixman", el_renderer_pixman_create, el_renderer_pixman_destroy },
	#endif
};

el_renderer_t
el_renderer_create (el_compositor_t compositor, const char* name, va_list args)
{
	el_renderer_t renderer = NULL;

	if (lengthof(renderers) == 0) {
		return NULL;
	}

	if (name == NULL) {
		renderer = renderers[0].constructor(compositor, args);
	}
	else {
		for (size_t i = 0; i < lengthof(renderers); i++) {
			if (strcmp(renderers[i].name, name) == 0) {
				renderer = renderers[i].constructor(compositor, args);
				break;
			}
		}
	}

	return renderer;
}

void
el_renderer_destroy (el_renderer_t self)
{
	for (size_t i = 0; i < lengthof(renderers); i++) {
		if (strcmp(self->name, renderers[i].name) == 0) {
			renderers[i].destructor(self);
			break;
		}
	}
}
