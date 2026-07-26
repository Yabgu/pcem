#ifndef _X86_OPS_CALL_H_
#define _X86_OPS_CALL_H_
#define CALL_FAR_w(new_seg, new_pc)                                                                                              \
        old_cs = CS;                                                                                                             \
        old_pc = cpu_state.pc;                                                                                                   \
        cpu_state.pc = new_pc;                                                                                                   \
        optype = CALL;                                                                                                           \
        cgate16 = cgate32 = 0;                                                                                                   \
        if (msw & 1)                                                                                                             \
                loadcscall(new_seg, old_pc);                                                                                     \
        else {                                                                                                                   \
                loadcs(new_seg);                                                                                                 \
                cycles -= timing_call_rm;                                                                                        \
        }                                                                                                                        \
        optype = 0;                                                                                                              \
        if (unlikely(cpu_state.abrt)) {                                                                                                    \
                cgate16 = cgate32 = 0;                                                                                           \
                return 1;                                                                                                        \
        }                                                                                                                        \
        oldss = ss;                                                                                                              \
        if (cgate32) {                                                                                                           \
                uint32_t old_esp = ESP;                                                                                          \
                PUSH_L(old_cs);                                                                                                  \
                if (unlikely(cpu_state.abrt)) {                                                                                            \
                        CS = old_cs;                                                                                             \
                        cgate16 = cgate32 = 0;                                                                                   \
                        return 1;                                                                                                \
                }                                                                                                                \
                PUSH_L(old_pc);                                                                                                  \
                if (unlikely(cpu_state.abrt)) {                                                                                            \
                        CS = old_cs;                                                                                             \
                        ESP = old_esp;                                                                                           \
                        return 1;                                                                                                \
                }                                                                                                                \
        } else {                                                                                                                 \
                uint32_t old_esp = ESP;                                                                                          \
                PUSH_W(old_cs);                                                                                                  \
                if (unlikely(cpu_state.abrt)) {                                                                                            \
                        CS = old_cs;                                                                                             \
                        cgate16 = cgate32 = 0;                                                                                   \
                        return 1;                                                                                                \
                }                                                                                                                \
                PUSH_W(old_pc);                                                                                                  \
                if (unlikely(cpu_state.abrt)) {                                                                                            \
                        CS = old_cs;                                                                                             \
                        ESP = old_esp;                                                                                           \
                        return 1;                                                                                                \
                }                                                                                                                \
        }

#define CALL_FAR_l(new_seg, new_pc)                                                                                              \
        old_cs = CS;                                                                                                             \
        old_pc = cpu_state.pc;                                                                                                   \
        cpu_state.pc = new_pc;                                                                                                   \
        optype = CALL;                                                                                                           \
        cgate16 = cgate32 = 0;                                                                                                   \
        if (msw & 1)                                                                                                             \
                loadcscall(new_seg, old_pc);                                                                                     \
        else {                                                                                                                   \
                loadcs(new_seg);                                                                                                 \
                cycles -= timing_call_rm;                                                                                        \
        }                                                                                                                        \
        optype = 0;                                                                                                              \
        if (unlikely(cpu_state.abrt)) {                                                                                                    \
                cgate16 = cgate32 = 0;                                                                                           \
                return 1;                                                                                                        \
        }                                                                                                                        \
        oldss = ss;                                                                                                              \
        if (cgate16) {                                                                                                           \
                uint32_t old_esp = ESP;                                                                                          \
                PUSH_W(old_cs);                                                                                                  \
                if (unlikely(cpu_state.abrt)) {                                                                                            \
                        CS = old_cs;                                                                                             \
                        cgate16 = cgate32 = 0;                                                                                   \
                        return 1;                                                                                                \
                }                                                                                                                \
                PUSH_W(old_pc);                                                                                                  \
                if (unlikely(cpu_state.abrt)) {                                                                                            \
                        CS = old_cs;                                                                                             \
                        ESP = old_esp;                                                                                           \
                        return 1;                                                                                                \
                }                                                                                                                \
        } else {                                                                                                                 \
                uint32_t old_esp = ESP;                                                                                          \
                PUSH_L(old_cs);                                                                                                  \
                if (unlikely(cpu_state.abrt)) {                                                                                            \
                        CS = old_cs;                                                                                             \
                        cgate16 = cgate32 = 0;                                                                                   \
                        return 1;                                                                                                \
                }                                                                                                                \
                PUSH_L(old_pc);                                                                                                  \
                if (unlikely(cpu_state.abrt)) {                                                                                            \
                        CS = old_cs;                                                                                             \
                        ESP = old_esp;                                                                                           \
                        return 1;                                                                                                \
                }                                                                                                                \
        }

static int opCALL_far_w(uint32_t fetchdat) {
        uint32_t old_cs, old_pc;
        uint16_t new_cs, new_pc;
        int cycles_old = cycles;
        UNUSED(cycles_old);

        new_pc = getwordf();
        new_cs = getword();
        if (unlikely(cpu_state.abrt))
                return 1;

        CALL_FAR_w(new_cs, new_pc);
        CPU_BLOCK_END();

        return 0;
}
static int opCALL_far_l(uint32_t fetchdat) {
        uint32_t old_cs, old_pc;
        uint32_t new_cs, new_pc;
        int cycles_old = cycles;
        UNUSED(cycles_old);

        new_pc = getlong();
        new_cs = getword();
        if (unlikely(cpu_state.abrt))
                return 1;

        CALL_FAR_l(new_cs, new_pc);
        CPU_BLOCK_END();

        return 0;
}

static int opFF_w_a16(uint32_t fetchdat) {
        uint16_t old_cs, new_cs;
        uint32_t old_pc, new_pc;
        int cycles_old = cycles;
        UNUSED(cycles_old);

        uint16_t temp;

        fetch_ea_16(fetchdat);

        switch (rmdat & 0x38) {
        case 0x00: /*INC w*/
                if (cpu_mod != 3)
                        SEG_CHECK_WRITE(cpu_state.ea_seg);
                temp = geteaw();
                if (unlikely(cpu_state.abrt))
                        return 1;
                seteaw(temp + 1);
                if (unlikely(cpu_state.abrt))
                        return 1;
                setadd16nc(temp, 1);
                CLOCK_CYCLES((cpu_mod == 3) ? timing_rr : timing_mm);
                break;
        case 0x08: /*DEC w*/
                if (cpu_mod != 3)
                        SEG_CHECK_WRITE(cpu_state.ea_seg);
                temp = geteaw();
                if (unlikely(cpu_state.abrt))
                        return 1;
                seteaw(temp - 1);
                if (unlikely(cpu_state.abrt))
                        return 1;
                setsub16nc(temp, 1);
                CLOCK_CYCLES((cpu_mod == 3) ? timing_rr : timing_mm);
                break;
        case 0x10: /*CALL*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                new_pc = geteaw();
                if (unlikely(cpu_state.abrt))
                        return 1;
                PUSH_W(cpu_state.pc);
                cpu_state.pc = new_pc;
                CPU_BLOCK_END();
                if (is486)
                        CLOCK_CYCLES(5);
                else
                        CLOCK_CYCLES((cpu_mod == 3) ? 7 : 10);
                break;
        case 0x18: /*CALL far*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                new_pc = readmemw(easeg, cpu_state.eaaddr);
                new_cs = readmemw(easeg, (cpu_state.eaaddr + 2));
                if (unlikely(cpu_state.abrt))
                        return 1;

                CALL_FAR_w(new_cs, new_pc);
                CPU_BLOCK_END();
                break;
        case 0x20: /*JMP*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                new_pc = geteaw();
                if (unlikely(cpu_state.abrt))
                        return 1;
                cpu_state.pc = new_pc;
                CPU_BLOCK_END();
                if (is486)
                        CLOCK_CYCLES(5);
                else
                        CLOCK_CYCLES((cpu_mod == 3) ? 7 : 10);
                break;
        case 0x28: /*JMP far*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                old_pc = cpu_state.pc;
                new_pc = readmemw(easeg, cpu_state.eaaddr);
                new_cs = readmemw(easeg, cpu_state.eaaddr + 2);
                if (unlikely(cpu_state.abrt))
                        return 1;
                cpu_state.pc = new_pc;
                loadcsjmp(new_cs, old_pc);
                if (unlikely(cpu_state.abrt))
                        return 1;
                CPU_BLOCK_END();
                break;
        case 0x30: /*PUSH w*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                temp = geteaw();
                if (unlikely(cpu_state.abrt))
                        return 1;
                PUSH_W(temp);
                CLOCK_CYCLES((cpu_mod == 3) ? 2 : 5);
                break;

        default:
                //                fatal("Bad FF opcode %02X\n",rmdat&0x38);
                x86illegal();
        }
        return cpu_state.abrt;
}
static int opFF_w_a32(uint32_t fetchdat) {
        uint16_t old_cs, new_cs;
        uint32_t old_pc, new_pc;
        int cycles_old = cycles;
        UNUSED(cycles_old);

        uint16_t temp;

        fetch_ea_32(fetchdat);

        switch (rmdat & 0x38) {
        case 0x00: /*INC w*/
                if (cpu_mod != 3)
                        SEG_CHECK_WRITE(cpu_state.ea_seg);
                temp = geteaw();
                if (unlikely(cpu_state.abrt))
                        return 1;
                seteaw(temp + 1);
                if (unlikely(cpu_state.abrt))
                        return 1;
                setadd16nc(temp, 1);
                CLOCK_CYCLES((cpu_mod == 3) ? timing_rr : timing_mm);
                break;
        case 0x08: /*DEC w*/
                if (cpu_mod != 3)
                        SEG_CHECK_WRITE(cpu_state.ea_seg);
                temp = geteaw();
                if (unlikely(cpu_state.abrt))
                        return 1;
                seteaw(temp - 1);
                if (unlikely(cpu_state.abrt))
                        return 1;
                setsub16nc(temp, 1);
                CLOCK_CYCLES((cpu_mod == 3) ? timing_rr : timing_mm);
                break;
        case 0x10: /*CALL*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                new_pc = geteaw();
                if (unlikely(cpu_state.abrt))
                        return 1;
                PUSH_W(cpu_state.pc);
                cpu_state.pc = new_pc;
                CPU_BLOCK_END();
                if (is486)
                        CLOCK_CYCLES(5);
                else
                        CLOCK_CYCLES((cpu_mod == 3) ? 7 : 10);
                break;
        case 0x18: /*CALL far*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                new_pc = readmemw(easeg, cpu_state.eaaddr);
                new_cs = readmemw(easeg, (cpu_state.eaaddr + 2));
                if (unlikely(cpu_state.abrt))
                        return 1;

                CALL_FAR_w(new_cs, new_pc);
                CPU_BLOCK_END();
                break;
        case 0x20: /*JMP*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                new_pc = geteaw();
                if (unlikely(cpu_state.abrt))
                        return 1;
                cpu_state.pc = new_pc;
                CPU_BLOCK_END();
                if (is486)
                        CLOCK_CYCLES(5);
                else
                        CLOCK_CYCLES((cpu_mod == 3) ? 7 : 10);
                break;
        case 0x28: /*JMP far*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                old_pc = cpu_state.pc;
                new_pc = readmemw(easeg, cpu_state.eaaddr);
                new_cs = readmemw(easeg, cpu_state.eaaddr + 2);
                if (unlikely(cpu_state.abrt))
                        return 1;
                cpu_state.pc = new_pc;
                loadcsjmp(new_cs, old_pc);
                if (unlikely(cpu_state.abrt))
                        return 1;
                CPU_BLOCK_END();
                break;
        case 0x30: /*PUSH w*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                temp = geteaw();
                if (unlikely(cpu_state.abrt))
                        return 1;
                PUSH_W(temp);
                CLOCK_CYCLES((cpu_mod == 3) ? 2 : 5);
                break;

        default:
                //                fatal("Bad FF opcode %02X\n",rmdat&0x38);
                x86illegal();
        }
        return cpu_state.abrt;
}

static int opFF_l_a16(uint32_t fetchdat) {
        uint16_t old_cs, new_cs;
        uint32_t old_pc, new_pc;
        int cycles_old = cycles;
        UNUSED(cycles_old);

        uint32_t temp;

        fetch_ea_16(fetchdat);

        switch (rmdat & 0x38) {
        case 0x00: /*INC l*/
                if (cpu_mod != 3)
                        SEG_CHECK_WRITE(cpu_state.ea_seg);
                temp = geteal();
                if (unlikely(cpu_state.abrt))
                        return 1;
                seteal(temp + 1);
                if (unlikely(cpu_state.abrt))
                        return 1;
                setadd32nc(temp, 1);
                CLOCK_CYCLES((cpu_mod == 3) ? timing_rr : timing_mm);
                break;
        case 0x08: /*DEC l*/
                if (cpu_mod != 3)
                        SEG_CHECK_WRITE(cpu_state.ea_seg);
                temp = geteal();
                if (unlikely(cpu_state.abrt))
                        return 1;
                seteal(temp - 1);
                if (unlikely(cpu_state.abrt))
                        return 1;
                setsub32nc(temp, 1);
                CLOCK_CYCLES((cpu_mod == 3) ? timing_rr : timing_mm);
                break;
        case 0x10: /*CALL*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                new_pc = geteal();
                if (unlikely(cpu_state.abrt))
                        return 1;
                PUSH_L(cpu_state.pc);
                cpu_state.pc = new_pc;
                CPU_BLOCK_END();
                if (is486)
                        CLOCK_CYCLES(5);
                else
                        CLOCK_CYCLES((cpu_mod == 3) ? 7 : 10);
                break;
        case 0x18: /*CALL far*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                new_pc = readmeml(easeg, cpu_state.eaaddr);
                new_cs = readmemw(easeg, (cpu_state.eaaddr + 4));
                if (unlikely(cpu_state.abrt))
                        return 1;

                CALL_FAR_l(new_cs, new_pc);
                CPU_BLOCK_END();
                break;
        case 0x20: /*JMP*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                new_pc = geteal();
                if (unlikely(cpu_state.abrt))
                        return 1;
                cpu_state.pc = new_pc;
                CPU_BLOCK_END();
                if (is486)
                        CLOCK_CYCLES(5);
                else
                        CLOCK_CYCLES((cpu_mod == 3) ? 7 : 10);
                break;
        case 0x28: /*JMP far*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                old_pc = cpu_state.pc;
                new_pc = readmeml(easeg, cpu_state.eaaddr);
                new_cs = readmemw(easeg, cpu_state.eaaddr + 4);
                if (unlikely(cpu_state.abrt))
                        return 1;
                cpu_state.pc = new_pc;
                loadcsjmp(new_cs, old_pc);
                if (unlikely(cpu_state.abrt))
                        return 1;
                CPU_BLOCK_END();
                break;
        case 0x30: /*PUSH l*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                temp = geteal();
                if (unlikely(cpu_state.abrt))
                        return 1;
                PUSH_L(temp);
                CLOCK_CYCLES((cpu_mod == 3) ? 2 : 5);
                break;

        default:
                //                fatal("Bad FF opcode %02X\n",rmdat&0x38);
                x86illegal();
        }
        return cpu_state.abrt;
}
static int opFF_l_a32(uint32_t fetchdat) {
        uint16_t old_cs, new_cs;
        uint32_t old_pc, new_pc;
        int cycles_old = cycles;
        UNUSED(cycles_old);

        uint32_t temp;

        fetch_ea_32(fetchdat);

        switch (rmdat & 0x38) {
        case 0x00: /*INC l*/
                if (cpu_mod != 3)
                        SEG_CHECK_WRITE(cpu_state.ea_seg);
                temp = geteal();
                if (unlikely(cpu_state.abrt))
                        return 1;
                seteal(temp + 1);
                if (unlikely(cpu_state.abrt))
                        return 1;
                setadd32nc(temp, 1);
                CLOCK_CYCLES((cpu_mod == 3) ? timing_rr : timing_mm);
                break;
        case 0x08: /*DEC l*/
                if (cpu_mod != 3)
                        SEG_CHECK_WRITE(cpu_state.ea_seg);
                temp = geteal();
                if (unlikely(cpu_state.abrt))
                        return 1;
                seteal(temp - 1);
                if (unlikely(cpu_state.abrt))
                        return 1;
                setsub32nc(temp, 1);
                CLOCK_CYCLES((cpu_mod == 3) ? timing_rr : timing_mm);
                break;
        case 0x10: /*CALL*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                new_pc = geteal();
                if (unlikely(cpu_state.abrt))
                        return 1;
                PUSH_L(cpu_state.pc);
                if (unlikely(cpu_state.abrt))
                        return 1;
                cpu_state.pc = new_pc;
                CPU_BLOCK_END();
                if (is486)
                        CLOCK_CYCLES(5);
                else
                        CLOCK_CYCLES((cpu_mod == 3) ? 7 : 10);
                break;
        case 0x18: /*CALL far*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                new_pc = readmeml(easeg, cpu_state.eaaddr);
                new_cs = readmemw(easeg, (cpu_state.eaaddr + 4));
                if (unlikely(cpu_state.abrt))
                        return 1;

                CALL_FAR_l(new_cs, new_pc);
                CPU_BLOCK_END();
                break;
        case 0x20: /*JMP*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                new_pc = geteal();
                if (unlikely(cpu_state.abrt))
                        return 1;
                cpu_state.pc = new_pc;
                CPU_BLOCK_END();
                if (is486)
                        CLOCK_CYCLES(5);
                else
                        CLOCK_CYCLES((cpu_mod == 3) ? 7 : 10);
                break;
        case 0x28: /*JMP far*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                old_pc = cpu_state.pc;
                new_pc = readmeml(easeg, cpu_state.eaaddr);
                new_cs = readmemw(easeg, cpu_state.eaaddr + 4);
                if (unlikely(cpu_state.abrt))
                        return 1;
                cpu_state.pc = new_pc;
                loadcsjmp(new_cs, old_pc);
                if (unlikely(cpu_state.abrt))
                        return 1;
                CPU_BLOCK_END();
                break;
        case 0x30: /*PUSH l*/
                if (cpu_mod != 3)
                        SEG_CHECK_READ(cpu_state.ea_seg);
                temp = geteal();
                if (unlikely(cpu_state.abrt))
                        return 1;
                PUSH_L(temp);
                break;

        default:
                //                fatal("Bad FF opcode %02X\n",rmdat&0x38);
                x86illegal();
        }
        return cpu_state.abrt;
}

#endif /* _X86_OPS_CALL_H_ */
