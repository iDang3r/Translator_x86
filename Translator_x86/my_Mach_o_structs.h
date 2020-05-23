//
//  my_Mach_o_structs.h
//  Translator_x86
//
//  Created by Александр on 22.05.2020.
//  Copyright © 2020 Alex. All rights reserved.
//

#ifndef my_Mach_o_structs_h
#define my_Mach_o_structs_h

struct thread_status {
    
    uint32_t    cmd;
    uint32_t    cmdsize;
    uint32_t    flavor;
    uint32_t    count;
    
    uint64_t    rax;
    uint64_t    rbx;
    uint64_t    rcx;
    uint64_t    rdx;
    uint64_t    rdi;
    uint64_t    rsi;
    uint64_t    rbp;
    uint64_t    rsp;
    uint64_t    r8;
    uint64_t    r9;
    uint64_t    r10;
    uint64_t    r11;
    uint64_t    r12;
    uint64_t    r13;
    uint64_t    r14;
    uint64_t    r15;
    uint64_t    rip;
    uint64_t    rflags;
    uint64_t    cs;
    uint64_t    fs;
    uint64_t    gs;
    
};

#endif /* my_Mach_o_structs_h */
