set(PCEM_PRIVATE_API ${PCEM_PRIVATE_API}
        ${CMAKE_SOURCE_DIR}/includes/private/sound/ayumi/ayumi.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/resid-fp/envelope.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/resid-fp/extfilt.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/resid-fp/filter.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/resid-fp/pot.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/resid-fp/siddefs-fp.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/resid-fp/sid.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/resid-fp/voice.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/resid-fp/wave.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_ad1848.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_adlibgold.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_adlib.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_audiopci.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_azt2316a.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_cms.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_dbopl.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_emu8k.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_gus.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_mmb.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_mpu401_uart.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_opl.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_pas16.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_ps1.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_pssj.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_resid.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_sb_dsp.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_sb.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_sn76489.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_speaker.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_ssi2001.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_wss.h
        ${CMAKE_SOURCE_DIR}/includes/private/sound/sound_ym7128.h
        )

set(PCEM_SRC ${PCEM_SRC}
        sound/sound.cpp
        sound/sound_ad1848.cpp
        sound/sound_adlib.cpp
        sound/sound_adlibgold.cpp
        sound/sound_audiopci.cpp
        sound/sound_azt2316a.cpp
        sound/sound_cms.cpp
        sound/sound_dbopl.cc
        sound/sound_emu8k.cpp
        sound/sound_gus.cpp
        sound/sound_mmb.cpp
        sound/sound_mpu401_uart.cpp
        sound/sound_opl.cpp
        sound/sound_pas16.cpp
        sound/sound_ps1.cpp
        sound/sound_pssj.cpp
        sound/sound_resid.cc
        sound/sound_sb.cpp
        sound/sound_sb_dsp.cpp
        sound/sound_sn76489.cpp
        sound/sound_speaker.cpp
        sound/sound_ssi2001.cpp
        sound/sound_wss.cpp
        sound/sound_ym7128.cpp
        sound/soundopenal.cpp
        )

# AYUMI
set(PCEM_SRC ${PCEM_SRC}
        sound/ayumi/ayumi.cpp
        )

# RESID-FP
set(PCEM_SRC ${PCEM_SRC}
        sound/resid-fp/convolve.cc
        sound/resid-fp/convolve-sse.cc
        sound/resid-fp/envelope.cc
        sound/resid-fp/extfilt.cc
        sound/resid-fp/filter.cc
        sound/resid-fp/pot.cc
        sound/resid-fp/sid.cc
        sound/resid-fp/voice.cc
        sound/resid-fp/wave6581_PS_.cc
        sound/resid-fp/wave6581_PST.cc
        sound/resid-fp/wave6581_P_T.cc
        sound/resid-fp/wave6581__ST.cc
        sound/resid-fp/wave8580_PS_.cc
        sound/resid-fp/wave8580_PST.cc
        sound/resid-fp/wave8580_P_T.cc
        sound/resid-fp/wave8580__ST.cc
        sound/resid-fp/wave.cc
        )

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux" AND USE_ALSA)
        set(PCEM_SRC ${PCEM_SRC}
                sound/midi_alsa.cpp
                )
        set(PCEM_ADDITIONAL_LIBS ${PCEM_ADDITIONAL_LIBS} ${ALSA_LIBRARIES})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(PCEM_SRC ${PCEM_SRC}
                sound/win-midi.cpp
                )
                set(PCEM_ADDITIONAL_LIBS ${PCEM_ADDITIONAL_LIBS} winmm)
else()
    message(STATUS "Warning: Using sdl2-midi. It currently is an empty midi implementation")
        set(PCEM_SRC ${PCEM_SRC}
                sound/sdl2-midi.cpp
                )
endif()
