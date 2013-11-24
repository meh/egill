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

#ifndef EGILL_XWAYLAND_H
#define EGILL_XWAYLAND_H

#include <egill/common.h>
#include <egill/process.h>

EL_CLASS(compositor);

EL_CLASS(xwayland) {
	el_compositor_t compositor;

	int   display;
	char* lock;
	
	struct {
		int abstract;
		int unix;
	} fd;

	el_process    process;
	wl_client_t   client;
	wl_resource_t resource;
};

/*!
 * Start X with Wayland support.
 */
el_xwayland_t xwayland_start (el_compositor_t compositor);

/*!
 * Stop the started X server.
 */
void xwayland_stop (el_xwayland_t);

#endif
