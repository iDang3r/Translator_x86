# Translator_x86
compiles my assembler language to Mac's Mach-o(64) executable file

uses Mach-o files only with:
1. Mach64 header
1. Load Commands
  1. Segment64 : __PAGEZERO
  1. Segment64 : __TEXT
    1. Section64 : __text
  1. Segment64 : __DATA
    1. Section64 : __data
  1. UNIXTHREAD
1. Section64 (__TEXT, __text)
  1. Assembly
1. Section64 (__DATA, __data)

Пример кода на ассемблере см. в example_asm.txt
Присутствует поддержка передачи аргументов функций через регистры (ax, bx, cx и dx) и через стек в (почти) неограниченном количестве
данный язык ассемблера - Тьюринг-полный
