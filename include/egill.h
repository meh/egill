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

#ifndef EGILL_H
#define EGILL_H

#include <stdbool.h>

struct el_backend;
struct el_renderer;
struct el_compositor;

typedef struct el_backend*    el_backend_t;
typedef struct el_renderer*   el_renderer_t;
typedef struct el_compositor* el_compositor_t;

el_compositor_t el_compositor_create (const char* socket);
void el_compositor_destroy (el_compositor_t self);
bool el_compositor_define_renderer (el_compositor_t self, const char* name, ...);
bool el_compositor_define_backend (el_compositor_t self, const char* name, ...);
bool el_compositor_run (el_compositor_t self);

#endif
