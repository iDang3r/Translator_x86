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

//#define w(x) cout << #x << ": " << x << endl;

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

namespace offsets {

uint32_t size         = 0x2000;
    
uint32_t  main_offset = 0x300;
uint32_t  scan_offset = 0;       // посчитаем позже
uint32_t print_offset = 0;       // посчитаем позже
uint32_t   end_offset = 0x1000;

uint32_t code_size_offset = 0;

}

namespace consts {
    
uint32_t precision = 100;

}

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

uint32_t make_header(char* buff, uint32_t buff_size);

void find_calls(char* p, char* p_end, map<string, uint32_t> &calls);

void assembling(char* buff, size_t buff_size, uint32_t offset, char* p, char* p_end,
                map<string, uint32_t> &labels, bool &second_assembling, map<string, uint32_t> &calls);

int main(int argc, const char* argv[]) {

    FILE* out = fopen("/Users/alex/Desktop/configi/test", "wb");
    
    char* buff = (char*)calloc(offsets::size, sizeof(char));
    uint32_t buff_size = offsets::size;
    uint32_t offset = make_header(buff, offsets::size);
    offset = offsets::main_offset;
    
    FILE* asm_file = fopen("/Users/alex/Desktop/~X/SquareSolv.txt", "rb");
    if (asm_file == nullptr) {
        cout << "asm File was not opened" << endl;
        return 0;
    }

    char* lang_asm = nullptr;
    size_t lang_asm_size = get_buffer(asm_file, &lang_asm);
    if (lang_asm == nullptr) {
        throw runtime_error("calloc did not allocate memory");
    }
    
    char* p     = lang_asm;
    char* p_end = lang_asm + lang_asm_size;
    
    bool need_second_assembling = false;
    
    map<string, uint32_t> labels;
    map<string, uint32_t> calls;
    
    find_calls(p, p_end, calls);
    
    assembling(buff, buff_size, offset, p, p_end, labels, need_second_assembling, calls);
    
    if (need_second_assembling) {
        assembling(buff, buff_size, offset, p, p_end, labels, need_second_assembling, calls);
    }
    
    fwrite(buff, sizeof(char), buff_size, out);
    
    cout << endl;
    
    free(buff);
    free(lang_asm);
    
    fclose(asm_file);
    fclose(out);
    
    printf("ALL DONE\n");
    
    return 0;
}

uint32_t make_header(char* buff, uint32_t buff_size) {
    
    uint32_t offset = 0;
    
    //--> Head

    mach_header_64 Head;
    Head.magic      = MH_MAGIC_64;
    Head.cputype    = CPU_TYPE_X86_64;
    Head.cpusubtype = CPU_SUBTYPE_X86_64_ALL;
    Head.filetype   = MH_EXECUTE;
    Head.ncmds      = 4;
    Head.sizeofcmds = 3 * sizeof(segment_command_64) + 2 * sizeof(section_64) + sizeof(thread_status);
    Head.flags      = MH_NOUNDEFS | MH_DYLDLINK | MH_TWOLEVEL;
    Head.reserved   = 0;

    memcpy(buff + offset, &Head, sizeof(mach_header_64));
    offset += sizeof(mach_header_64);

    //-->

    segment_command_64 segZero;
    segZero.cmd      = LC_SEGMENT_64;
    segZero.cmdsize  = sizeof(segment_command_64);
    memcpy(segZero.segname, "__PAGEZERO\0\0\0\0\0\0", 16);
    segZero.vmaddr   = 0;
    segZero.vmsize   = 0x100000000;
    segZero.fileoff  = 0;
    segZero.filesize = 0;
    segZero.maxprot  = VM_PROT_NONE;
    segZero.initprot = VM_PROT_NONE;
    segZero.nsects   = 0;
    segZero.flags    = 0;

    memcpy(buff + offset, &segZero, sizeof(segment_command_64));
    offset += sizeof(segment_command_64);

    //-->

    segment_command_64 Text;
    Text.cmd         = LC_SEGMENT_64;
    Text.cmdsize     = sizeof(segment_command_64) + sizeof(section_64);
    memcpy(Text.segname, "__TEXT\0\0\0\0\0\0\0\0\0\0", 16);
    Text.vmaddr      = 0x100000000;
    Text.vmsize      = 0x1000;
    Text.fileoff     = 0;
    Text.filesize    = 0x1000;
    Text.maxprot     = VM_PROT_READ | VM_PROT_EXECUTE | VM_PROT_WRITE;
    Text.initprot    = VM_PROT_READ | VM_PROT_EXECUTE | VM_PROT_WRITE;
    Text.nsects      = 1;
    Text.flags       = 0;

    memcpy(buff + offset, &Text, sizeof(segment_command_64));
    offset += sizeof(segment_command_64);

    //-->

    section_64 text;
    memcpy(text.sectname, "__text\0\0\0\0\0\0\0\0\0\0", 16);
    memcpy(text.segname,  "__TEXT\0\0\0\0\0\0\0\0\0\0", 16);
    text.addr        = 0x100000000 | offsets::main_offset;              ///<---- TODO address
    text.size        = offsets::end_offset - offsets::main_offset;      ///<---- TODO size
    text.offset      = offsets::main_offset;                            ///<---- TODO offset
    text.align       = 1;
    text.reloff      = 0;
    text.nreloc      = 0;
    text.flags       = S_REGULAR | S_ATTR_PURE_INSTRUCTIONS | S_ATTR_SOME_INSTRUCTIONS;
    text.reserved1   = 0;
    text.reserved2   = 0;
    text.reserved3   = 0;

    memcpy(buff + offset, &text, sizeof(section_64));
    offset += sizeof(section_64);

    //-->

    segment_command_64 Data;
    Data.cmd         = LC_SEGMENT_64;
    Data.cmdsize     = sizeof(segment_command_64) + sizeof(section_64);
    memcpy(Data.segname, "__DATA\0\0\0\0\0\0\0\0\0\0", 16);
    Data.vmaddr      = 0x100000000 | 0x1000;
    Data.vmsize      = 0x1000;
    Data.fileoff     = 0x1000;
    Data.filesize    = 0x1000;
    Data.maxprot     = VM_PROT_READ | VM_PROT_WRITE;
    Data.initprot    = VM_PROT_READ | VM_PROT_WRITE;
    Data.nsects      = 1;
    Data.flags       = 0;

    memcpy(buff + offset, &Data, sizeof(segment_command_64));
    offset += sizeof(segment_command_64);

    //-->

    section_64 data;
    memcpy(data.sectname, "__data\0\0\0\0\0\0\0\0\0\0", 16);
    memcpy(data.segname,  "__DATA\0\0\0\0\0\0\0\0\0\0", 16);
    data.addr        = 0x100000000 | 0x1000;
    data.size        = 512;
    data.offset      = 0x1000;
    data.align       = 1;
    data.reloff      = 0;
    data.nreloc      = 0;
    data.flags       = S_REGULAR;
    data.reserved1   = 0;
    data.reserved2   = 0;
    data.reserved3   = 0;

    memcpy(buff + offset, &data, sizeof(section_64));
    offset += sizeof(section_64);

    //-->

    thread_status unixth;
    unixth.cmd       = LC_UNIXTHREAD;
    unixth.cmdsize   = sizeof(thread_status);
    unixth.flavor    = x86_THREAD_STATE64;
    unixth.count     = 42;
    unixth.rax       = 0;
    unixth.rbx       = 0;
    unixth.rcx       = 0;
    unixth.rdx       = 0;
    unixth.rdi       = 0;
    unixth.rsi       = 0;
    unixth.rbp       = 0;
    unixth.rsp       = 0;
    unixth.r8        = 0;
    unixth.r9        = 0;
    unixth.r10       = 0;
    unixth.r11       = 0;
    unixth.r12       = 0;
    unixth.r13       = 0;
    unixth.r14       = 0;
    unixth.r15       = 0;
    unixth.rip       = 0x100000000 | offsets::main_offset;     ///<-- TODO  RIP
    unixth.rflags    = 0;
    unixth.cs        = 0;
    unixth.fs        = 0;
    unixth.gs        = 0;

    memcpy(buff + offset, &unixth, sizeof(thread_status));
    offset += sizeof(thread_status);

    //-->

    uint32_t offset_cpy = offset;
    
    offset = offsets::end_offset - print_lib_size - scan_lib_size;
    
    offsets::scan_offset = offset;
    command(scan_lib);
    
    offsets::print_offset = offset;
    command(print_lib);
    
    offset = offset_cpy;
    
    return offset;
}


void assembling(char* buff, size_t buff_size, uint32_t offset, char* p, char* p_end,
                map<string, uint32_t> &labels, bool &second_assembling, map<string, uint32_t> &calls) {
    
    int n = 0;
    uint32_t x = 0;
    char name_of_command[100 + 1] = {};
    char name_of_registr[10  + 1] = {};
    
    command(mov_r8_memptr);
    
    while (p < p_end) {
        if (*p == ' ' || *p == '\n' || *p == '\t') {
            p++;
            continue;
        }
        
        sscanf(p, "%[a-z_0-9]%n", name_of_command, &n);
        p += n;
        
        if (*p == ':') { // add back jmp
            labels[name_of_command] = offset;
            p++;
            
            if (calls[name_of_command] != 0) {
                command(pop_rdi);
            }
            
            continue;
        }
        
        if (!strcmp(name_of_command, "push")) {
            
            sscanf(p, "%d%n", &x, &n);
            p += n;
            
            command_p(push_int, x * consts::precision);
            
        } else if (!strcmp(name_of_command, "pushm")) {
            
            sscanf(p, " [%n", &n);
            p += n;
            
            if ('0' <= *p && *p <= '9') {
                
                sscanf(p, "%d]%n", &x, &n);
                p += n;
                
                command_p(mov_r9_from_mem, x);
                command(push_r9);
                
            } else {
                
                sscanf(p, "ax + %d]%n", &x, &n);
                p += n;
                
                command_p(mov_r9_from_mem_add_ax, x);
                command(push_r9);
                
            }
            
        } else if (!strcmp(name_of_command, "popm")) {
            
            sscanf(p, " [%n", &n);
            p += n;
            
            if ('0' <= *p && *p <= '9') {
                
                sscanf(p, "%d]%n", &x, &n);
                p += n;
                
                command(pop_r9);
                command_p(mov_mem_from_r9, x);
                
            } else {
                
                sscanf(p, "ax + %d]%n", &x, &n);
                p += n;
                
                command(pop_r9);
                command_p(mov_mem_add_ax_from_r9, x);
                
            }
            
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
            
            scan_label_and_check();
            command(save_ret);
            jump(call, labels[name_of_command]);
            command(rest_ret);
            
        } else if (!strcmp(name_of_command, "ret")) {
            
            command(push_rdi);
            command(ret);
            
        } else if (!strcmp(name_of_command, "in")) {
            
            jump(scan, offsets::scan_offset);
            command(push_r9);
            
        } else if (!strcmp(name_of_command, "out")) {
            
            command(pop_r9);
            jump(print, offsets::print_offset);
            
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
                
                printf("register: %s\n", name_of_registr);
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
                
                printf("register: %s\n", name_of_registr);
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
                
                printf("register: %s\n", name_of_registr);
                throw runtime_error("Incorrect name of register");
                
            }
            
        } else if (!strcmp(name_of_command, "jmp")) {
            
            scan_label_and_check();
            jump(jmp, labels[name_of_command]);
            
        } else if (!strcmp(name_of_command, "ja")) {
            
            scan_label_and_check();
            jump(ja, labels[name_of_command]);
            
        } else if (!strcmp(name_of_command, "jae")) {
            
            scan_label_and_check();
            jump(jae, labels[name_of_command]);
            
        } else if (!strcmp(name_of_command, "jb")) {
            
            scan_label_and_check();
            jump(jb, labels[name_of_command]);
            
        } else if (!strcmp(name_of_command, "jbe")) {
            
            scan_label_and_check();
            jump(jbe, labels[name_of_command]);
            
        } else if (!strcmp(name_of_command, "je")) {
            
            scan_label_and_check();
            jump(je, labels[name_of_command]);
            
        } else if (!strcmp(name_of_command, "jne")) {
            
            scan_label_and_check();
            jump(jne, labels[name_of_command]);
            
        } else if (!strcmp(name_of_command, "end")) {
            
            command(program_end);
            
        } else {
            
            printf("command: %s\n", name_of_command);
            throw runtime_error("Undefined name of command");
            
        }
        
    }
    
    command(program_end);
    
}

void find_calls(char* p, char* p_end, map<string, uint32_t> &calls) {
    
    int n = 0;
    char name_of_command[100 + 1] = {};
    
    while (p < p_end) {
        
        if (*p == ' ' || *p == '\n' || *p == '\t') {
            p++;
            continue;
        }
        
        sscanf(p, "%[a-z_0-9]%n", name_of_command, &n);
        p += n;
        
        if (*p == ':') {
            p++;
            continue;
        }
        
        if (!strcmp(name_of_command, "call")) {
            
            sscanf(p, " %[a-z_0-9]%n", name_of_command, &n);
            p += n;
            
            calls[name_of_command] = 1;
        }
        
    }
    
}
