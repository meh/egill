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

#ifndef EGILL_BACKEND_X11
#define EGILL_BACKEND_X11

#include <xcb/xcb.h>
#include <xcb/shm.h>

#include <egill/backend.h>
#include <egill/output.h>

EL_CLASS(backend_x11) {
	EL_INHERIT(backend);

	xcb_connection_t* connection;
	xcb_screen_t*     screen;
	xcb_cursor_t      cursor;

	struct {
		bool xkb;
		bool fullscreen;
	} has;

	struct {
		xcb_atom_t wm_protocols;
		xcb_atom_t wm_normal_hints;
		xcb_atom_t wm_size_hints;
		xcb_atom_t wm_delete_window;
		xcb_atom_t wm_class;

		xcb_atom_t net_wm_name;
		xcb_atom_t net_supporting_wm_check;
		xcb_atom_t net_supported;
		xcb_atom_t net_wm_icon;
		xcb_atom_t net_wm_state;
		xcb_atom_t net_wm_state_fullscreen;

		xcb_atom_t xkb_names;

		xcb_atom_t string;
		xcb_atom_t utf8_string;
		xcb_atom_t cardinal;
	} atom;
};

EL_CLASS(output_x11) {
	EL_INHERIT(output);

	xcb_window_t       window;
	el_output_mode     mode;
	wl_event_source_t finish_frame_timer;

	xcb_gc_t        gc;
	xcb_shm_seg_t   segment;
	pixman_image_t* surface;

	int     id;
	void*   buffer;
	uint8_t depth;
	int32_t scale;
};

el_backend_t el_backend_x11_create (el_compositor_t compositor, va_list args);

void el_backend_x11_destroy (el_backend_t self);

#endif
