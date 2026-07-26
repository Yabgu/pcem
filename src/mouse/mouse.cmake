set(PCEM_PRIVATE_API ${PCEM_PRIVATE_API}
        ${CMAKE_SOURCE_DIR}/includes/private/mouse/mouse.h
        ${CMAKE_SOURCE_DIR}/includes/private/mouse/mouse_msystems.h
        ${CMAKE_SOURCE_DIR}/includes/private/mouse/mouse_ps2.h
        ${CMAKE_SOURCE_DIR}/includes/private/mouse/mouse_serial.h
        )

set(PCEM_SRC ${PCEM_SRC}
        mouse/mouse.cpp
        mouse/mouse_msystems.cpp
        mouse/mouse_ps2.cpp
        mouse/mouse_serial.cpp
        )