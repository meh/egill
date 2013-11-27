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

#ifndef EGILL_TRANSFORM_H
#define EGILL_TRANSFORM_H

#include <egill/common.h>

EL_ENUM(transform_type) {
	EL_MATRIX_TRANSFORM_TRANSLATE = 1 << 0,
	EL_MATRIX_TRANSFORM_SCALE     = 1 << 1,
	EL_MATRIX_TRANSFORM_ROTATE    = 1 << 2,
	EL_MATRIX_TRANSFORM_OTHER     = 1 << 3
};

EL_CLASS(matrix) {
	float    d[16];
	uint32_t type;
};

EL_CLASS(vector) {
	float f[4];
};

EL_CLASS(transform) {
	el_matrix matrix;
	wl_list   link;
};

el_matrix_t el_matrix_create (float data[16]);

void el_matrix_initialize (el_matrix_t matrix);

void el_matrix_destroy (el_matrix_t self);

void el_matrix_multiply (el_matrix_t self, const el_matrix_t other);

void el_matrix_scale (el_matrix_t self, float x, float y, float z);

void el_matrix_translate (el_matrix_t self, float x, float y, float z);

void el_matrix_rotate (el_matrix_t self, float cos, float sin);

void el_matrix_transform (el_matrix_t self, el_vector_t vector);

bool el_matrix_invert (el_matrix_t self, const el_matrix_t other);

void el_transform_region (wl_output_transform_e transform,
                          int width, int height, int32_t scale,
                          pixman_region32_t* src, pixman_region32_t* dest);

#endif

