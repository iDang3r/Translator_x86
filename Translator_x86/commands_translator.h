//
//  comands.h
//  Translator_x86
//
//  Created by Александр on 05.05.2020.
//  Copyright © 2020 Alex. All rights reserved.
//

#ifndef comands_h
#define comands_h

#define command(x)                          \
    memcpy(buff + offset, x(), x##_size);   \
    offset += x##_size                      \

#define command_p(x, y)                      \
    memcpy(buff + offset, x(y), x##_size);   \
    offset += x##_size                       \

#define jump(x, adr)                                             \
    memcpy(buff + offset, x(adr - offset - x##_size), x##_size); \
    offset += x##_size                                           \

#define scan_label_and_check()                       \
    sscanf(p, " %[a-z_0-9]%n", name_of_command, &n); \
    p += n;                                          \
    if (labels[name_of_command] == 0)                \
        second_assembling = true                     \

char* call(uint32_t addr) {     // example of implementation with static struct
    
#pragma pack(push, 1)
    static struct {
        const u_char op = 0xE8; // call addr64
        uint32_t     addres;
    } code;
#pragma pack(pop)
    
    code.addres = addr;
//    *(uint32_t*)(code + 1) = adr;
    
    return (char*)&code;
}
uint32_t call_size = 5;

char* ret() {
    
    static char code[] = {
        0xC3
    };
    
    return code;
}
uint32_t ret_size = 1;

char* push_rax() {
    
    static char code[] = {
        0x50
    };

    return code;
}
uint32_t push_rax_size = 1;

char* push_rbx() {
    
    static char code[] = {
        0x53
    };
    
    return code;
}
uint32_t push_rbx_size = 1;

char* push_rcx() {
    
    static char code[] = {
        0x51
    };
    
    return code;
}
uint32_t push_rcx_size = 1;

char* push_rdx() {
    
    static char code[] = {
        0x52
    };
    
    return code;
}
uint32_t push_rdx_size = 1;

char* push_r9() {
    
    static char code[] = {
        0x41, 0x51
    };
    
    return code;
}
uint32_t push_r9_size = 2;

char* push_r10() {
    
    static char code[] = {
        0x41, 0x52
    };
    
    return code;
}
uint32_t push_r10_size = 2;

char* push_r11() {
    
    static char code[] = {
        0x41, 0x53
    };
    
    return code;
}
uint32_t push_r11_size = 2;

char* push_r12() {
    
    static char code[] = {
        0x41, 0x54
    };
    
    return code;
}
uint32_t push_r12_size = 2;

char* push_rdi() {
    
    static char code[] = {
        0x57
    };
    
    return code;
}
uint32_t push_rdi_size = 1;

char* pop_rdi() {
    
    static char code[] = {
        0x5F
    };
    
    return code;
}
uint32_t pop_rdi_size = 1;

char* push_rsi() {
    
    static char code[] = {
        0x56
    };
    
    return code;
}
uint32_t push_rsi_size = 1;

char* pop_rsi() {
    
    static char code[] = {
        0x5E
    };
    
    return code;
}
uint32_t pop_rsi_size = 1;

char* pop_rax() {
    
    static char code[] = {
        0x58
    };
    
    return code;
}
uint32_t pop_rax_size = 1;

char* pop_rbx() {
    
    static char code[] = {
        0x5B
    };
    
    return code;
}
uint32_t pop_rbx_size = 1;

char* pop_rcx() {
    
    static char code[] = {
        0x59
    };
    
    return code;
}
uint32_t pop_rcx_size = 1;

char* pop_rdx() {
    
    static char code[] = {
        0x5A
    };
    
    return code;
}
uint32_t pop_rdx_size = 1;

char* pop_r9() {
    
    static char code[] = {
        0x41, 0x59
    };
    
    return code;
}
uint32_t pop_r9_size = 2;

char* pop_r10() {
    
    static char code[] = {
        0x41, 0x5A
    };
    
    return code;
}
uint32_t pop_r10_size = 2;

char* pop_r11() {
    
    static char code[] = {
        0x41, 0x5B
    };
    
    return code;
}
uint32_t pop_r11_size = 2;

char* pop_r12() {
    
    static char code[] = {
        0x41, 0x5C
    };
    
    return code;
}
uint32_t pop_r12_size = 2;

char* add_r9_r10() {
    
    static char code[] = {
        0x4D, 0x01, 0xD1
    };
    
    return code;
}
uint32_t add_r9_r10_size = 3;

char* sub_r10_r9() {
    
    static char code[] = {
        0x4D, 0x29, 0xCA
    };
    
    return code;
}
uint32_t sub_r10_r9_size = 3;

char* mov_r15_rax() {
    
    static char code[] = {
        0x49, 0x89, 0xC7
    };
    
    return code;
}
uint32_t mov_r15_rax_size = 3;

char* mov_r14_rbx() {
    
    static char code[] = {
        0x49, 0x89, 0xDE
    };
    
    return code;
}
uint32_t mov_r14_rbx_size = 3;

char* mov_r13_rcx() {
    
    static char code[] = {
        0x49, 0x89, 0xCD
    };
    
    return code;
}
uint32_t mov_r13_rcx_size = 3;

char* mov_r12_rdx() {
    
    static char code[] = {
        0x49, 0x89, 0xD4
    };
    
    return code;
}
uint32_t mov_r12_rdx_size = 3;

char* mov_r11_rsi() {
    
    static char code[] = {
        0x49, 0x89, 0xF3
    };
    
    return code;
}
uint32_t mov_r11_rsi_size = 3;

char* mov_r14_rdx() {
    
    static char code[] = {
        0x49, 0x89, 0xD6
    };
    
    return code;
}
uint32_t mov_r14_rdx_size = 3;

char* mov_rax_r15() {
    
    static char code[] = {
        0x4C, 0x89, 0xF8
    };
    
    return code;
}
uint32_t mov_rax_r15_size = 3;

char* mov_rbx_r14() {
    
    static char code[] = {
        0x4C, 0x89, 0xF3
    };
    
    return code;
}
uint32_t mov_rbx_r14_size = 3;

char* mov_rcx_r13() {
    
    static char code[] = {
        0x4C, 0x89, 0xE9
    };
    
    return code;
}
uint32_t mov_rcx_r13_size = 3;

char* mov_rdx_r12() {
    
    static char code[] = {
        0x4C, 0x89, 0xE2
    };
    
    return code;
}
uint32_t mov_rdx_r12_size = 3;

char* mov_rsi_r11() {
    
    static char code[] = {
        0x4C, 0x89, 0xDE
    };
    
    return code;
}
uint32_t mov_rsi_r11_size = 3;

char* mov_rdx_r14() {
    
    static char code[] = {
        0x4C, 0x89, 0xF2
    };
    
    return code;
}
uint32_t mov_rdx_r14_size = 3;

char* mul_r9() {
    
    static char code[] = {
        0x41, 0xF7, 0xE9
    };
    
    return code;
}
uint32_t mul_r9_size = 3;

char* mov_r9_0x64() {
    
    static char code[] = {
        0x41, 0xB9, 0x64, 0x00, 0x00, 0x00
    };
    
    return code;
}
uint32_t mov_r9_0x64_size = 6;

char* cdq() {
    
    static char code[] = {
        0x99
    };
    
    return code;
}
uint32_t cdq_size = 1;

char* div_r9() {
    
    static char code[] = {
        0x41, 0xF7, 0xF9
    };
    
    return code;
}
uint32_t div_r9_size = 3;

char* div_r10() {
    
    static char code[] = {
        0x41, 0xF7, 0xFA
    };
    
    return code;
}
uint32_t div_r10_size = 3;

char* xor_rdx() {
    
    static char code[] = {
        0x48, 0x31, 0xD2
    };
    
    return code;
}
uint32_t xor_rdx_size = 3;

char* xor_rbx() {
    
    static char code[] = {
        0x48, 0x31, 0xDB
    };
    
    return code;
}
uint32_t xor_rbx_size = 3;

char* bsr_ecx_eax() {
    
    static char code[] = {
        0x0F, 0xBD, 0xC8
    };
    
    return code;
}
uint32_t bsr_ecx_eax_size = 3;

char* and_cl_0xFE() {
    
    static char code[] = {
        0x80, 0xE1, 0xFE
    };
    
    return code;
}
uint32_t and_cl_0xFE_size = 3;

char* mov_edx_0x1() {
    
    static char code[] = {
        0xBA, 0x01, 0x00, 0x00, 0x00
    };
    
    return code;
}
uint32_t mov_edx_0x1_size = 5;

char* shl_edx_cl() {
    
    static char code[] = {
        0xD3, 0xE2
    };
    
    return code;
}
uint32_t shl_edx_cl_size = 2;

char* mov_esi_ebx() {
    
    static char code[] = {
        0x89, 0xDE
    };
    
    return code;
}
uint32_t mov_esi_ebx_size = 2;

char* add_esi_edx() {
    
    static char code[] = {
        0x01, 0xD6
    };
    
    return code;
}
uint32_t add_esi_edx_size = 2;

char* cmp_esi_eax() {
    
    static char code[] = {
        0x39, 0xC6
    };
    
    return code;
}
uint32_t cmp_esi_eax_size = 2;

char* ja_byte(char adr) {
    
    static char code[] = {
        0x77, 0x00
    };
    code[1] = adr;
    
    return code;
}
uint32_t ja_byte_size = 2;

char* sub_eax_esi() {
    
    static char code[] = {
        0x29, 0xF0
    };
    
    return code;
}
uint32_t sub_eax_esi_size = 2;

char* shr_ebx() {
    
    static char code[] = {
        0xD1, 0xEB
    };
    
    return code;
}
uint32_t shr_ebx_size = 2;

char* add_ebx_edx() {
    
    static char code[] = {
        0x01, 0xD3
    };
    
    return code;
}
uint32_t add_ebx_edx_size = 2;

char* jmp_byte(char adr) {
    
    static char code[] = {
        0xEB, 0x00
    };
    code[1] = adr;
    
    return code;
}
uint32_t jmp_byte_size = 2;

char* shr_edx_0x2() {
    
    static char code[] = {
        0xC1, 0xEA, 0x02
    };
    
    return code;
}
uint32_t shr_edx_0x2_size = 3;

char* jne_byte(char adr) {
    
    static char code[] = {
        0x75, 0x00
    };
    code[1] = adr;
    
    return code;
}
uint32_t jne_byte_size = 2;

char* push_int(uint32_t val) {

    static char code[] = {
        0x68, 0x67, 0x45, 0x23, 0x01
    };

    *(uint32_t*)(code + 1) = val;
    return code;
}
uint32_t push_int_size = 5;

char* inc_ax() {
    
    static char code[] = {
        0x48, 0xFF, 0xC0
    };
    
    return code;
}
uint32_t inc_ax_size = 3;

char* inc_bx() {
    
    static char code[] = {
        0x48, 0xFF, 0xC3
    };
    
    return code;
}
uint32_t inc_bx_size = 3;

char* inc_cx() {
    
    static char code[] = {
        0x48, 0xFF, 0xC1
    };
    
    return code;
}
uint32_t inc_cx_size = 3;

char* inc_dx() {
    
    static char code[] = {
        0x48, 0xFF, 0xC2
    };
    
    return code;
}
uint32_t inc_dx_size = 3;

char* jmp(uint32_t adr) {
    
    static char code[] = {
        0xE9, 0xBD, 0x00, 0x00, 0x00
    };
    
    *(uint32_t*)(code + 1) = adr;
    
    return code;
}
uint32_t jmp_size = 5;

char* ja(uint32_t adr) {
    
    static char code[] = {
        0x41, 0x5A,
        0x41, 0x59,
        0x45, 0x39, 0xD1,
        0x0F, 0x8F, 0xBF, 0x01, 0x00, 0x00
    };
    
    *(uint32_t*)(code + 9) = adr;
    
    return code;
}
uint32_t ja_size = 13;

char* jae(uint32_t adr) {
    
    static char code[] = {
        0x41, 0x5A,
        0x41, 0x59,
        0x45, 0x39, 0xD1,
        0x0F, 0x8D, 0xBF, 0x01, 0x00, 0x00
    };
    
    *(uint32_t*)(code + 9) = adr;
    
    return code;
}
uint32_t jae_size = 13;

char* jb(uint32_t adr) {
    
    static char code[] = {
        0x41, 0x5A,
        0x41, 0x59,
        0x45, 0x39, 0xD1,
        0x0F, 0x8C, 0xBF, 0x01, 0x00, 0x00
    };
    
    *(uint32_t*)(code + 9) = adr;
    
    return code;
}
uint32_t jb_size = 13;

char* jbe(uint32_t adr) {
    
    static char code[] = {
        0x41, 0x5A,
        0x41, 0x59,
        0x45, 0x39, 0xD1,
        0x0F, 0x8E, 0xBF, 0x01, 0x00, 0x00
    };
    
    *(uint32_t*)(code + 9) = adr;
    
    return code;
}
uint32_t jbe_size = 13;

char* je(uint32_t adr) {
    
    static char code[] = {
        0x41, 0x5A,
        0x41, 0x59,
        0x45, 0x39, 0xD1,
        0x0F, 0x84, 0xBF, 0x01, 0x00, 0x00
    };
    
    *(uint32_t*)(code + 9) = adr;
    
    return code;
}
uint32_t je_size = 13;

char* jne(uint32_t adr) {
    
    static char code[] = {
        0x41, 0x5A,
        0x41, 0x59,
        0x45, 0x39, 0xD1,
        0x0F, 0x85, 0xBF, 0x01, 0x00, 0x00
    };
    
    *(uint32_t*)(code + 9) = adr;
    
    return code;
}
uint32_t jne_size = 13;

char* save_ret() {
    
    static char code[] = {
        0x48, 0x89, 0x3E,
        0x48, 0x83, 0xC6, 0x08
    };
    
    return code;
}
uint32_t save_ret_size = 7;

char* rest_ret() {
    
    static char code[] = {
        0x48, 0x83, 0xEE, 0x08,
        0x48, 0x8B, 0x3E
    };
    
    return code;
}
uint32_t rest_ret_size = 7;

char* program_end() {
    
    static char code[] = {
        0xB8, 0x01, 0x00, 0x00, 0x02,
        0xBF, 0x00, 0x00, 0x00, 0x00,
        0x0F, 0x05
    };
    
    return code;
}
uint32_t program_end_size = 12;

char* mov_r8_memptr() {
    
    static char code[] = {
        0x49, 0xB8, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
        0x4C, 0x89, 0xC6,
        0x48, 0x81, 0xC6, 0xD8, 0x00, 0x00, 0x00
    };
    
    return code;
}
uint32_t mov_r8_memptr_size = 20;

char* mov_mem_from_r9(uint32_t adr) {
    
    static char code[] = {
        0x45, 0x89, 0x88, 0x00, 0x00, 0x00, 0x00
    };
    
    *(uint32_t*)(code + 3) = adr * 4;
    return code;
}
uint32_t mov_mem_from_r9_size = 7;

char* mov_r9_from_mem(uint32_t adr) {
    
    static char code[] = {
        0x45, 0x8B, 0x88, 0x39, 0x30, 0x00, 0x00
    };
    
    *(uint32_t*)(code + 3) = adr * 4;
    return code;
}
uint32_t mov_r9_from_mem_size = 7;

char* mov_r9_from_mem_add_ax(u_char adr) {
    
    static char code[] = {
        0x49, 0x89, 0xD5,
        0x49, 0x89, 0xC4,
        0x41, 0xBB, 0x19, 0x00, 0x00, 0x00,
        0x99,
        0x41, 0xF7, 0xFB,
        0x4D, 0x89, 0xC2,
        0x49, 0x01, 0xC2,
        0x49, 0x81, 0xC2, 0x45, 0x23, 0x01, 0x00,
        0x45, 0x8B, 0x0A,
        0x4C, 0x89, 0xEA,
        0x4C, 0x89, 0xE0
    };
    
    *(uint32_t*)(code + 25) = adr * 4;
    return code;
}
uint32_t mov_r9_from_mem_add_ax_size = 38;

char* mov_mem_add_ax_from_r9(u_char adr) {
    
    static char code[] = {
        0x49, 0x89, 0xD5,
        0x49, 0x89, 0xC4,
        0x41, 0xBB, 0x19, 0x00, 0x00, 0x00,
        0x99,
        0x41, 0xF7, 0xFB,
        0x4D, 0x89, 0xC2,
        0x49, 0x01, 0xC2,
        0x49, 0x81, 0xC2, 0x0E, 0x00, 0x00, 0x00,
        0x45, 0x89, 0x0A,
        0x4C, 0x89, 0xEA,
        0x4C, 0x89, 0xE0
    };
    
    *(uint32_t*)(code + 25) = adr * 4;
    return code;
}
uint32_t mov_mem_add_ax_from_r9_size = 38;

char* scan_lib() {
    
    static char code[] = {
        
        0x57, 0x56, 0x49, 0x89, 0xC7, 0x49, 0x89, 0xDE, 0x49, 0x89, 0xCD, 0x49, 0x89, 0xD4, 0xB8, 0x03, 0x00, 0x00, 0x02, 0xBF, 0x02, 0x00, 0x00, 0x00, 0x48, 0xBE, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xBA, 0x08, 0x00, 0x00, 0x00, 0x0F, 0x05, 0x48, 0xBE, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x48, 0x31, 0xC0, 0x41, 0xB9, 0x0A, 0x00, 0x00, 0x00, 0x41, 0xBB, 0x01, 0x00, 0x00, 0x00, 0x8A, 0x06, 0x3C, 0x2D, 0x75, 0x09, 0x48, 0xFF, 0xC6, 0x41, 0xBB, 0xFF, 0xFF, 0xFF, 0xFF, 0xAC, 0x3C, 0x0A, 0x74, 0x15, 0x50, 0x48, 0x89, 0xD0, 0x48, 0x31, 0xD2, 0x41, 0xF7, 0xE9, 0x48, 0x89, 0xC2, 0x58, 0x2C, 0x30, 0x48, 0x01, 0xC2, 0xEB, 0xE6, 0x41, 0x83, 0xFB, 0xFF, 0x75, 0x11, 0x48, 0x89, 0xD0, 0x41, 0xB9, 0x9C, 0xFF, 0xFF, 0xFF, 0x41, 0xF7, 0xE9, 0x49, 0x89, 0xC1, 0xEB, 0x0F, 0x48, 0x89, 0xD0, 0x41, 0xB9, 0x64, 0x00, 0x00, 0x00, 0x41, 0xF7, 0xE9, 0x49, 0x89, 0xC1, 0x4C, 0x89, 0xF8, 0x4C, 0x89, 0xF3, 0x4C, 0x89, 0xE9, 0x4C, 0x89, 0xE2, 0x5E, 0x5F, 0xC3
        
    };
    
    return code;
}
uint32_t scan_lib_size = 160;

char* print_lib() {
    
    static char code[] = {
        
        0x57, 0x56, 0x49, 0x89, 0xC7, 0x49, 0x89, 0xDE, 0x49, 0x89, 0xCB, 0x49, 0x89, 0xD4, 0x4C, 0x89, 0xC8, 0x41, 0xB9, 0x01, 0x00, 0x00, 0x00, 0x41, 0xC1, 0xE1, 0x1F, 0x41, 0x21, 0xC1, 0x41, 0x83, 0xF9, 0x00, 0x74, 0x16, 0x50, 0xB0, 0x2D, 0x48, 0xBF, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xAA, 0xE8, 0xD3, 0x00, 0x00, 0x00, 0x58, 0xF7, 0xD8, 0x49, 0x89, 0xC2, 0x4D, 0x31, 0xC9, 0x48, 0x31, 0xDB, 0x41, 0xBB, 0x0A, 0x00, 0x00, 0x00, 0x4C, 0x89, 0xD0, 0x48, 0x31, 0xD2, 0x41, 0xF7, 0xFB, 0x49, 0xC1, 0xE1, 0x04, 0x41, 0x01, 0xD1, 0x49, 0x89, 0xC2, 0x48, 0xFF, 0xC3, 0x49, 0x83, 0xFA, 0x00, 0x77, 0xE4, 0x48, 0x83, 0xFB, 0x02, 0x77, 0x3E, 0xB0, 0x30, 0x48, 0xBF, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xAA, 0xE8, 0x8D, 0x00, 0x00, 0x00, 0xB0, 0x2E, 0x48, 0xBF, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xAA, 0xE8, 0x7B, 0x00, 0x00, 0x00, 0x48, 0x83, 0xFB, 0x01, 0x74, 0x02, 0xEB, 0x12, 0xB0, 0x30, 0x48, 0xBF, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xAA, 0xE8, 0x61, 0x00, 0x00, 0x00, 0x4C, 0x89, 0xC8, 0x48, 0x83, 0xE0, 0x0F, 0x48, 0x83, 0xC0, 0x30, 0x48, 0xBF, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xAA, 0xE8, 0x46, 0x00, 0x00, 0x00, 0x49, 0xC1, 0xE9, 0x04, 0x48, 0xFF, 0xCB, 0x48, 0x83, 0xFB, 0x02, 0x75, 0x12, 0xB0, 0x2E, 0x48, 0xBF, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xAA, 0xE8, 0x27, 0x00, 0x00, 0x00, 0x48, 0x83, 0xFB, 0x00, 0x75, 0xC0, 0xB0, 0x0A, 0x48, 0xBF, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xAA, 0xE8, 0x0F, 0x00, 0x00, 0x00, 0x4C, 0x89, 0xF8, 0x4C, 0x89, 0xF3, 0x4C, 0x89, 0xE9, 0x4C, 0x89, 0xE2, 0x5E, 0x5F, 0xC3, 0x56, 0xB8, 0x04, 0x00, 0x00, 0x02, 0xBF, 0x01, 0x00, 0x00, 0x00, 0x48, 0xBE, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xBA, 0x01, 0x00, 0x00, 0x00, 0x0F, 0x05, 0x5E, 0xC3
        
    };
    
    return code;
}
uint32_t print_lib_size = 296;

#endif /* comands_h */
