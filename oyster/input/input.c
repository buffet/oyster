/* Copyright (c), Niclas Meyer <niclas@countingsort.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "input/input.h"

#include <stdlib.h>

#include <wlr/types/wlr_input_device.h>
#include <wlr/util/log.h>

#include "input/keyboard.h"

static void
new_pointer(struct oys_input *input, struct wlr_input_device *device)
{
    wlr_cursor_attach_input_device(input->cursor->cursor, device);
}

static void
new_keyboard(struct oys_input *input, struct wlr_input_device *device)
{
    struct oys_keyboard *keyboard = keyboard_create(input, device);
    if (!keyboard) {
        wlr_log(WLR_ERROR, "Failed to create keyboard");
        return;
    }

    wl_list_insert(&input->keyboards, &keyboard->link);
}

static void
new_input_notify(struct wl_listener *listener, void *data)
{
    struct oys_input *input = wl_container_of(listener, input, new_input);
    struct wlr_input_device *device = data;

    wlr_log(WLR_DEBUG, "New input %p, %s", device, device->name);

    switch (device->type) {
    case WLR_INPUT_DEVICE_POINTER:
        new_pointer(input, device);
        break;
    case WLR_INPUT_DEVICE_KEYBOARD:
        new_keyboard(input, device);
        break;
    default:
        // EMPTY
        break;
    }

    uint32_t caps = WL_SEAT_CAPABILITY_POINTER;
    if (!wl_list_empty(&input->keyboards)) {
        caps |= WL_SEAT_CAPABILITY_KEYBOARD;
    }

    wlr_seat_set_capabilities(input->seat->seat, caps);
}

struct oys_input *
input_create(struct oys_server *server)
{
    struct oys_input *input = malloc(sizeof(*input));
    if (!input) {
        wlr_log(WLR_ERROR, "Failed to allocate input");
        return NULL;
    }

    input->server = server;

    input->seat = seat_create(input);
    if (!input->seat) {
        wlr_log(WLR_ERROR, "Failed to create seat");
        return NULL;
    }

    input->cursor = cursor_create(input);
    if (!input->cursor) {
        wlr_log(WLR_ERROR, "Failed to create cursor");
        seat_destroy(input->seat);
        return NULL;
    }

    input->new_input.notify = new_input_notify;
    wl_signal_add(&server->backend->events.new_input, &input->new_input);

    wl_list_init(&input->keyboards);

    return input;
}

void
input_destroy(struct oys_input *input)
{
    seat_destroy(input->seat);

    free(input);
}
