MODEL	= ../../
LTINST  = $(shell lmake --ltinst)
VERSION = $(shell lmake --ver)
LCC     = $(MODEL)/$(VERSION)/bin/lcc
CC      = $(MODEL)/$(VERSION)/bin/lcc-clang
AS      = $(MODEL)/$(VERSION)/bin/lasm
# Init Stackbase according to data memory position (See memory_cfg.h for this boundary)
AFLAGS  = -q --defsym _stackbase=0xff0000 \
	--defsym _heapbase=0xc00000 \
	--defsym _heapsize=0xb00000
CFLAGS  = -g
LDFLAGS = -Wl,--entry=0,linker.cmd
MCFG 	= $(MODEL)/hdl_gen/hdl_memory_configuration.txt
VLOG_SIMDIR = $(MODEL)/hdl_gen/verilog/sim_vcs
VHDL_SIMDIR = $(MODEL)/hdl_gen/vhdl/sim_vcs

BIN  = sieve.out

all: $(BIN)

hdl:	hdlmem ref.dmp

$(BIN): sieve.o boot.o
	$(LCC) $(CFLAGS) $(LDFLAGS) -o $@ boot.o $<

sieve.s: sieve.c
	$(CC) $(CFLAGS) -S -o $@ -c $<

sieve.o: sieve.c
	$(CC) $(CFLAGS) -o $@ -c $<

boot.o: startup.s
	$(AS) $(AFLAGS) -o $@ $<

hdlmem: $(BIN) $(MCFG)
	exe2txt -e sieve.out -i $(MCFG)
	mv contents_prog_meml.mmap contents_prog_mem.mmap
	$(MODEL)/contrib/utils/split contents_prog_mem.mmap contents_prog_meml.mmap contents_prog_memh.mmap

ref.dmp: sieve.out $(MODEL)/$(VERSION)/lib/architecture.so
	lvcdgen -o ref.dmp -a ../../$(VERSION)/lib/architecture.so -e sieve.out

vcd:	verilog_vcd

vhdl_vcd:	hdlmem ref.dmp	
	cp -f *.mmap exit.addr $(VHDL_SIMDIR)
	make -C $(VHDL_SIMDIR) vcd
	lvcdcmp -is -file1 ref.dmp -file2 $(VHDL_SIMDIR)/regr.dump

verilog_vcd:	hdlmem ref.dmp	
	cp -f *.mmap exit.addr $(VLOG_SIMDIR)
	make -C $(VLOG_SIMDIR) vcd
	lvcdcmp -is -file1 ref.dmp -file2 $(VLOG_SIMDIR)/regr.dump

distclean:
	rm -f *.o sieve.s *.mmap ref.dmp exit.addr
clean:
	rm -f *.o sieve.s $(BIN) *.mmap ref.dmp exit.addr
debug:
	pdbg -a $(MODEL)$(VERSION)/lib/architecture.so -e $(BIN)
run:
	$(MODEL)/contrib/pdriscrun/run $(BIN)
