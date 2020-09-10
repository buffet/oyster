/* Copyright (c), Niclas Meyer <niclas@countingsort.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef OYSTER_SERVER_H
#define OYSTER_SERVER_H

#include <stdbool.h>

#include <wayland-server.h>
#include <wlr/backend.h>

#include "desktop/desktop.h"
#include "input/input.h"

struct oys_server {
    struct wl_display *display;
    struct wlr_backend *backend;
    const char *socket;

    struct oys_desktop *desktop;
    struct oys_input *input;
};

struct oys_server *server_create(void);
bool server_run(struct oys_server *server);
void server_destroy(struct oys_server *server);

#endif /* OYSTER_SERVER_H */
