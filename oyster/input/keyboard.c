/* Copyright (c), Niclas Meyer <niclas@countingsort.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "input/keyboard.h"

#include <stdbool.h>
#include <stdlib.h>

#include <wlr/backend.h>
#include <wlr/backend/multi.h>
#include <wlr/types/wlr_input_device.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/util/log.h>
#include <xkbcommon/xkbcommon.h>

#include "input/input.h"

static bool
switch_vt(const xkb_keycode_t *syms, int nsyms, struct wlr_backend *backend)
{
    for (int i = 0; i < nsyms; ++i) {
        const xkb_keysym_t sym = syms[i];

        if (sym >= XKB_KEY_XF86Switch_VT_1 && sym <= XKB_KEY_XF86Switch_VT_12) {
            if (wlr_backend_is_multi(backend)) {
                struct wlr_session *session = wlr_backend_get_session(backend);
                if (session) {
                    unsigned int vt = sym - XKB_KEY_XF86Switch_VT_1 + 1;
                    wlr_session_change_vt(session, vt);
                }
            }

            return true;
        }
    }

    return false;
}

static void
keyboard_key_notify(struct wl_listener *listener, void *data)
{
    struct oys_keyboard *keyboard = wl_container_of(listener, keyboard, key);
    struct oys_server *server     = keyboard->input->server;
    struct wlr_event_keyboard_key *event = data;

    uint32_t keycode = event->keycode + 8;

    const xkb_keysym_t *translated_syms;
    int translated_syms_len = xkb_state_key_get_syms(
        keyboard->device->keyboard->xkb_state, keycode, &translated_syms);

    switch_vt(translated_syms, translated_syms_len, server->backend);
}

static void
keyboard_modifiers_notify(struct wl_listener *listener, void *UNUSED(data))
{
    struct oys_keyboard *keyboard =
        wl_container_of(listener, keyboard, modifiers);
    struct oys_seat *seat = keyboard->input->seat;

    wlr_seat_set_keyboard(seat->seat, keyboard->device);
    wlr_seat_keyboard_notify_modifiers(
        seat->seat, &keyboard->device->keyboard->modifiers);
}

static void
keyboard_destroy_notify(struct wl_listener *listener, void *UNUSED(data))
{
    struct oys_keyboard *keyboard =
        wl_container_of(listener, keyboard, destroy);

    wl_list_remove(&keyboard->link);
    wl_list_remove(&keyboard->key.link);
    wl_list_remove(&keyboard->modifiers.link);
    wl_list_remove(&keyboard->destroy.link);

    free(keyboard);
}

struct oys_keyboard *
keyboard_create(struct oys_input *input, struct wlr_input_device *device)
{
    struct oys_keyboard *keyboard = malloc(sizeof(*keyboard));
    if (!keyboard) {
        wlr_log(WLR_ERROR, "Failed to allocate keyboard");
        return NULL;
    }

    keyboard->input  = input;
    keyboard->device = device;

    keyboard->key.notify       = keyboard_key_notify;
    keyboard->modifiers.notify = keyboard_modifiers_notify;
    keyboard->destroy.notify   = keyboard_destroy_notify;

    struct xkb_rule_names rules = {0};
    struct xkb_context *context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    struct xkb_keymap *keymap =
        xkb_map_new_from_names(context, &rules, XKB_KEYMAP_COMPILE_NO_FLAGS);
    wlr_keyboard_set_keymap(device->keyboard, keymap);
    xkb_keymap_unref(keymap);
    xkb_context_unref(context);
    wlr_keyboard_set_repeat_info(device->keyboard, 25, 600);

    wlr_seat_set_keyboard(input->seat->seat, device);

    wl_signal_add(&device->keyboard->events.key, &keyboard->key);
    wl_signal_add(&device->keyboard->events.modifiers, &keyboard->modifiers);
    wl_signal_add(&device->events.destroy, &keyboard->destroy);

    return keyboard;
}
