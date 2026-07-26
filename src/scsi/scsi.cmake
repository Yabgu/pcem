set(PCEM_PRIVATE_API ${PCEM_PRIVATE_API}
        ${CMAKE_SOURCE_DIR}/includes/private/scsi/scsi_53c400.h
        ${CMAKE_SOURCE_DIR}/includes/private/scsi/scsi_aha1540.h
        ${CMAKE_SOURCE_DIR}/includes/private/scsi/scsi_cd.h
        ${CMAKE_SOURCE_DIR}/includes/private/scsi/scsi.h
        ${CMAKE_SOURCE_DIR}/includes/private/scsi/scsi_hd.h
        ${CMAKE_SOURCE_DIR}/includes/private/scsi/scsi_ibm.h
        ${CMAKE_SOURCE_DIR}/includes/private/scsi/scsi_zip.h
        )

set(PCEM_SRC ${PCEM_SRC}
        scsi/scsi.cpp
        scsi/scsi_53c400.cpp
        scsi/scsi_aha1540.cpp
        scsi/scsi_cd.cpp
        scsi/scsi_hd.cpp
        scsi/scsi_ibm.cpp
        scsi/scsi_zip.cpp
        )