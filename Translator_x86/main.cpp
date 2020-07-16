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
#include "commands_translator.h"
#include "my_map.h"
#include "super_define.h"
#include "my_Mach_o_structs.h"
#include "instructions.h"
#include "my_hash_table.h"

using std::cout;
using std::endl;
using std::cin;
using std::runtime_error;

namespace offsets {

uint32_t size         = 0x2000;
    
uint32_t  main_offset = 0x300;
uint32_t  scan_offset = 0;       // посчитаем позже
uint32_t print_offset = 0;       // посчитаем позже
uint32_t   end_offset = 0x1000;

uint32_t code_size_offset = 0;   // посчитаем позже

}

namespace consts {
    
const uint32_t precision = 100;

}

///---->// Getting my_asm codes

#define DEF_CMD(name, num, next, cpu_code, disasm_code) \
    name##_ = num,

enum my_asm_codes {
    /// AUTO_GENERATING ENUM
    #include "commands.h"
    
};

#undef DEF_CMD

///---->// Getting registers numbres
    
#define DEF_REG(name, num) \
    reg_##name = num,
    
enum registers {
    /// AUTO_GENERATING ENUM
    #include "registers.h"
};
    
#undef DEF_REG

size_t get_buffer(FILE* in, char* *r_buffer);

uint32_t make_header(char* buff, uint32_t buff_size);

void find_calls_assembling(char* p, char* p_end, my_map &calls);

void find_calls_and_labels_translating(char* p, char* p_end, Hash_table<char*, bool> &calls, Hash_table<char*, bool> &labels,
                                       Hash_table<char, int> &my_asm_codes_size);

void assembling(char* buff, size_t buff_size, uint32_t offset, char* p, char* p_end,
                my_map &labels, bool &second_assembling, my_map &calls);

void translating(char* buff, size_t buff_size, uint32_t offset, char* p, char* p_end,
                 Hash_table<char*, bool> &labels, bool &second_assembling, Hash_table<char*, bool> &calls, Hash_table<char, int> &my_asm_codes_size,
                 Hash_table<char*, uint32_t> &labels_offsets);

int main(int argc, const char* argv[]) {
    
    if ((argc >= 2 && !strcmp(argv[1], "-help")) || (argc < 3 || argc > 4)) {
        cout << "Input format: \n";
        cout << "1)  Name of file with my_asm code\n";
        cout << "2)  Name out output executable file\n";
        cout << "3*) \"-asm\" if you want to get executable file from my_asm\n";
        return 0;
    }
    
    bool binary_translate = true; // false means assembling from my_asm
    if (argc == 4) {
        
        if (!strcmp(argv[3], "-asm")) {
            binary_translate = false;
        } else {
            cout << "Incorrect 3rd console argument\n";
            cout << "To get more information use \"-help\" flag\n";
            return 0;
        }
        
    }
    
    FILE* out = fopen(argv[2], "wb");
    if (out == nullptr) {
        cout << "output file was not opened or created" << endl;
        return 0;
    }
    // ПМИ АААААААаааа
    char* buff = reinterpret_cast<char*>(calloc(offsets::size, sizeof(char)));
    if (buff == nullptr) {
        throw runtime_error("Calloc did not allocate memory");
    }
    uint32_t buff_size = offsets::size;
    
    uint32_t offset = make_header(buff, offsets::size);
    offset = offsets::main_offset;
    
//    Hash_table<int, int>* hash_t = new Hash_table<int, int>();
//    char* buff_ch = new char[100] {0};
    
    FILE* asm_file = fopen(argv[1], "rb");
    if (asm_file == nullptr) {
        cout << "asm File was not opened" << endl;
        return 0;
    }

    char* lang_asm = nullptr;
    size_t lang_asm_size = get_buffer(asm_file, &lang_asm);
    if (lang_asm == nullptr) {
        throw runtime_error("Calloc did not allocate memory");
    }
    
    char* p     = lang_asm;
    char* p_end = lang_asm + lang_asm_size;
    
    bool need_second_running = false;
    
    if (binary_translate) {
        Hash_table<char, int>   my_asm_codes_size;
        Hash_table<char*, bool> calls, labels;
            
        // Getting commands sizes
            
    #define DEF_CMD(name, num, next, cpu_code, disasm_code) \
        my_asm_codes_size[num] = next;
            
        #include "commands.h"

    #undef DEF_CMD
        
        find_calls_and_labels_translating(p, p_end, calls, labels, my_asm_codes_size);
        
        Hash_table<char*, uint32_t> labels_offsets;
        translating(buff, buff_size, offset, p, p_end, labels, need_second_running, calls, my_asm_codes_size, labels_offsets);
        
        if (need_second_running) {
            translating(buff, buff_size, offset, p, p_end, labels, need_second_running, calls, my_asm_codes_size, labels_offsets);
        }
        
    } else {
        
        my_map labels;
        my_map calls;
        
        find_calls_assembling(p, p_end, calls);
        
        assembling(buff, buff_size, offset, p, p_end, labels, need_second_running, calls);
        
        if (need_second_running) {
            assembling(buff, buff_size, offset, p, p_end, labels, need_second_running, calls);
        }
        
    }
    
    fwrite(buff, sizeof(char), buff_size, out);
    
    free(buff);
    free(lang_asm);
    
    fclose(asm_file);
    fclose(out);
    
    // Making output file executable
    // Works on MacOS, erase these commands if they don't work, and change file mode in console by yourself
    char* sys_command = concat("chmod 755 ", argv[2]);
    if (sys_command == nullptr) {
        throw runtime_error("Calloc did not allocate memory");
    }
    system(sys_command);
    free(sys_command);
    
    printf("ALL DONE\n");
    
    return 0;
}

uint32_t make_header(char* buff, uint32_t buff_size) {
    
    uint32_t offset = 0;
    
    //--> Head

    mach_header_64 Head = {
        .magic      = MH_MAGIC_64,
        .cputype    = CPU_TYPE_X86_64,
        .cpusubtype = CPU_SUBTYPE_X86_64_ALL,
        .filetype   = MH_EXECUTE,
        .ncmds      = 4,
        .sizeofcmds = 3 * sizeof(segment_command_64) + 2 * sizeof(section_64) + sizeof(thread_status),
        .flags      = MH_NOUNDEFS | MH_DYLDLINK | MH_TWOLEVEL,
        .reserved   = 0
    };

    memcpy(buff + offset, &Head, sizeof(mach_header_64));
    offset += sizeof(mach_header_64);

    //-->

    segment_command_64 segZero = {
        .cmd      = LC_SEGMENT_64,
        .cmdsize  = sizeof(segment_command_64),
        .segname  = "__PAGEZERO\0\0\0\0\0",
        .vmaddr   = 0,
        .vmsize   = 0x100000000,
        .fileoff  = 0,
        .filesize = 0,
        .maxprot  = VM_PROT_NONE,
        .initprot = VM_PROT_NONE,
        .nsects   = 0,
        .flags    = 0
    };
    
    memcpy(buff + offset, &segZero, sizeof(segment_command_64));
    offset += sizeof(segment_command_64);

    //-->

    segment_command_64 Text = {
        .cmd         = LC_SEGMENT_64,
        .cmdsize     = sizeof(segment_command_64) + sizeof(section_64),
        .segname     = "__TEXT\0\0\0\0\0\0\0\0\0",
        .vmaddr      = 0x100000000,
        .vmsize      = 0x1000,
        .fileoff     = 0,
        .filesize    = 0x1000,
        .maxprot     = VM_PROT_READ | VM_PROT_EXECUTE,
        .initprot    = VM_PROT_READ | VM_PROT_EXECUTE,
        .nsects      = 1,
        .flags       = 0
    };

    memcpy(buff + offset, &Text, sizeof(segment_command_64));
    offset += sizeof(segment_command_64);

    //-->

    section_64 text = {
        .sectname    = "__text\0\0\0\0\0\0\0\0\0",
        .segname     = "__TEXT\0\0\0\0\0\0\0\0\0",
        .addr        = static_cast<uint64_t>(0x100000000 | offsets::main_offset),
        .size        = offsets::end_offset - offsets::main_offset,
        .offset      = offsets::main_offset,
        .align       = 1,
        .reloff      = 0,
        .nreloc      = 0,
        .flags       = S_REGULAR | S_ATTR_PURE_INSTRUCTIONS | S_ATTR_SOME_INSTRUCTIONS,
        .reserved1   = 0,
        .reserved2   = 0,
        .reserved3   = 0
    };
    
    memcpy(buff + offset, &text, sizeof(section_64));
    offset += sizeof(section_64);

    //-->

    segment_command_64 Data = {
        .cmd         = LC_SEGMENT_64,
        .cmdsize     = sizeof(segment_command_64) + sizeof(section_64),
        .segname     = "__DATA\0\0\0\0\0\0\0\0\0",
        .vmaddr      = 0x100000000 | 0x1000,
        .vmsize      = 0x1000,
        .fileoff     = 0x1000,
        .filesize    = 0x1000,
        .maxprot     = VM_PROT_READ | VM_PROT_WRITE,
        .initprot    = VM_PROT_READ | VM_PROT_WRITE,
        .nsects      = 1,
        .flags       = 0
    };

    memcpy(buff + offset, &Data, sizeof(segment_command_64));
    offset += sizeof(segment_command_64);

    //-->

    section_64 data = {
        .sectname    = "__data\0\0\0\0\0\0\0\0\0",
        .segname     = "__DATA\0\0\0\0\0\0\0\0\0",
        .addr        = 0x100000000 | 0x1000,
        .size        = 512,
        .offset      = 0x1000,
        .align       = 1,
        .reloff      = 0,
        .nreloc      = 0,
        .flags       = S_REGULAR,
        .reserved1   = 0,
        .reserved2   = 0,
        .reserved3   = 0
    };

    memcpy(buff + offset, &data, sizeof(section_64));
    offset += sizeof(section_64);

    //-->

    thread_status unixth = {
        .cmd       = LC_UNIXTHREAD,
        .cmdsize   = sizeof(thread_status),
        .flavor    = x86_THREAD_STATE64,
        .count     = 42,
        .rax       = 0,
        .rbx       = 0,
        .rcx       = 0,
        .rdx       = 0,
        .rdi       = 0,
        .rsi       = 0,
        .rbp       = 0,
        .rsp       = 0,
        .r8        = 0,
        .r9        = 0,
        .r10       = 0,
        .r11       = 0,
        .r12       = 0,
        .r13       = 0,
        .r14       = 0,
        .r15       = 0,
        .rip       = static_cast<uint64_t>(0x100000000 | offsets::main_offset),
        .rflags    = 0,
        .cs        = 0,
        .fs        = 0,
        .gs        = 0,
    };

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

void translating(char* buff, size_t buff_size, uint32_t offset, char* p, char* p_end,
                 Hash_table<char*, bool> &labels, bool &second_translating, Hash_table<char*, bool> &calls,
                 Hash_table<char, int> &my_asm_codes_size, Hash_table<char*, uint32_t> &labels_offsets) {

    char* p_begin = p;
    p += sizeof(Signature);
    
    command(mov_r8_memptr);
    
    while (p < p_end) {
        
        if (calls[p]) { // call jumps here
            labels_offsets[p] = offset;
            command(pop_rdi);
        }
        
        if (labels[p]) { // jump jumps here
            labels_offsets[p] = offset;
        }
        
        switch (*p) {
            case push_:
                
                command_p(push_int, *(int32_t*)(p + 1));
                
                break;
                
            case pushm_:
                
                if (*(p + 1) == 0) {
                    
                    command_p(mov_r9_from_mem, *(int32_t*)(p + 2));
                    
                } else if (*(p + 1) == reg_ax) {
                    
                    command_p(mov_r9_from_mem_add_ax, *(int32_t*)(p + 2));
                    
                } else {
                    
                    throw runtime_error("Memory access on this register is not available(only ax supporting)");
                    
                }
                
                command(push_r9);
                
                break;
                
            case popm_:
                
                command(pop_r9);
                
                if (*(p + 1) == 0) {
                    
                    command_p(mov_mem_from_r9, *(int32_t*)(p + 2));
                    
                } else if (*(p + 1) == reg_ax) {
                    
                    command_p(mov_mem_add_ax_from_r9, *(int32_t*)(p + 2));
                    
                } else {
                    
                    throw runtime_error("Memory access on this register is not available(only ax supporting)");
                    
                }
                
                break;
                
            case add_:
                
                command(pop_r9);
                command(pop_r10);
                command(add_r9_r10);
                command(push_r9);
                
                break;
                
            case sub_:
                
                command(pop_r9);
                command(pop_r10);
                command(sub_r10_r9);
                command(push_r10);
            
                break;
                
            case mul_:
                
                command(mov_r15_rax);
                command(mov_r14_rdx);
                
                command(pop_rax);
                command(pop_r9);
                command(mul_r9);
                command(mov_r9_0x64);
                command(cdq);
                command(div_r9);
                command(push_rax);
                
                command(mov_rax_r15);
                command(mov_rdx_r14);
            
                break;
                
            case div_:
                
                command(mov_r15_rax);
                command(mov_r14_rdx);
                
                command(pop_r10);
                command(pop_rax);
                command(mov_r9_0x64);
                command(mul_r9);
                command(cdq);
                command(div_r10);
                command(push_rax);
                
                command(mov_rax_r15);
                command(mov_rdx_r14);
                
                break;
                
            case sqrt_:
                
                command(mov_r15_rax);
                command(mov_r14_rbx);
                command(mov_r13_rcx);
                command(mov_r12_rdx);
                command(mov_r11_rsi);
                
                command(xor_rdx);         ///
                command(pop_rax);         ///
                command(mov_r9_0x64);     ///
                command(mul_r9);          ///
                command(xor_rbx);         ///
                command(bsr_ecx_eax);     ///
                command(and_cl_0xFE);     ///
                command(mov_edx_0x1);     ///
                command(shl_edx_cl);      ///
                
                command(mov_esi_ebx);     ///<<-------------@
                command(add_esi_edx);     ///               |
                command(cmp_esi_eax);     ///               |
                command_p(ja_byte, 0x08); ///>>--------@    |
                command(sub_eax_esi);     ///          |    |
                command(shr_ebx);         ///          |    |
                command(add_ebx_edx);     ///          |    |
                command_p(jmp_byte, 0x02);///>>---@    |    |
                                          ///     |    |    |
                command(shr_ebx);         ///<<---+----@    |
                                          ///     |         |
                command(shr_edx_0x2);     ///<<---@         |
                command_p(jne_byte, 0xE9);///>>-------------@
                
                command(push_rbx);        ///
                
                command(mov_rax_r15);
                command(mov_rbx_r14);
                command(mov_rcx_r13);
                command(mov_rdx_r12);
                command(mov_rsi_r11);
                
                break;
                
            case in_:
                
                jump(call, offsets::scan_offset);
                command(push_r9);
                
                break;
                
            case out_:
                
                command(pop_r9);
                jump(call, offsets::print_offset);
                
                break;
                
            case pushr_:
                
                switch (*(p + 1)) {
                        
                    case reg_ax:
                        command(push_rax);
                        break;
                        
                    case reg_bx:
                        command(push_rbx);
                        break;
                        
                    case reg_cx:
                        command(push_rcx);
                        break;
                        
                    case reg_dx:
                        command(push_rdx);
                        break;
                }
                
                break;
                
            case popr_:
                
                switch (*(p + 1)) {
                        
                    case reg_ax:
                        command(pop_rax);
                        break;
                        
                    case reg_bx:
                        command(pop_rbx);
                        break;
                        
                    case reg_cx:
                        command(pop_rcx);
                        break;
                        
                    case reg_dx:
                        command(pop_rdx);
                        break;
                }
                
                break;
                
            case call_:
                
                if (labels_offsets[p_begin + *(uint32_t*)(p + 1)] == 0) {
                    second_translating = true;
                }
                
                command(save_ret);
                jump(call, labels_offsets[p_begin + *(uint32_t*)(p + 1)]);
                command(rest_ret);
                
                break;
                
            case ret_:
                
                command(push_rdi);
                command(ret);
                
                break;
                
            case jmp_:
                
                if (labels_offsets[p_begin + *(uint32_t*)(p + 1)] == 0) {
                    second_translating = true;
                }
                
                jump(jmp, labels_offsets[p_begin + *(uint32_t*)(p + 1)]);
                
                break;
                
#define DEF_JMP(name, num, op)                                              \
                                                                            \
            case name##_:                                                   \
                if (labels_offsets[p_begin + *(uint32_t*)(p + 1)] == 0) {   \
                    second_translating = true;                              \
                }                                                           \
                jump(name, labels_offsets[p_begin + *(uint32_t*)(p + 1)]);  \
                break;
                
                #include "jumps.h"
                
#undef DEF_JUMP
                
        }
        
        p += my_asm_codes_size[*p]; // сдвиг указателя к следующей команде(на размер текущей команды)
    }
    
    command(program_end);
    
}

void assembling(char* buff, size_t buff_size, uint32_t offset, char* p, char* p_end,
                my_map &labels, bool &second_assembling, my_map &calls) {
    
    int32_t  n = 0;
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
        
        if (*p == ':') {
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
            
            command(pop_r9);
            command(pop_r10);
            command(add_r9_r10);
            command(push_r9);
            
        } else if (!strcmp(name_of_command, "sub")) {
            
            command(pop_r9);
            command(pop_r10);
            command(sub_r10_r9);
            command(push_r10);
            
        } else if (!strcmp(name_of_command, "mul")) {
            
            command(mov_r15_rax);
            command(mov_r14_rdx);
            
            command(pop_rax);
            command(pop_r9);
            command(mul_r9);
            command(mov_r9_0x64);
            command(cdq);
            command(div_r9);
            command(push_rax);
            
            command(mov_rax_r15);
            command(mov_rdx_r14);
            
        } else if (!strcmp(name_of_command, "div")) {
            
            command(mov_r15_rax);
            command(mov_r14_rdx);
            
            command(pop_r10);
            command(pop_rax);
            command(mov_r9_0x64);
            command(mul_r9);
            command(cdq);
            command(div_r10);
            command(push_rax);
            
            command(mov_rax_r15);
            command(mov_rdx_r14);
            
        } else if (!strcmp(name_of_command, "sqrt")) {
            
            command(mov_r15_rax);
            command(mov_r14_rbx);
            command(mov_r13_rcx);
            command(mov_r12_rdx);
            command(mov_r11_rsi);
            
            command(xor_rdx);         ///
            command(pop_rax);         ///
            command(mov_r9_0x64);     ///
            command(mul_r9);          ///
            command(xor_rbx);         ///
            command(bsr_ecx_eax);     ///
            command(and_cl_0xFE);     ///
            command(mov_edx_0x1);     ///
            command(shl_edx_cl);      ///
            
            command(mov_esi_ebx);     ///<<-------------@
            command(add_esi_edx);     ///               |
            command(cmp_esi_eax);     ///               |
            command_p(ja_byte, 0x08); ///>>--------@    |
            command(sub_eax_esi);     ///          |    |
            command(shr_ebx);         ///          |    |
            command(add_ebx_edx);     ///          |    |
            command_p(jmp_byte, 0x02);///>>---@    |    |
                                      ///     |    |    |
            command(shr_ebx);         ///<<---+----@    |
                                      ///     |         |
            command(shr_edx_0x2);     ///<<---@         |
            command_p(jne_byte, 0xE9);///>>-------------@
            
            command(push_rbx);        ///
            
            command(mov_rax_r15);
            command(mov_rbx_r14);
            command(mov_rcx_r13);
            command(mov_rdx_r12);
            command(mov_rsi_r11);
            
        } else if (!strcmp(name_of_command, "call")) {
            
            scan_label_and_check();
            command(save_ret);
            jump(call, labels[name_of_command]);
            command(rest_ret);
            
        } else if (!strcmp(name_of_command, "ret")) {
            
            command(push_rdi);
            command(ret);
            
        } else if (!strcmp(name_of_command, "in")) {
            
            jump(call, offsets::scan_offset);
            command(push_r9);
            
        } else if (!strcmp(name_of_command, "out")) {
            
            command(pop_r9);
            jump(call, offsets::print_offset);
            
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

void find_calls_and_labels_translating(char* p, char* p_end, Hash_table<char*, bool> &calls, Hash_table<char*, bool> &labels,
                                       Hash_table<char, int> &my_asm_codes_size) {
    char* p_begin = p;
    p += sizeof(Signature);
    
    while (p < p_end) {
        
        if (*p == call_) {
            calls[p_begin + *(uint32_t*)(p + 1)] = true;
        }
        
        if (jmp_ <= *p && *p <= jne_) {
            labels[p_begin + *(uint32_t*)(p + 1)] = true;
        }
        
        p += my_asm_codes_size[*p];
    }
    
}

void find_calls_assembling(char* p, char* p_end, my_map &calls) {
    
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

size_t get_buffer(FILE* in, char* *r_buffer) {

    if (in == nullptr) {
        throw runtime_error("File pointer is null");
    }

    fseek(in, 0, SEEK_END);
    size_t input_file_size = ftell(in);
    rewind(in);

    char* buffer = reinterpret_cast<char*>(calloc(input_file_size + 2, sizeof(char)));
    if (buffer == nullptr) {
        throw runtime_error("Calloc did not allocate memory");
        return 0;
    }

    *r_buffer = buffer;

    size_t buffer_size = fread(buffer, sizeof(char), input_file_size, in);
    if (buffer_size != input_file_size) {
        cout << input_file_size << " " << buffer_size << endl;
        *r_buffer = nullptr;
        throw runtime_error("Size of file and size of buffer are not equal");
        return 0;
    }

    return buffer_size;
}
