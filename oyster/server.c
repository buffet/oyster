/* Copyright (c), Niclas Meyer <niclas@countingsort.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "server.h"

#include <stdlib.h>

#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_screencopy_v1.h>
#include <wlr/util/log.h>

struct oys_server *
server_create(void)
{
    wlr_log(WLR_DEBUG, "Initializing Wayland server");

    struct oys_server *server = malloc(sizeof(*server));
    if (!server) {
        wlr_log(WLR_ERROR, "Failed to allocate server");
        return NULL;
    }

    server->display = wl_display_create();
    if (!server->display) {
        wlr_log(WLR_ERROR, "Failed to create display");
        return NULL;
    }

    server->backend = wlr_backend_autocreate(server->display, NULL);
    if (!server->backend) {
        wlr_log(WLR_ERROR, "Failed to create backend");
        wl_display_destroy(server->display);
        return NULL;
    }

    struct wlr_renderer *renderer = wlr_backend_get_renderer(server->backend);
    wlr_renderer_init_wl_display(renderer, server->display);

    server->desktop = desktop_create(server, renderer);
    if (!server->desktop) {
        wlr_log(WLR_ERROR, "Failed to initialize desktop");
        wl_display_destroy(server->display);
        return NULL;
    }

    server->input = input_create(server);
    if (!server->input) {
        wlr_log(WLR_ERROR, "Failed to initialize input");
        desktop_destroy(server->desktop);
        wl_display_destroy(server->display);
        return NULL;
    }

    server->socket = wl_display_add_socket_auto(server->display);
    if (!server->socket) {
        wlr_log(WLR_ERROR, "Failed to open Wayland socket");
        input_destroy(server->input);
        desktop_destroy(server->desktop);
        wl_display_destroy(server->display);
        return NULL;
    }

    wlr_screencopy_manager_v1_create(server->display);

    return server;
}

bool
server_run(struct oys_server *server)
{
    wlr_log(WLR_INFO, "Running Wayland server on display '%s'", server->socket);

    setenv("WAYLAND_DISPLAY", server->socket, true);

    if (!wlr_backend_start(server->backend)) {
        wlr_log(WLR_ERROR, "Failed to start backend");
        wl_display_destroy(server->display);
        return false;
    }

    wl_display_run(server->display);

    return true;
}

void
server_destroy(struct oys_server *server)
{
    wlr_log(WLR_DEBUG, "Shutting down Wayland server");

    wl_display_destroy_clients(server->display);

    input_destroy(server->input);
    desktop_destroy(server->desktop);

    wl_display_destroy(server->display);

    free(server);
}
