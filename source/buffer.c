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
buffer_handle_destroy (wl_listener_t listener, void* data)
{
	(void) data;

	el_buffer_destroy(containerof(listener, buffer, destroy.listener));
}

WL_EXPORT el_buffer_t
el_buffer_from_resource (wl_resource_t resource)
{
	wl_listener_t listener = wl_resource_get_destroy_listener(resource,
		buffer_handle_destroy);

	if (listener) {
		return containerof(listener, buffer, destroy.listener);
	}

	el_buffer_t self = el_create(buffer);

	self->resource = resource;

	wl_signal_init(&self->destroy.signal);
	self->destroy.listener.notify = buffer_handle_destroy;

	self->inverted = true;
	wl_resource_add_destroy_listener(resource, &self->destroy.listener);

	return self;
}

WL_EXPORT void
el_buffer_destroy (el_buffer_t self)
{
	wl_signal_emit(&self->destroy.signal, self);

	el_destroy(self);
}

static void
reference_handle_destroy (wl_listener_t listener, void* data)
{
	el_buffer_reference_t self = containerof(listener, buffer_reference,
		destroy.listener);

	assert(data == self->buffer);

	self->buffer = NULL;
}

WL_EXPORT void
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
	self->destroy.listener.notify = reference_handle_destroy;
}
