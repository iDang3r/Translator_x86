//
//  comands.h
//  Translator_x86
//
//  Created by Александр on 05.05.2020.
//  Copyright © 2020 Alex. All rights reserved.
//

#ifndef comands_h
#define comands_h

char* program_end() {
    
    static char code[] = {
        0xB8, 0x01, 0x00, 0x00, 0x02,
        0xBF, 0x00, 0x00, 0x00, 0x00,
        0x0F, 0x05
    };
    
    return code;
}
int program_end_size = 12;

char* mov_r8_memptr() {
    
    static char code[] = {
        0x49, 0xB8, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
    };
    
    return code;
}
int mov_r8_memptr_size = 10;

char* scan(uint32_t adr) {
    
    static char code[] = {
        0xE8, 0x29, 0x00, 0x00, 0x00,
        0x41, 0x51
    };
    
    *(uint32_t*)(code + 1) = adr + 2;
    
    return code;
}
int scan_size = 7;

char* print(uint32_t adr) {
    
    static char code[] = {
        0x41, 0x59,
        0xE8, 0x9F, 0x00, 0x00, 0x00
    };
    
    *(uint32_t*)(code + 3) = adr;
    
    return code;
}
int print_size = 7;

char* call(uint32_t adr) {
    
    static char code[] = {
        0xE8, 0x29, 0x00, 0x00, 0x00
    };
    
    *(uint32_t*)(code + 1) = adr;
    
    return code;
}
int call_size = 5;

char* ret() {
    
    static char code[] = {
        0xC3
    };
    
    return code;
}
int ret_size = 1;

char* mov_mem_from_r9(u_char adr) {
    
    static char code[] = {
        0x4D, 0x89, 0x48, 0x00
    };
    code[3] = adr * 4;
    
    return code;
}
int mov_mem_r8_from_r9_size = 4;

char* mov_r9_from_mem(u_char adr) {
    
    static char code[] = {
        0x45, 0x8B, 0x48, 0x00
    };
    code[3] = adr * 4;
    
    return code;
}
int mov_r9_from_mem_size = 4;

char* push_rax() {
    
    static char code[] = {
        0x50
    };

    return code;
}
int push_rax_size = 1;

char* push_rbx() {
    
    static char code[] = {
        0x53
    };
    
    return code;
}
int push_rbx_size = 1;

char* push_rcx() {
    
    static char code[] = {
        0x51
    };
    
    return code;
}
int push_rcx_size = 1;

char* push_rdx() {
    
    static char code[] = {
        0x52
    };
    
    return code;
}
int push_rdx_size = 1;

char* push_r9() {
    
    static char code[] = {
        0x41, 0x51
    };
    
    return code;
}
int push_r9_size = 2;

char* push_r10() {
    
    static char code[] = {
        0x41, 0x52
    };
    
    return code;
}
int push_r10_size = 2;

char* push_r11() {
    
    static char code[] = {
        0x41, 0x53
    };
    
    return code;
}
int push_r11_size = 2;

char* push_r12() {
    
    static char code[] = {
        0x41, 0x54
    };
    
    return code;
}
int push_r12_size = 2;

char* push_rdi() {
    
    static char code[] = {
        0x57
    };
    
    return code;
}
int push_rdi_size = 1;

char* pop_rdi() {
    
    static char code[] = {
        0x5F
    };
    
    return code;
}
int pop_rdi_size = 1;

char* pop_rax() {
    
    static char code[] = {
        0x58
    };
    
    return code;
}
int pop_rax_size = 1;

char* pop_rbx() {
    
    static char code[] = {
        0x5B
    };
    
    return code;
}
int pop_rbx_size = 1;

char* pop_rcx() {
    
    static char code[] = {
        0x59
    };
    
    return code;
}
int pop_rcx_size = 1;

char* pop_rdx() {
    
    static char code[] = {
        0x5A
    };
    
    return code;
}
int pop_rdx_size = 1;

char* pop_r9() {
    
    static char code[] = {
        0x41, 0x59
    };
    
    return code;
}
int pop_r9_size = 2;

char* pop_r10() {
    
    static char code[] = {
        0x41, 0x5A
    };
    
    return code;
}
int pop_r10_size = 2;

char* pop_r11() {
    
    static char code[] = {
        0x41, 0x5B
    };
    
    return code;
}
int pop_r11_size = 2;

char* pop_r12() {
    
    static char code[] = {
        0x41, 0x5C
    };
    
    return code;
}
int pop_r12_size = 2;

char* add() {
    
    static char code[] = {
        0x41, 0x59,
        0x41, 0x5A,
        0x4D, 0x01, 0xD1,
        0x41, 0x51
    };
    
    return code;
}
int add_size = 9;

char* sub() {
    
    static char code[] = {
        0x41, 0x59,
        0x41, 0x5A,
        0x4D, 0x29, 0xCA,
        0x41, 0x52
    };
    
    return code;
}
int sub_size = 9;

char* mul() {
    
    static char code[] = {
        0x49, 0x89, 0xC1,
        0x49, 0x89, 0xD3,
        0x58,
        0x41, 0x5A,
        0x41, 0xF7, 0xEA,
        0x41, 0xBA, 0x64, 0x00, 0x00, 0x00,
        0x41, 0xF7, 0xFA,
        0x50,
        0x4C, 0x89, 0xC8,
        0x4C, 0x89, 0xDA
    };
    
    return code;
}
int mul_size = 28;

char* div() {
    
    static char code[] = {
        0x49, 0x89, 0xC6,
        0x49, 0x89, 0xD7,
        0x41, 0x59,
        0x58,
        0x41, 0xBA, 0x64, 0x00, 0x00, 0x00,
        0x41, 0xF7, 0xEA,
        0x41, 0xF7, 0xF9,
        0x50,
        0x4C, 0x89, 0xF0,
        0x4C, 0x89, 0xFA
    };
    
    return code;
}
int div_size = 28;

char* push_int(uint32_t val) {
    
    static char code[] = {
        0x68, 0x67, 0x45, 0x23, 0x01
    };
    *(uint32_t*)(code + 1) = val;
    
    return code;
}
int push_int_size = 5;

char* sqrt() {
    
    static char code[] = {
        0x49, 0x89, 0xC7,
        0x49, 0x89, 0xDE,
        0x49, 0x89, 0xCD,
        0x49, 0x89, 0xD4,
        0x48, 0x31, 0xD2,
        0x58,
        0x41, 0xB9, 0x64, 0x00, 0x00, 0x00,
        0x41, 0xF7, 0xE9,
        0x31, 0xDB,
        0x0F, 0xBD, 0xC8,
        0x80, 0xE1, 0xFE,
        0xBA, 0x01, 0x00, 0x00, 0x00,
        0xD3, 0xE2,
        0x89, 0xDE,
        0x01, 0xD6,
        0x39, 0xC6,
        0x77, 0x08,
        0x29, 0xF0,
        0xD1, 0xEB,
        0x01, 0xD3,
        0xEB, 0x02,
        0xD1, 0xEB,
        0xC1, 0xEA, 0x02,
        0x75, 0xE9,
        0x53,
        0x4C, 0x89, 0xF8,
        0x4C, 0x89, 0xF3,
        0x4C, 0x89, 0xE9,
        0x4C, 0x89, 0xE2
    };
    
    return code;
}
int sqrt_size = 76;

char* inc_ax() {
    
    static char code[] = {
        0x48, 0xFF, 0xC0
    };
    
    return code;
}
int inc_ax_size = 3;

char* inc_bx() {
    
    static char code[] = {
        0x48, 0xFF, 0xC3
    };
    
    return code;
}
int inc_bx_size = 3;

char* inc_cx() {
    
    static char code[] = {
        0x48, 0xFF, 0xC1
    };
    
    return code;
}
int inc_cx_size = 3;

char* inc_dx() {
    
    static char code[] = {
        0x48, 0xFF, 0xC2
    };
    
    return code;
}
int inc_dx_size = 3;

char* jmp(uint32_t adr) {
    
    static char code[] = {
        0xE9, 0xBD, 0x00, 0x00, 0x00
    };
    
    code[1] = adr;
    return code;
}
int jmp_size = 5;

#endif /* comands_h */
