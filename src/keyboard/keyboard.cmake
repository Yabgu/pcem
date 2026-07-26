set(PCEM_PRIVATE_API ${PCEM_PRIVATE_API}
        ${CMAKE_SOURCE_DIR}/includes/private/keyboard/keyboard_amstrad.h
        ${CMAKE_SOURCE_DIR}/includes/private/keyboard/keyboard_at.h
        ${CMAKE_SOURCE_DIR}/includes/private/keyboard/keyboard.h
        ${CMAKE_SOURCE_DIR}/includes/private/keyboard/keyboard_olim24.h
        ${CMAKE_SOURCE_DIR}/includes/private/keyboard/keyboard_pcjr.h
        ${CMAKE_SOURCE_DIR}/includes/private/keyboard/keyboard_xt.h
        )

set(PCEM_SRC ${PCEM_SRC}
        keyboard/keyboard.cpp
        keyboard/keyboard_amstrad.cpp
        keyboard/keyboard_at.cpp
        keyboard/keyboard_olim24.cpp
        keyboard/keyboard_pcjr.cpp
        keyboard/keyboard_xt.cpp
        )