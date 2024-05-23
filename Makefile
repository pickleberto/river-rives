game_name = river-rives
game = $(game_name).elf
cartridge = $(game_name).sqfs
cartridge_sdk = $(game_name)-sdk.sqfs
rivemuexec = rivemu -quiet -no-window -sdk -workspace -exec
entry = 0-entry.sh
info_json = info.json
SPRITES := $(wildcard sprites/*.png)
SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)
big_main = sf/main.c
big_header = sf/headers.h
SF_DIR = sf

OBJDIR := objs
OBJS := $(patsubst src/%.c,$(OBJDIR)/%.o,$(wildcard src/*.c))

play: $(cartridge)
	rivemu $(cartridge)

$(cartridge): $(big_main) $(big_header) $(SPRITES) |$(entry)
	$(rivemuexec) riv-mksqfs $(big_main) $(big_header) $(info_json) $(SPRITES) $(entry) $(cartridge) -comp xz

$(entry):
	echo "riv-jit-c main.c" > $(entry)
	chmod +x $(entry)

$(big_main): $(SOURCES) |$(SF_DIR)
	cat >> $(big_main) $$(find src -name '*.c')
	sed -i 's/\#include \"\(.*\)\"//g' $(big_main)
	sed -i 's/\#include <riv.h>//g' $(big_main)
	sed  -i '1i \#include \"headers.h\"' $(big_main)

$(big_header): $(HEADERS)|$(SF_DIR)
	cat >> $(big_header) src/score.h
	cat >> $(big_header) src/level.h
	cat >> $(big_header) $$(find src -name '*.h' ! -name '*level.h' ! -name '*score.h')
	sed -i 's/\#pragma once//g' $(big_header)
	sed -i 's/\#include \"\(.*\)\"//g' $(big_header)
	sed -i 's/\#include <riv.h>//g' $(big_header)
	sed  -i '1i \#include <riv.h>' $(big_header)

$(SF_DIR):
	mkdir sf

clean:
	rm $(cartridge) $(entry)
	rm -r $(SF_DIR)

web:
	mkdir sf
	cat >> sf/main.c $$(find src -name '*.c')
	cat >> sf/headers.h src/score.h
	cat >> sf/headers.h src/level.h
	cat >> sf/headers.h $$(find src -name '*.h' ! -name '*level.h' ! -name '*score.h')
	sed -i 's/\#pragma once//g' sf/headers.h
	sed -i 's/\#include \"\(.*\)\"//g' sf/headers.h
	sed -i 's/\#include <riv.h>//g' sf/headers.h
	sed -i 's/\#include \"\(.*\)\"//g' sf/main.c
	sed -i 's/\#include <riv.h>//g' sf/main.c
	sed  -i '1i \#include <riv.h>' sf/headers.h
	sed  -i '1i \#include \"headers.h\"' sf/main.c

sdk: $(cartridge_sdk)
	rivemu -sdk $(cartridge_sdk)

$(game): $(OBJS)
	$(rivemuexec) 'gcc -o $(game) $(OBJS) $$(riv-opt-flags -Ospeed)'
	$(rivemuexec) riv-strip $(game)

$(cartridge_sdk): $(game) $(SPRITES)
	$(rivemuexec) riv-mksqfs $(game) $(SPRITES) $(cartridge_sdk) -comp xz

$(OBJDIR)/%.o : src/%.c
	$(rivemuexec) 'gcc $< -o $@ -c'

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

clean-sdk:
	rm -r $(OBJDIR)
	rm $(cartridge_sdk) $(game)