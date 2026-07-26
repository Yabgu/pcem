set(PCEM_PRIVATE_API ${PCEM_PRIVATE_API}
        ${CMAKE_SOURCE_DIR}/includes/private/mfm/mfm_at.h
        ${CMAKE_SOURCE_DIR}/includes/private/mfm/mfm_xebec.h
        )

set(PCEM_SRC ${PCEM_SRC}
        mfm/mfm_at.cpp
        mfm/mfm_xebec.cpp
        )