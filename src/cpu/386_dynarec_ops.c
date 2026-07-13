#include "ibm.h"
#include "cpu.h"
#include "x86.h"
#include "x86_ops.h"
#include "x87.h"
#include "x86_flags.h"
#include "mem.h"
#include "nmi.h"
#include "pic.h"
#include "codegen.h"

#define CPU_BLOCK_END() cpu_block_end = 1

#include "386_common.h"

static inline void fetch_ea_32_long(uint32_t rmdat) {
    easeg = cpu_state.ea_seg->base;

    uint32_t addr_full = easeg + cpu_state.eaaddr;
    int outer = (easeg != 0xFFFFFFFF) & ((addr_full & 0xFFF) <= 0xFFC);

    /* Force addr to 0 when the outer condition is false – safe index */
    uint32_t addr = addr_full & -(uint32_t)outer;
    uint32_t page = addr >> 12;

    int has_read = (readlookup2[page] != -1) & outer;
    int has_write = (writelookup2[page] != -1) & outer;

    uintptr_t rptr = (uintptr_t)(readlookup2[page] + addr);
    uintptr_t wptr = (uintptr_t)(writelookup2[page] + addr);

    eal_r = (uint32_t *)(rptr & -(uintptr_t)has_read);
    eal_w = (uint32_t *)(wptr & -(uintptr_t)has_write);
}

static inline void fetch_ea_16_long(uint32_t rmdat) {
    easeg = cpu_state.ea_seg->base;

    uint32_t addr_full = easeg + cpu_state.eaaddr;
    int outer = (easeg != 0xFFFFFFFF) & ((addr_full & 0xFFF) <= 0xFFC);

    /* Force addr to 0 when the outer condition is false – safe index */
    uint32_t addr = addr_full & -(uint32_t)outer;
    uint32_t page = addr >> 12;

    int has_read = (readlookup2[page] != -1) & outer;
    int has_write = (writelookup2[page] != -1) & outer;

    uintptr_t rptr = (uintptr_t)(readlookup2[page] + addr);
    uintptr_t wptr = (uintptr_t)(writelookup2[page] + addr);

    eal_r = (uint32_t *)(rptr & -(uintptr_t)has_read);
    eal_w = (uint32_t *)(wptr & -(uintptr_t)has_write);
}


#define fetch_ea_16(rmdat)                                                                                                       \
        cpu_state.pc++;                                                                                                          \
        if (cpu_mod != 3)                                                                                                        \
                fetch_ea_16_long(rmdat);
#define fetch_ea_32(rmdat)                                                                                                       \
        cpu_state.pc++;                                                                                                          \
        if (cpu_mod != 3)                                                                                                        \
                fetch_ea_32_long(rmdat);

#define PREFETCH_RUN(instr_cycles, bytes, modrm, reads, read_ls, writes, write_ls, ea32)
#define PREFETCH_PREFIX()
#define PREFETCH_FLUSH()

#define OP_TABLE(name) dynarec_ops_##name
/*Temporary*/
#define CLOCK_CYCLES(c)
#define CLOCK_CYCLES_ALWAYS(c) cycles -= (c)

#include "386_ops.h"
