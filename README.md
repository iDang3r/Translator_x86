# Translator_x86
compiles my assembler language to Mac's Mach-o(64) executable file

uses Mach-o files only with:
1. Mach64 header
2. Load Commands
  2.1. Segment64 : __PAGEZERO
  2.2. Segment64 : __TEXT
    2.2.1. Section64 : __text
  2.3. Segment64 : __DATA
    2.3.1. Section64 : __data
  2.4. UNIXTHREAD
3. Section64 (__TEXT, __text)
  3.1. Assembly
4. Section64 (__DATA, __data)

Пример кода на ассемблере см. в example_asm.txt
Присутствует поддержка передачи аргументов функций через регистры (ax, bx, cx и dx) и через стек в (почти) неограниченном количестве
данный язык ассемблера - Тьюринг-полный
