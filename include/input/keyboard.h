/* Copyright (c), Niclas Meyer <niclas@countingsort.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef OYSTER_INPUT_KEYBOARD_H
#define OYSTER_INPUT_KEYBOARD_H

#include "input/cursor.h"

#include <wayland-server.h>
#include <wlr/types/wlr_input_device.h>

struct oys_keyboard {
    struct wl_list link;
    struct oys_input *input;
    struct wlr_input_device *device;

    struct wl_listener key;
    struct wl_listener modifiers;
    struct wl_listener destroy;
};

struct oys_keyboard *
keyboard_create(struct oys_input *input, struct wlr_input_device *device);

#endif /* OYSTER_INPUT_KEYBOARD_H */
