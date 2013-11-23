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
	type##_t pointer = el_malloc(sizeof(struct type)); \
	memset(pointer, 0, sizeof(struct type)); \
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

#define EL_OBJECT(name) \
	typedef struct el_##name el_##name; \
	typedef el_##name* el_##name##_t; \
	struct el_##name

#define EL_ENUM(name) \
	typedef enum el_##name el_##name; \
	typedef el_##name el_##name##_e; \
	enum el_##name

#define EL_INTERFACE(name) \
	typedef struct el_##name el_##name##_interface; \
	typedef el_##name* el_##name##_i; \
	struct el_##name##_interface \

#endif
