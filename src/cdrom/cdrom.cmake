set(PCEM_PRIVATE_API ${PCEM_PRIVATE_API}
        ${CMAKE_SOURCE_DIR}/includes/private/cdrom/cdrom-image.h
        ${CMAKE_SOURCE_DIR}/includes/private/cdrom/cdrom-ioctl.h
        ${CMAKE_SOURCE_DIR}/includes/private/cdrom/cdrom-null.h
        )

set(PCEM_SRC ${PCEM_SRC}
        cdrom/cdrom-image.cc
        cdrom/cdrom-null.cpp
        )

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        set(PCEM_SRC ${PCEM_SRC} ${PCEM_SRC_CDROM}
                cdrom/cdrom-ioctl-linux.cpp
                )
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        set(PCEM_SRC ${PCEM_SRC} ${PCEM_SRC_CDROM}
                cdrom/cdrom-ioctl.cpp
                )
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        set(PCEM_SRC ${PCEM_SRC} ${PCEM_SRC_CDROM}
                cdrom/cdrom-ioctl-osx.cpp
                )
endif()

