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

#ifndef EGILL_RENDERER_PIXMAN
#define EGILL_RENDERER_PIXMAN

#include <egill/renderer.h>

EL_CLASS(renderer_pixman) {
	EL_INHERIT(renderer);
};

el_renderer_t el_renderer_pixman_create (el_compositor_t compositor,
                                         va_list args);

void el_renderer_pixman_destroy (el_renderer_t self);

#endif
