/* linker.ld */
ENTRY(_start)
SECTIONS
{
  . = 0x00100000; /* load at 1 MiB (typischer Ort für Kernel) */
  .text : { *(.multiboot) *(.text) }
  .rodata : { *(.rodata) }
  .data : { *(.data) }
  .bss : { *(.bss) }
}