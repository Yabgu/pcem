set(PCEM_PRIVATE_API ${PCEM_PRIVATE_API}
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-app.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-common.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-createdisc.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-deviceconfig.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-dialogbox.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-display.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-glsl.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-glslp-parser.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-hostconfig.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-joystickconfig.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-sdl2-glw.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-sdl2.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-sdl2-video-gl3.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-sdl2-video.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-sdl2-video-renderer.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-shaderconfig.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-status.h
        ${CMAKE_SOURCE_DIR}/includes/private/wx-ui/wx-utils.h
        )

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/wx-ui)

find_program(WXRC_EXECUTABLE wxrc
        HINTS "${wxWidgets_ROOT_DIR}/tools/wxwidgets"
        REQUIRED)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/wx-ui/wx-resources.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/wx-ui/pc.xrc
        COMMAND ${WXRC_EXECUTABLE}
        ARGS -c ${CMAKE_CURRENT_SOURCE_DIR}/wx-ui/pc.xrc -o ${CMAKE_CURRENT_BINARY_DIR}/wx-ui/wx-resources.cpp)

set(PCEM_SRC ${PCEM_SRC}
        wx-ui/pc.xrc
        wx-ui/wx-main.cc
        wx-ui/wx-config_sel.cpp
        wx-ui/wx-dialogbox.cc
        wx-ui/wx-utils.cc
        wx-ui/wx-app.cc
        wx-ui/wx-sdl2-joystick.cpp
        wx-ui/wx-sdl2-mouse.cpp
        wx-ui/wx-sdl2-keyboard.cpp
        wx-ui/wx-sdl2-video.cpp
        wx-ui/wx-sdl2.cpp
        wx-ui/wx-config.cpp
        wx-ui/wx-deviceconfig.cc
        wx-ui/wx-status.cc
        wx-ui/wx-sdl2-status.cpp
        wx-ui/wx-thread.cpp
        wx-ui/wx-common.cpp
        wx-ui/wx-sdl2-video-renderer.cpp
        wx-ui/wx-sdl2-video-gl3.cpp
        wx-ui/wx-glslp-parser.cpp
        wx-ui/wx-shader_man.cpp
        wx-ui/wx-shaderconfig.cc
        wx-ui/wx-joystickconfig.cc
        wx-ui/wx-config-eventbinder.cc
        wx-ui/wx-createdisc.cc
        ${CMAKE_CURRENT_BINARY_DIR}/wx-ui/wx-resources.cpp
        )

if(USE_NETWORKING)
        set(PCEM_SRC ${PCEM_SRC}
                wx-ui/wx-hostconfig.cpp
                )
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        set(PCEM_SRC ${PCEM_SRC}
                wx-ui/wx-sdl2-display.cpp
                )
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        set(PCEM_SRC ${PCEM_SRC}
                wx-ui/wx-sdl2-display-win.cpp
                wx-ui/wx.rc
                )
endif()


if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        set(PCEM_SRC ${PCEM_SRC}
                wx-ui/wx-sdl2-display.cpp
                )

        add_compile_definitions(PCEM_RENDER_WITH_TIMER PCEM_RENDER_TIMER_LOOP)

endif()
