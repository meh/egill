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

#ifndef EGILL_OUTPUT_H
#define EGILL_OUTPUT_H

#include <egill/common.h>

EL_CLASS(output_mode) {
	uint32_t flags;

	int32_t width;
	int32_t height;

	uint32_t refresh;

	struct wl_list link;
};

EL_ENUM(dpms) {
	EL_DPMS_ON,
	EL_DPMS_STANDBY,
	EL_DPMS_SUSPEND,
	EL_DPMS_OFF
};

EL_CLASS(output) {
	uint32_t    id;
	const char* name;

	struct el_compositor* compositor;
	void*                 state;

	int32_t x;
	int32_t y;

	int32_t width;
	int32_t height;
};

void el_output_create (el_output_t self);

void el_output_destroy (el_output_t self);

#endif
