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

#ifndef EGILL_COMPOSITOR_H
#define EGILL_COMPOSITOR_H

#include <egill/common.h>
#include <egill/process.h>

EL_ENUM(compositor_state) {
	EL_COMPOSITOR_ACTIVE,
	EL_COMPOSITOR_IDLE,
	EL_COMPOSITOR_OFFSCREEN,
	EL_COMPOSITOR_SLEEPING
};

EL_ENUM(compositor_capability) {
	EL_CAPABILITY_ROTATION_ANY  = 1 << 0,
	EL_CAPABILITY_CAPTURE_YFLIP = 1 << 1,
};

EL_CLASS(compositor) {
	const char*     socket;
	wl_display_t    display;
	wl_event_loop_t loop;

	struct el_renderer* renderer;
	struct el_backend*  backend;

	el_compositor_state_e state;

	wl_list processes;
};

/*!
 * Create a compositor on the given socket name.
 *
 * Once the compositor has been created you can register for events, register a
 * backend, register a renderer and finally run the compositor.
 *
 * \param socket the name of the socket, if NULL "wayland-0" will be used
 *
 * \return a new compositor instance
 *
 * \memberof el_compositor_t
 */
el_compositor_t el_compositor_create (const char* socket);

/*!
 * Destroy the compositor.
 *
 * \memberof el_compositor_t
 */
void el_compositor_destroy (el_compositor_t self);

/*!
 * Run the compositor, this function will block and run the event loop.
 *
 * \memberof el_compositor_t
 */
bool el_compositor_run (el_compositor_t self);

void el_compositor_watch_process (el_compositor_t self, el_process_t process);

#endif
