set(PCEM_PRIVATE_API ${PCEM_PRIVATE_API}
        ${CMAKE_SOURCE_DIR}/includes/private/joystick/gameport.h
        ${CMAKE_SOURCE_DIR}/includes/private/joystick/joystick_ch_flightstick_pro.h
        ${CMAKE_SOURCE_DIR}/includes/private/joystick/joystick_standard.h
        ${CMAKE_SOURCE_DIR}/includes/private/joystick/joystick_sw_pad.h
        ${CMAKE_SOURCE_DIR}/includes/private/joystick/joystick_tm_fcs.h
        )

set(PCEM_SRC ${PCEM_SRC}
        joystick/gameport.cpp
        joystick/joystick_ch_flightstick_pro.cpp
        joystick/joystick_standard.cpp
        joystick/joystick_sw_pad.cpp
        joystick/joystick_tm_fcs.cpp
        )