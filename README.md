# Translator_x86
compiles my assembler language to Mac's Mach-o(64) executable file

uses Mach-o files only with:
- Mach64 header
- Load Commands
  - Segment64 : __PAGEZERO
  - Segment64 : __TEXT
  - Section64 : __text
  - Segment64 : __DATA
  - Section64 : __data
  - UNIXTHREAD
- Section64 (__TEXT, __text)
  - Assembly
- Section64 (__DATA, __data)

Пример кода на ассемблере см. в example_asm.txt

Присутствует поддержка передачи аргументов функций через регистры (ax, bx, cx и dx) и/или через стек

данный язык ассемблера - Тьюринг-полный
