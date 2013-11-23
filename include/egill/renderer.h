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

#ifndef EGILL_RENDERER_H
#define EGILL_RENDERER_H

#include <egill/compositor.h>

EL_OBJECT(renderer) {
	const char*     name;
	el_compositor_t compositor;
};

/*!
 * Create the renderer with the given argument list.
 *
 * \param compositor the compositor for the renderer
 * \param name the name of the renderer
 * \param args the list of arguments, the last argument must be NULL
 *
 * \memberof el_renderer_t
 */
el_renderer_t el_renderer_create (el_compositor_t compositor, const char* name, va_list args);

/*!
 * Destroy the renderer.
 *
 * \param self the renderer to destroy
 *
 * \memberof el_renderer_t
 */
void el_renderer_destroy (el_renderer_t self);

#endif
