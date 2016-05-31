MEMORY
{
    PAGE 0: PROG : origin = 0x000000, length = 0x800000, bytes=2
    PAGE 2: DATA : origin = 0x000000, length = 0x400000, bytes=1
}

SECTIONS
{
	.dummy: > PROG {
		*(.boot)
		*(.text)
		}
	.rodata: > DATA
	.data: > DATA
	.bss:  > DATA
	.boot: > PROG { *(.empty) }
	.text: > PROG { *(.empty) }
        .debug_info:      bytes=1
	.debug_abbrev:    bytes=1
        .debug_aranges:   bytes=1
        .debug_macinfo:   bytes=1
        .debug_line:      bytes=1
        .debug_loc:       bytes=1
        .debug_pubnames:  bytes=1
        .debug_pubtypes:  bytes=1
        .debug_str:       bytes=1
        .debug_ranges:    bytes=1
        .debug_frame:     bytes=1
}
