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

#ifndef EGILL_BACKEND_H
#define EGILL_BACKEND_H

#include <egill/compositor.h>

EL_OBJECT(backend) {
	const char*     name;
	el_compositor_t compositor;
};

/*!
 * Create the backend with the given argument list.
 *
 * \param compositor the compositor for the backend
 * \param name the name of the backend
 * \param args the list of arguments, the last argument must be NULL
 *
 * \memberof el_backend_t
 */
el_backend_t el_backend_create (el_compositor_t compositor, const char* name, va_list args);

/*!
 * Destroy the backend.
 *
 * \param self the backend to destroy
 *
 * \memberof el_backend_t
 */
void el_backend_destroy (el_backend_t self);

#endif
