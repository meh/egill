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

#ifndef EGILL_COMMON_H
#define EGILL_COMMON_H

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>

#include <pixman.h>
#include <egill/wayland.h>

#define lengthof(a) \
	(sizeof(a) / sizeof(a[0]))

#define max(a, b) ({ \
	__typeof__(a) _a = a; \
	__typeof__(b) _b = b; \
	_a > _b ? _a : _b; \
})

#define min(a, b) ({ \
	__typeof__(a) _a = a; \
	__typeof__(b) _b = b; \
	_a < _b ? _a : _b; \
})

#define containerof(ptr, type, member) ({ \
	const __typeof__(((type##_t) 0)->member)* __mptr = (ptr); \
	(type##_t) ((char*) __mptr - offsetof(struct type, member)); \
})

void* el_malloc (size_t size);

#define el_create(type) ({ \
	el_##type##_t pointer = el_malloc(sizeof(struct el_##type)); \
	memset(pointer, 0, sizeof(struct el_##type)); \
	pointer; \
})

void el_free (void* pointer);

#define el_destroy(what) \
	el_free(what)

int el_create_anonymous_file (off_t size);

void el_empty_region (pixman_region32_t* region);

void el_move_resources (wl_list_t destination, wl_list_t source);

void el_move_resources_for_client (wl_list_t destination, wl_list_t source,
                                   wl_client_t client);

void el_resource_unbind (wl_resource_t resource);

void el_resource_release (wl_client_t client, wl_resource_t resource);

#define EL_CLASS(name) \
	typedef struct el_##name el_##name; \
	typedef struct el_##name* el_##name##_t; \
	struct el_##name

#define EL_ENUM(name) \
	typedef enum el_##name el_##name; \
	typedef enum el_##name el_##name##_e; \
	enum el_##name

#define EL_INTERFACE(name) \
	typedef struct el_##name##_interface el_##name##_interface; \
	typedef struct el_##name##_interface* el_##name##_i; \
	struct el_##name##_interface

#define EL_INHERIT(name) \
	struct el_##name super

#define EL_SUPER(ptr) \
	(&(ptr)->super)

#define EL_IMPLEMENTS(...) \
	__el_implements(, ## __VA_ARGS__, \
		__el_implements2(__VA_ARGS__), \
		__el_implements1(__VA_ARGS__))

#define __el_implements(_, _1, _2, selected, ...) \
	selected

#define __el_implements1(name) \
	struct el_##name##_interface* method

#define __el_implements2(name, as) \
	struct el_##name##_interface* as

#define EL_IMPLEMENTATION(...) \
	__el_implementation(, ## __VA_ARGS__, \
		__el_implementation2(__VA_ARGS__), \
		__el_implementation1(__VA_ARGS__))

#define __el_implementation(_, _1, _2, selected, ...) \
	selected

#define __el_implementation1(name) \
	static struct el_##name##_interface name =

#define __el_implementation2(name, as) \
	static struct el_##name##_interface as =

#endif
