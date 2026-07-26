set(PCEM_PRIVATE_API ${PCEM_PRIVATE_API}
        ${CMAKE_SOURCE_DIR}/includes/private/hdd/hdd_esdi.h
        ${CMAKE_SOURCE_DIR}/includes/private/hdd/hdd_file.h
        ${CMAKE_SOURCE_DIR}/includes/private/hdd/hdd.h
        ${CMAKE_SOURCE_DIR}/includes/private/hdd/ramdisk/ramdisk.h
        ${CMAKE_SOURCE_DIR}/includes/private/hdd/minivhd/cwalk.h
        ${CMAKE_SOURCE_DIR}/includes/private/hdd/minivhd/libxml2_encoding.h
        ${CMAKE_SOURCE_DIR}/includes/private/hdd/minivhd/minivhd_create.h
        ${CMAKE_SOURCE_DIR}/includes/private/hdd/minivhd/minivhd.h
        ${CMAKE_SOURCE_DIR}/includes/private/hdd/minivhd/minivhd_internal.h
        ${CMAKE_SOURCE_DIR}/includes/private/hdd/minivhd/minivhd_io.h
        ${CMAKE_SOURCE_DIR}/includes/private/hdd/minivhd/minivhd_struct_rw.h
        ${CMAKE_SOURCE_DIR}/includes/private/hdd/minivhd/minivhd_util.h
        )

set(PCEM_SRC ${PCEM_SRC}
        hdd/hdd.cpp
        hdd/hdd_esdi.cpp
        hdd/hdd_file.cpp
        )

# RAMDisk
set(PCEM_SRC ${PCEM_SRC}
        hdd/ramdisk/ramdisk.cpp
        )

# MiniVHD
set(PCEM_SRC ${PCEM_SRC}
        hdd/minivhd/cwalk.cpp
        hdd/minivhd/libxml2_encoding.cpp
        hdd/minivhd/minivhd_convert.cpp
        hdd/minivhd/minivhd_create.cpp
        hdd/minivhd/minivhd_io.cpp
        hdd/minivhd/minivhd_manage.cpp
        hdd/minivhd/minivhd_struct_rw.cpp
        hdd/minivhd/minivhd_util.cpp
        )
