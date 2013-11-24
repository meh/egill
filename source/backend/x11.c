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

#include <egill/backend/x11.h>

static void
get_resources (el_backend_x11_t self)
{
	#define F(field) offsetof(struct el_backend_x11, field)
	static const struct {
		const char* name;
		int         offset;
	} atoms[] = {
		{ "WM_PROTOCOLS",     F(atom.wm_protocols) },
		{ "WM_NORMAL_HINTS",  F(atom.wm_normal_hints) },
		{ "WM_SIZE_HINTS",    F(atom.wm_size_hints) },
		{ "WM_DELETE_WINDOW", F(atom.wm_delete_window) },
		{ "WM_CLASS",         F(atom.wm_class) },

		{ "_NET_WM_NAME",             F(atom.net_wm_name) },
		{ "_NET_WM_ICON",             F(atom.net_wm_icon) },
		{ "_NET_WM_STATE",            F(atom.net_wm_state) },
		{ "_NET_WM_STATE_FULLSCREEN", F(atom.net_wm_state_fullscreen) },
		{ "_NET_SUPPORTING_WM_CHECK", F(atom.net_supporting_wm_check) },
		{ "_NET_SUPPORTED",           F(atom.net_supported) },

		{ "_XKB_RULES_NAMES", F(atom.xkb_names) },

		{ "STRING",      F(atom.string) },
		{ "UTF8_STRING", F(atom.utf8_string) },
		{ "CARDINAL",    F(atom.cardinal) },
	};
	#undef F

	uint8_t data[] = { 0, 0, 0, 0 };

	xcb_intern_atom_cookie_t cookies[lengthof(atoms)];

	for (size_t i = 0; i < lengthof(atoms); i++) {
		cookies[i] = xcb_intern_atom(self->connection, 0,
			strlen(atoms[i].name), atoms[i].name);
	}

	for (size_t i = 0; i < lengthof(atoms); i++) {
		xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(self->connection,
			cookies[i], NULL);

		*(xcb_atom_t*) ((char*) self + atoms[i].offset) = reply->atom;

		free(reply);
	}

	xcb_pixmap_t pixmap = xcb_generate_id(self->connection);
	xcb_gc_t     gc     = xcb_generate_id(self->connection);

	xcb_create_pixmap(self->connection, 1, pixmap, self->screen->root, 1, 1);
	xcb_create_gc(self->connection, gc, pixmap, 0, NULL);
	xcb_put_image(self->connection, XCB_IMAGE_FORMAT_XY_PIXMAP,
		pixmap, gc, 1, 1, 0, 0, 0, 32, sizeof(data), data);

	self->cursor = xcb_generate_id(self->connection);
	xcb_create_cursor(self->connection, self->cursor, pixmap, pixmap,
		0, 0, 0, 0, 0, 0, 1, 1);

	xcb_free_gc(self->connection, gc);
	xcb_free_pixmap(self->connection, pixmap);
}

static void
get_wm_info (el_backend_x11_t self)
{
	xcb_get_property_cookie_t cookie = xcb_get_property(self->connection, 0,
		self->screen->root, self->atom.net_supported, XCB_ATOM_ATOM, 0, 1024);

	xcb_get_property_reply_t* reply;

	if ((reply = xcb_get_property_reply(self->connection, cookie, NULL)) == NULL) {
		return;
	}

	xcb_atom_t* atom = (xcb_atom_t*) xcb_get_property_value(reply);

	for (size_t i = 0; i < reply->value_len; i++) {
		if (atom[i] == self->atom.net_wm_state_fullscreen) {
			self->has.fullscreen = true;
		}
	}
}

WL_EXPORT el_backend_t
el_backend_x11_create (el_compositor_t compositor, va_list args)
{
	const char* name;
	      bool  fullscreen = false;
	      int   width      = 1024;
	      int   height     = 640;

	while ((name = va_arg(args, const char*)) != NULL) {
		if (strcmp(name, "fullscreen") == 0) {
			fullscreen = va_arg(args, int);
		}
		else if (strcmp(name, "width") == 0) {
			width = va_arg(args, int);
		}
		else if (strcmp(name, "height") == 0) {
			height = va_arg(args, int);
		}
		else {
			return NULL;
		}
	}

	el_backend_x11_t self = el_create(backend_x11);

	EL_SUPER(self)->name       = "x11";
	EL_SUPER(self)->compositor = compositor;

	self->connection = xcb_connect(NULL, NULL);

	if (xcb_connection_has_error(self->connection)) {
		goto fail;
	}

	self->screen = xcb_setup_roots_iterator(xcb_get_setup(self->connection)).data;

	get_resources(self);
	get_wm_info(self);

	if (fullscreen && !self->has.fullscreen) {
		fullscreen = 0;
	}

	if (!create_input(self)) {
		goto fail;
	}

	create_output(self, 0, 0, width, height, fullscreen,
		NULL, WL_OUTPUT_TRANSFORM_NORMAL, 1);

	return (el_backend_t) self;

fail:
	el_backend_x11_destroy((el_backend_t) self);

	return NULL;
}

WL_EXPORT void
el_backend_x11_destroy (el_backend_t backend)
{
	el_backend_x11_t self = (el_backend_x11_t) backend;

	if (self->connection) {
		xcb_disconnect(self->connection);
	}

	el_destroy(self);
}
