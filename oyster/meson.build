oyster_sources = files(
    'main.c',
    'server.c',
    'desktop/desktop.c',
    'desktop/output.c',
    'input/cursor.c',
    'input/input.c',
    'input/keyboard.c',
    'input/seat.c', )

    oyster_deps =
        [
            wayland_server,
            wlroots,
            xkbcommon,
        ]

    executable('oyster', oyster_sources, include_directories
               : [include], dependencies
               : oyster_deps, install
               : true, )
