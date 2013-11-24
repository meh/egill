#ifndef EGILL_WAYLAND_H
#define EGILL_WAYLAND_H

#define WL_HIDE_DEPRECATED
#include <wayland-server.h>

typedef struct wl_list         wl_list;
typedef struct wl_array        wl_array;
typedef struct wl_shm_buffer   wl_shm_buffer;
typedef struct wl_client       wl_client;
typedef struct wl_resource     wl_resource;
typedef struct wl_signal       wl_signal;
typedef struct wl_listener     wl_listener;
typedef struct wl_display      wl_display;
typedef struct wl_event_loop   wl_event_loop;
typedef struct wl_event_source wl_event_source;
typedef struct wl_global       wl_global;

typedef wl_list*         wl_list_t;
typedef wl_array*        wl_array_t;
typedef wl_shm_buffer*   wl_shm_buffer_t;
typedef wl_client*       wl_client_t;
typedef wl_resource*     wl_resource_t;
typedef wl_signal*       wl_signal_t;
typedef wl_listener*     wl_listener_t;
typedef wl_display*      wl_display_t;
typedef wl_event_loop*   wl_event_loop_t;
typedef wl_event_source* wl_event_source_t;
typedef wl_global*       wl_global_t;

typedef enum wl_output_transform wl_output_transform_e;
typedef enum wl_seat_capability wl_seat_capability_e;

#endif
