/* Copyright (c), Niclas Meyer <niclas@countingsort.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "input/cursor.h"

#include <stdlib.h>

#include <wlr/types/wlr_seat.h>
#include <wlr/util/log.h>

#include "desktop/desktop.h"
#include "server.h"

static void
process_cursor_motion(struct oys_server *server, uint32_t UNUSED(time))
{
    struct oys_cursor *cursor = server->input->cursor;

    wlr_xcursor_manager_set_cursor_image(
        cursor->xcursor_manager, "left_ptr", cursor->cursor);
}

static void
cursor_motion_notify(struct wl_listener *listener, void *data)
{
    struct oys_cursor *cursor = wl_container_of(listener, cursor, motion);
    struct wlr_event_pointer_motion *event = data;

    wlr_cursor_move(
        cursor->cursor, event->device, event->delta_x, event->delta_y);

    process_cursor_motion(cursor->input->server, event->time_msec);
}

static void
cursor_motion_absolute_notify(struct wl_listener *listener, void *data)
{
    struct oys_cursor *cursor =
        wl_container_of(listener, cursor, motion_absolute);
    struct wlr_event_pointer_motion_absolute *event = data;

    wlr_cursor_warp_absolute(cursor->cursor, event->device, event->x, event->y);

    process_cursor_motion(cursor->input->server, event->time_msec);
}

static void
cursor_frame_notify(struct wl_listener *listener, void *UNUSED(data))
{
    struct oys_cursor *cursor = wl_container_of(listener, cursor, frame);
    struct oys_input *input   = cursor->input;

    wlr_seat_pointer_notify_frame(input->seat->seat);
}

struct oys_cursor *
cursor_create(struct oys_input *input)
{
    struct oys_desktop *desktop = input->server->desktop;

    struct oys_cursor *cursor = malloc(sizeof(*cursor));
    if (!cursor) {
        wlr_log(WLR_ERROR, "Failed to allocate cursor");
        return NULL;
    }

    cursor->input = input;

    cursor->cursor = wlr_cursor_create();
    if (!cursor->cursor) {
        wlr_log(WLR_ERROR, "Failed to create wlr_cursor");
        free(cursor);
        return NULL;
    }

    cursor->xcursor_manager = wlr_xcursor_manager_create(NULL, 24);

    wlr_cursor_attach_output_layout(cursor->cursor, desktop->output_layout);

    cursor->motion.notify          = cursor_motion_notify;
    cursor->motion_absolute.notify = cursor_motion_absolute_notify;
    cursor->frame.notify           = cursor_frame_notify;

    wl_signal_add(&cursor->cursor->events.motion, &cursor->motion);
    wl_signal_add(
        &cursor->cursor->events.motion_absolute, &cursor->motion_absolute);
    wl_signal_add(&cursor->cursor->events.frame, &cursor->frame);

    return cursor;
}

void
cursor_destroy(struct oys_cursor *cursor)
{
    wlr_cursor_destroy(cursor->cursor);
    wlr_xcursor_manager_destroy(cursor->xcursor_manager);

    wl_list_remove(&cursor->motion.link);
    wl_list_remove(&cursor->motion_absolute.link);

    free(cursor);
}
