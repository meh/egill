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

#ifndef EGILL_PROCESS_H
#define EGILL_PROCESS_H

#include <egill/common.h>

EL_CLASS(process);

EL_INTERFACE(process_cleanup) {
	void (*cleanup)(el_process_t self, int status);
};

EL_CLASS(process) {
	EL_IMPLEMENTS(process_cleanup);

	pid_t pid;
	void* private;

	wl_list link;
};

void el_process_cleanup (el_process_t self, int status);

#endif
