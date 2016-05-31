# PLEASE DEFINE THE BASE DIRECTORY OF YOUR MODEL HERE:
MODEL   = ../../

LISATEK = $(shell lmake --ltinst)
VERSION = $(shell lmake --ver)
TOOLS   = $(MODEL)/$(VERSION)/bin/

CC      = $(TOOLS)/lcc-clang
INCLUDE =
CFLAGS  = -g
#CFLAGS  = -O3 -fomit-frame-pointer 
#CFLAGS = -g -O3
LDFLAGS =

AS      = $(TOOLS)/lasm
LD      = $(TOOLS)/llnk
SIM_DIR = $(MODEL)/contrib/pdriscrun

#PLEASE ADD INCLUDE AND CFLAGS TO YOUR COMMAND

# PLEASE ADD LDFLAGS TO YOUR LINKER COMMAND

BIN  = hello_world.out
OBJS = main.o

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)
%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $(INCLUDE) $<

%.s: %.c
	$(CC) -S -o $@ $(CFLAGS) $(INCLUDE) $<

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
