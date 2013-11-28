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

#ifndef EGILL_BUFFER_H
#define EGILL_BUFFER_H

#include <egill/common.h>

EL_CLASS(buffer) {
	wl_resource_t resource;

	struct {
		wl_signal   signal;
		wl_listener listener;
	} destroy;

	wl_shm_buffer_t shm;

	int32_t width;
	int32_t height;

	bool inverted;

	uint32_t busy;
};

EL_CLASS(buffer_reference) {
	el_buffer_t buffer;

	struct {
		wl_listener listener;
	} destroy;
};

el_buffer_t el_buffer_from_resource (wl_resource_t resource);

void el_buffer_destroy (el_buffer_t self);

void el_buffer_point (el_buffer_reference_t self, el_buffer_t buffer);

#endif
