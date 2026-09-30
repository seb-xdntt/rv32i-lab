.section .text.init
.globl _start

_start:
    addi x1, x0, 15
    addi x2, x0, -5

    add  x3, x1, x2
    add  x4, x3, x1

    sub  x5, x4, x1
    sub  x6, x5, x3

    addi x0, x0, 99
    add  x0, x1, x2
    addi x7, x0, 0

    li   t0, 1
    la   t1, tohost
    sw   t0, 0(t1)