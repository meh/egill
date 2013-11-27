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

#include <egill/buffer.h>

static void
handle_destroy (wl_listener_t listener, void* data)
{
	el_buffer_reference_t self = containerof(listener, el_buffer_reference,
		destroy.listener);

	assert(data == self->buffer);

	self->buffer = NULL;
}

void
el_buffer_point (el_buffer_reference_t self, el_buffer_t buffer)
{
	if (self->buffer && buffer != self->buffer) {
		self->buffer->busy--;

		if (self->buffer->busy == 0) {
			assert(wl_resource_get_client(self->buffer->resource));

			wl_resource_queue_event(self->buffer->resource, WL_BUFFER_RELEASE);
		}

		wl_list_remove(&self->destroy.listener.link);
	}

	if (buffer && buffer != self->buffer) {
		buffer->busy++;

		wl_signal_add(&buffer->destroy.signal, &self->destroy.listener);
	}

	self->buffer                  = buffer;
	self->destroy.listener.notify = handle_destroy;
}

