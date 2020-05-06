//
//  main.cpp
//  Translator_x86
//
//  Created by Александр on 03.05.2020.
//  Copyright © 2020 Alex. All rights reserved.
//

#include <iostream>
#include <mach-o/loader.h>
#include <mach/machine/thread_status.h>
#include <iomanip>
#include "comands.h"
#include <map>
#include <string>
using namespace std;

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

size_t get_buffer(FILE* in, char* *r_buffer) {

    assert(in);

    fseek(in, 0, SEEK_END);
    size_t input_file_size = ftell(in);
    rewind(in);

    char* buffer = (char*)calloc(input_file_size + 2, sizeof(char));
    if (buffer == nullptr) {
        throw runtime_error("calloc did not allocate memory");
        return 0;
    }

    *r_buffer = buffer;

    size_t buffer_size = fread(buffer, sizeof(char), input_file_size, in);
    if (buffer_size != input_file_size) {
        cout << input_file_size << " " << buffer_size << endl;
        *r_buffer = nullptr;
        throw runtime_error("size of file and size of buffer are not equal");
        return 0;
    }

    return buffer_size;
}

#define w(x) cout << #x << " " << x << endl;

int main(int argc, const char * argv[]) {
    
    FILE* templ = fopen("/Users/alex/Desktop/configi/new_tmplte", "rb");
    if (templ == nullptr) {
        cout << "File was not opened" << endl;
        return 0;
    }
    FILE* out = fopen("/Users/alex/Desktop/configi/test", "wb");
    cout << hex;
    
    char* buff = nullptr;
    size_t buff_size = get_buffer(templ, &buff);
    
    uint32_t offset = 0;
    
//    //--> Head
//
//    mach_header_64 Head;
//    Head.magic      = MH_MAGIC_64;
//    Head.cputype    = CPU_TYPE_X86_64;
//    Head.cpusubtype = CPU_SUBTYPE_X86_64_ALL;
//    Head.filetype   = MH_EXECUTE;
//    Head.ncmds      = 4;
//    Head.sizeofcmds = 4 * sizeof(segment_command_64) + 2 * sizeof(section_64);
//    Head.flags      = MH_NOUNDEFS | MH_DYLDLINK | MH_TWOLEVEL;
//    Head.reserved   = 0;
//
//    offset += sizeof(mach_header_64);
//
//    //-->
//
//    segment_command_64 segZero;
//    segZero.cmd      = LC_SEGMENT_64;
//    segZero.cmdsize  = sizeof(segment_command_64);
//    memcpy(segZero.segname, "__PAGEZERO\0\0\0\0\0\0", 16);
//    segZero.vmaddr   = 0;
//    segZero.vmsize   = 0x100000000;
//    segZero.fileoff  = 0;
//    segZero.filesize = 0;
//    segZero.maxprot  = VM_PROT_NONE;
//    segZero.initprot = VM_PROT_NONE;
//    segZero.nsects   = 0;
//    segZero.flags    = 0;
//
//    offset += sizeof(segment_command_64);
//
//    //-->
//
//    segment_command_64 Text;
//    Text.cmd         = LC_SEGMENT_64;
//    Text.cmdsize     = sizeof(segment_command_64) + sizeof(section_64);
//    memcpy(Text.segname, "__TEXT\0\0\0\0\0\0\0\0\0\0", 16);
//    Text.vmaddr      = 0x100000000;
//    Text.vmsize      = 4096;  // TODO
//    Text.fileoff     = 0;
//    Text.filesize    = 4096;
//    Text.maxprot     = VM_PROT_READ | VM_PROT_EXECUTE;
//    Text.initprot    = VM_PROT_READ | VM_PROT_EXECUTE;
//    Text.nsects      = 1;
//    Text.flags       = 0;
//
//    offset += sizeof(segment_command_64);
//
//    //-->
//
//    section_64 text;
//    memcpy(text.sectname, "__text\0\0\0\0\0\0\0\0\0\0", 16);
//    memcpy(text.segname,  "__TEXT\0\0\0\0\0\0\0\0\0\0", 16);
//    text.addr        = 0x100000000 | (0x0);                 ///<---- TODO address
//    text.size        = 80;                                  ///<---- TODO size
//    text.offset      = 0xF68;                               ///<---- TODO offset
//    text.align       = 1;
//    text.reloff      = 0;
//    text.nreloc      = 0;
//    text.flags       = S_REGULAR | S_ATTR_PURE_INSTRUCTIONS | S_ATTR_SOME_INSTRUCTIONS;
//    text.reserved1   = 0;
//    text.reserved2   = 0;
//    text.reserved3   = 0;
//
//    offset += sizeof(section_64);
//
//    //-->
//
//    segment_command_64 Data;
//    Data.cmd         = LC_SEGMENT_64;
//    Data.cmdsize     = sizeof(segment_command_64) + sizeof(section);
//    memcpy(Data.segname, "__DATA\0\0\0\0\0\0\0\0\0\0", 16);
//    Data.vmaddr      = 0x100000000 | 0x1000;
//    Data.vmsize      = 0x1000;
//    Data.fileoff     = 0x1000;
//    Data.filesize    = 0x1000;
//    Data.maxprot     = VM_PROT_READ | VM_PROT_WRITE;
//    Data.initprot    = VM_PROT_READ | VM_PROT_WRITE;
//    Data.nsects      = 1;
//    Data.flags       = 0;
//
//    offset += sizeof(segment_command_64);
//
//    //-->
//
//    section_64 data;
//    memcpy(data.sectname, "__data\0\0\0\0\0\0\0\0\0\0", 16);
//    memcpy(data.segname,  "__DATA\0\0\0\0\0\0\0\0\0\0", 16);
//    data.addr        = 0x100000000 | 0x1000;
//    data.size        = 256;                              ///<---- TODO size
//    data.offset      = 0x1000;
//    data.align       = 1;
//    data.reloff      = 0;
//    data.nreloc      = 0;
//    data.flags       = S_REGULAR;
//    data.reserved1   = 0;
//    data.reserved2   = 0;
//    data.reserved3   = 0;
//
//    offset += sizeof(section_64);
//
//    //-->
//
//    thread_status unixth;
//    unixth.cmd       = LC_UNIXTHREAD;
//    unixth.cmdsize   = sizeof(thread_status);
//    unixth.flavor    = x86_THREAD_STATE64;
//    unixth.count     = 42;
//    unixth.rax       = 0;
//    unixth.rbx       = 0;
//    unixth.rcx       = 0;
//    unixth.rdx       = 0;
//    unixth.rdi       = 0;
//    unixth.rsi       = 0;
//    unixth.rbp       = 0;
//    unixth.rsp       = 0;
//    unixth.r8        = 0;
//    unixth.r9        = 0;
//    unixth.r10       = 0;
//    unixth.r11       = 0;
//    unixth.r12       = 0;
//    unixth.r13       = 0;
//    unixth.r14       = 0;
//    unixth.r15       = 0;
//    unixth.rip       = 0x100000000 | 0x0420;             ///<-- TODO  RIP
//    unixth.rflags    = 0;
//    unixth.cs        = 0;
//    unixth.fs        = 0;
//    unixth.gs        = 0;
//
//    offset += sizeof(thread_status);
//
//    //-->
//
//    char* result_buff = (char*)calloc(0x2100, 1);
//    size_t result_buff_size = 0;
//
//    memcpy(result_buff + result_buff_size, &Head, sizeof(mach_header));
//    result_buff_size += sizeof(mach_header);
//
//    memcpy(result_buff + result_buff_size, &segZero, sizeof(segment_command_64));
//    result_buff_size += sizeof(segment_command_64);
//
//    memcpy(result_buff + result_buff_size, &Text, sizeof(segment_command_64));
//    result_buff_size += sizeof(segment_command_64);
//
//    memcpy(result_buff + result_buff_size, &text, sizeof(section_64));
//    result_buff_size += sizeof(section_64);
//
//    memcpy(result_buff + result_buff_size, &Data, sizeof(segment_command_64));
//    result_buff_size += sizeof(segment_command_64);
//
//    memcpy(result_buff + result_buff_size, &data, sizeof(section_64));
//    result_buff_size += sizeof(section_64);
//
//    memcpy(result_buff + result_buff_size, &unixth, sizeof(thread_status));
//    result_buff_size += sizeof(thread_status);
//
//    result_buff_size = 0x0420;
//    memcpy(result_buff + result_buff_size, asm_code, sizeof(asm_code));
    
    enum {
        CALL = 1,
        JMP  = 2,
        JA   = 3,
        JAE  = 4,
        JB   = 5,
        JBE  = 6,
        JE   = 7,
        JNE  = 8,
    };
    
    FILE* asm_file = fopen("/Users/alex/Desktop/~X/SquareSolv.txt", "rb");
    if (asm_file == nullptr) {
        cout << "asm File was not opened" << endl;
        return 0;
    }
    
    #define command(x)                      \
    memcpy(buff + offset, x(), x##_size);   \
    offset += x##_size                      \

    #define command_p(x, y)                 \
    memcpy(buff + offset, x(y), x##_size);  \
    offset += x##_size                      \
    
    char* lang_asm = nullptr;
    size_t lang_asm_size = get_buffer(asm_file, &lang_asm);
    
    char* p     = lang_asm;
    char* p_end = lang_asm + lang_asm_size;
    int n = 0;
    uint32_t x = 0;
    char name_of_command[100 + 1] = {};
    char name_of_registr[10  + 1] = {};
    bool need_second_assembling = false;
    
    map<string, uint32_t> labels;
    
    w(buff_size);
    uint32_t begin_offset = 0x714;
    offset = begin_offset;
    
    memcpy(buff + offset, mov_r8_memptr(), mov_r8_memptr_size);
    offset += mov_r8_memptr_size;
    
    while (p < p_end) {
        
        sscanf(p, "%[a-z_0-9<>=!]%n", name_of_command, &n);
        p += n;
        
        if (*p == ':') { // add back jmp
            
            labels[name_of_command] = offset;
            continue;
        }
        
        if (!strcmp(name_of_command, "push")) {
            
            sscanf(p, "%d%n", &x, &n);
            p += n;
            
            memcpy(buff + offset, push_int(x * 100), push_int_size);
            offset += push_int_size;
            
        } else if (!strcmp(name_of_command, "pushm")) {
            
            
        } else if (!strcmp(name_of_command, "popm")) {
            
            
        } else if (!strcmp(name_of_command, "add")) {
            
            command(add);
            
        } else if (!strcmp(name_of_command, "sub")) {
            
            command(sub);
            
        } else if (!strcmp(name_of_command, "mul")) {
            
            command(mul);
            
        } else if (!strcmp(name_of_command, "div")) {
            
            command(div);
            
        } else if (!strcmp(name_of_command, "sqrt")) {
            
            command(sqrt);
            
        } else if (!strcmp(name_of_command, "call")) {
            
            sscanf(p, " %[a-z_0-9]%n", name_of_command, &n);
            p += n;
            
            if (labels[name_of_command] != 0) { // метка есть
                
                memcpy(buff + offset, call(labels[name_of_command] - offset - call_size), call_size);
                buff += call_size;
                
            } else { // метки нет(
                
                need_second_assembling = true;
                offset += call_size;
                
            }
            
        } else if (!strcmp(name_of_command, "ret")) {
            
            command(ret);
            
        } else if (!strcmp(name_of_command, "in")) {
            
            memcpy(buff + offset, scan(0xE47 - offset - scan_size), scan_size);
            offset += scan_size;
            
        } else if (!strcmp(name_of_command, "out")) {
            
            memcpy(buff + offset, print(0xED6 - offset - print_size), print_size);
            offset += print_size;
            
        } else if (!strcmp(name_of_command, "inc")) {
            
            sscanf(p, " %[a-z0-9]%n", name_of_registr, &n);
            p += n;
            
            if (!strcmp(name_of_registr, "ax")) {
                
                command(inc_ax);
                
            } else if (!strcmp(name_of_registr, "bx")) {
                
                command(inc_bx);
                
            } else if (!strcmp(name_of_registr, "cx")) {
                
                command(inc_cx);
                
            } else if (!strcmp(name_of_registr, "dx")) {
                
                command(inc_dx);
                
            } else {
                
                w(name_of_registr);
                throw runtime_error("Incorrect name of register");
                
            }
            
        } else if (!strcmp(name_of_command, "pushr")) {
            
            sscanf(p, " %[a-z0-9]%n", name_of_registr, &n);
            p += n;
            
            if (!strcmp(name_of_registr, "ax")) {
                
                command(push_rax);
                
            } else if (!strcmp(name_of_registr, "bx")) {
                
                command(push_rbx);
                
            } else if (!strcmp(name_of_registr, "cx")) {
                
                command(push_rcx);
                
            } else if (!strcmp(name_of_registr, "dx")) {
                
                command(push_rdx);
                
            } else {
                
                w(name_of_registr);
                throw runtime_error("Incorrect name of register");
                
            }
            
        } else if (!strcmp(name_of_command, "popr")) {
            
            sscanf(p, " %[a-z0-9]%n", name_of_registr, &n);
            p += n;
            
            if (!strcmp(name_of_registr, "ax")) {
                
                command(pop_rax);
                
            } else if (!strcmp(name_of_registr, "bx")) {
                
                command(pop_rbx);
                
            } else if (!strcmp(name_of_registr, "cx")) {
                
                command(pop_rcx);
                
            } else if (!strcmp(name_of_registr, "dx")) {
                
                command(pop_rdx);
                
            } else {
                
                w(name_of_registr);
                throw runtime_error("Incorrect name of register");
                
            }
            
        } else if (!strcmp(name_of_command, "jmp")) {
            
            
        } else if (!strcmp(name_of_command, "ja")) {
            
            
        } else if (!strcmp(name_of_command, "jae")) {
            
            
        } else if (!strcmp(name_of_command, "jb")) {
            
            
        } else if (!strcmp(name_of_command, "jbe")) {
            
            
        } else if (!strcmp(name_of_command, "je")) {
            
            
        } else if (!strcmp(name_of_command, "jne")) {
            
            
        } else {
            
            cout << name_of_command << endl;
            throw runtime_error("Undefined name of command");
            
        }
        
        sscanf(p, "\n%n", &n);
        p += n;
    }
    
    command(program_end);

/*
    
    memcpy(buff + offset, mov_r8_memptr(), mov_r8_memptr_size);
    offset += mov_r8_memptr_size;
    
    memcpy(buff + offset, scan(0xE77 - offset - scan_size), scan_size);
    offset += scan_size;
    
//    memcpy(buff + offset, scan(0xE77 - offset - scan_size), scan_size);
//    offset += scan_size;
    
    command(sqrt);
    
    memcpy(buff + offset, print(0xEEE - offset - print_size), print_size);
    offset += print_size;
    
 */
    
    fwrite(buff, 1, buff_size, out);
    
    cout << endl;
    
    free(buff);
    free(lang_asm);
    
    fclose(asm_file);
    fclose(out);
    fclose(templ);
    
    return 0;
}
