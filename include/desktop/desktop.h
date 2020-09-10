/* Copyright (c), Niclas Meyer <niclas@countingsort.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef OYSTER_DESKTOP_DESKTOP_H
#define OYSTER_DESKTOP_DESKTOP_H

#include <stdbool.h>

#include <wayland-server.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_output_layout.h>

#include "server.h"

struct oys_desktop {
    struct oys_server *server;
    struct wlr_compositor *compositor;
    struct wlr_data_device_manager *data_device_manager;
    struct wlr_output_layout *output_layout;
    struct wl_list outputs; // struct oys_output::link

    struct wl_listener new_output;
};

struct oys_desktop *
desktop_create(struct oys_server *server, struct wlr_renderer *renderer);
void desktop_destroy(struct oys_desktop *desktop);

#endif /* OYSTER_DESKTOP_DESKTOP_H */
