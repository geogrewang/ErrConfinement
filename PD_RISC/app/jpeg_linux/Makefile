# PLEASE DEFINE THE BASE DIRECTORY OF YOUR MODEL HERE:
MODEL   = ../../

LISATEK = $(shell lmake --ltinst)
VERSION = $(shell lmake --ver)
TOOLS   = $(MODEL)/$(VERSION)/bin/

LCC     = $(TOOLS)/lcc
CC      = $(TOOLS)/lcc-clang
INCLUDE =
CFLAGS  = -g -v
#CFLAGS  = -O3 -fomit-frame-pointer 
#CFLAGS = -g -O3
LDFLAGS = -lm -v
MCFG 	= $(MODEL)/hdl_gen/hdl_memory_configuration.txt

AS      = $(TOOLS)/lasm
LD      = $(TOOLS)/llnk
SIM_DIR = $(MODEL)/contrib/pdriscrun

#PLEASE ADD INCLUDE AND CFLAGS TO YOUR COMMAND

# PLEASE ADD LDFLAGS TO YOUR LINKER COMMAND

BIN  = dct.out
OBJS = dct.o

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)
%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $(INCLUDE) $<

%.s: %.c
	$(CC) -S -o $@ $(CFLAGS) $(INCLUDE) $<

hdlmem: $(BIN) $(MCFG)
	exe2txt -e dct.out -i $(MCFG)
	mv contents_prog_meml.mmap contents_prog_mem.mmap
	$(MODEL)/contrib/utils/split contents_prog_mem.mmap contents_prog_meml.mmap contents_prog_memh.mmap

clean:
	rm -f *.o
	rm -f *~
	rm -f $(BIN)

distclean:
	rm -f *.o
	rm -f *~

debug:
	pdbg -a $(MODEL)$(VERSION)/lib/architecture.so -e $(BIN)
run:
	make -C $(SIM_DIR)
	$(SIM_DIR)/run $(BIN)
