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

#include <egill/backend.h>

#ifdef USE_BACKEND_X11
#include <egill/backend/x11.h>
#endif

static struct {
	const char* name;

	el_backend_t (*constructor)(el_compositor_t, va_list);
	void         (*destructor)(el_backend_t);
} backends[] = {
	#ifdef USE_BACKEND_X11
	{ "x11", el_backend_x11_create, el_backend_x11_destroy },
	#endif
};

el_backend_t
el_backend_create (el_compositor_t compositor, const char* name, va_list args)
{
	el_backend_t backend = NULL;

	if (lengthof(backends) == 0) {
		return NULL;
	}

	if (name == NULL) {
		backend = backends[0].constructor(compositor, args);
	}
	else {
		for (size_t i = 0; i < lengthof(backends); i++) {
			if (strcmp(backends[i].name, name) == 0) {
				backend = backends[i].constructor(compositor, args);
				break;
			}
		}
	}

	return backend;
}

void
el_backend_destroy (el_backend_t self)
{
	for (size_t i = 0; i < lengthof(backends); i++) {
		if (strcmp(self->name, backends[i].name) == 0) {
			backends[i].destructor(self);
			break;
		}
	}
}
