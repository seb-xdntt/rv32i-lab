OUTPUT_ARCH("riscv")
ENTRY(_start)

MEMORY
{
  RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 16M
}

SECTIONS
{
  .text : {
    *(.text.init)
    *(.text)
    *(.text.*)
  } > RAM

  . = ALIGN(0x1000);
  .tohost : {
    tohost = .;
    *(.tohost)
    . += 8;
    fromhost = .;
    *(.fromhost)
    . += 8;
  } > RAM

  .data : {
    *(.data)
    *(.data.*)
  } > RAM

  .bss : {
    *(.bss)
    *(.bss.*)
  } > RAM
}