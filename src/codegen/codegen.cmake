set(PCEM_PRIVATE_API ${PCEM_PRIVATE_API}
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_accumulate.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_allocator.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_arm64_defs.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_arm64.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_arm64_ops.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_arm_defs.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_arm.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_arm_ops.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_x86-64_defs.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_x86-64.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_x86-64_ops.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_x86-64_ops_helpers.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_x86-64_ops_sse.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_x86_defs.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_x86.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_x86_ops_fpu.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_x86_ops.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_x86_ops_helpers.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_backend_x86_ops_sse.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ir_defs.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ir.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_3dnow.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_arith.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_branch.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_fpu_arith.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_fpu_constant.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_fpu_loadstore.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_fpu_misc.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_helpers.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_jump.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_logic.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_misc.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_mmx_arith.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_mmx_cmp.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_mmx_loadstore.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_mmx_logic.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_mmx_pack.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_mmx_shift.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_mov.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_shift.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_ops_stack.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_reg.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_timing_common.h
        ${CMAKE_SOURCE_DIR}/includes/private/codegen/codegen_x86-64.h
        )

set(PCEM_SRC ${PCEM_SRC}
        codegen/codegen.cpp
        codegen/codegen_accumulate.cpp
        codegen/codegen_allocator.cpp
        codegen/codegen_block.cpp
        codegen/codegen_ir.cpp
        codegen/codegen_ops.cpp
        codegen/codegen_ops_3dnow.cpp
        codegen/codegen_ops_arith.cpp
        codegen/codegen_ops_branch.cpp
        codegen/codegen_ops_fpu_arith.cpp
        codegen/codegen_ops_fpu_constant.cpp
        codegen/codegen_ops_fpu_loadstore.cpp
        codegen/codegen_ops_fpu_misc.cpp
        codegen/codegen_ops_helpers.cpp
        codegen/codegen_ops_jump.cpp
        codegen/codegen_ops_logic.cpp
        codegen/codegen_ops_misc.cpp
        codegen/codegen_ops_mmx_arith.cpp
        codegen/codegen_ops_mmx_cmp.cpp
        codegen/codegen_ops_mmx_loadstore.cpp
        codegen/codegen_ops_mmx_logic.cpp
        codegen/codegen_ops_mmx_pack.cpp
        codegen/codegen_ops_mmx_shift.cpp
        codegen/codegen_ops_mov.cpp
        codegen/codegen_ops_shift.cpp
        codegen/codegen_ops_stack.cpp
        codegen/codegen_reg.cpp
        codegen/codegen_timing_486.cpp
        codegen/codegen_timing_686.cpp
        codegen/codegen_timing_common.cpp
        codegen/codegen_timing_cyrixiii.cpp
        codegen/codegen_timing_k6.cpp
        codegen/codegen_timing_p6.cpp
        codegen/codegen_timing_pentium.cpp
        codegen/codegen_timing_winchip.cpp
        codegen/codegen_timing_winchip2.cpp
        )

if(${PCEM_CPU_TYPE} STREQUAL "x86_64")
        set(PCEM_SRC ${PCEM_SRC}
                codegen/x86-64/codegen_backend_x86-64.cpp
                codegen/x86-64/codegen_backend_x86-64_ops.cpp
                codegen/x86-64/codegen_backend_x86-64_ops_sse.cpp
                codegen/x86-64/codegen_backend_x86-64_uops.cpp
                )
endif()

if(${PCEM_CPU_TYPE} STREQUAL "i386")
        set(PCEM_SRC ${PCEM_SRC}
                codegen/x86/codegen_backend_x86.cpp
                codegen/x86/codegen_backend_x86_ops.cpp
                codegen/x86/codegen_backend_x86_ops_fpu.cpp
                codegen/x86/codegen_backend_x86_ops_sse.cpp
                codegen/x86/codegen_backend_x86_uops.cpp
                )
endif()

if(${PCEM_CPU_TYPE} STREQUAL "arm64")
        set(PCEM_SRC ${PCEM_SRC}
                codegen/arm64/codegen_backend_arm64.cpp
                codegen/arm64/codegen_backend_arm64_imm.cpp
                codegen/arm64/codegen_backend_arm64_ops.cpp
                codegen/arm64/codegen_backend_arm64_uops.cpp
                )
elseif(${PCEM_CPU_TYPE} MATCHES "armv.*")
        set(PCEM_SRC ${PCEM_SRC}
                codegen/arm32/codegen_backend_arm.cpp
                codegen/arm32/codegen_backend_arm_ops.cpp
                codegen/arm32/codegen_backend_arm_uops.cpp
                )
endif()
