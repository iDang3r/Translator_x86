# Translator_x86
1) compiles my assembler language to Mac's Mach-o(64) executable file (use flag -asm)

2) translates my_CPU_code to Mac's Mach-o(64) executable file

To get more information use flag -help

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

Пример кода на ассемблере см. в example_asm.txt(сгенерирован автоматически)

Формат входных 

Присутствует поддержка передачи аргументов функций через регистры (ax, bx, cx и dx) и/или через стек
Обращение к памяти поддерживается по индексу или по смещению(only ax) + индекс

Данный язык ассемблера - Тьюринг-полный

Ускорение, полученное относительно моего эмулятора CPU:

Emulate CPU | x86_CPU
------------|--------
28.4867 sec.| 0.000326 sec.

Ускорение: 87 тысяч раз
