game_name = river-rives
cartridge = $(game_name).sqfs
rivemuexec = rivemu -quiet -no-window -sdk -workspace -exec
entry = 0-entry.sh
SPRITES := $(wildcard sprites/*.png)
SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)
big_main = sf/main.c
big_header = sf/headers.h
SF_DIR = sf

play: $(cartridge)
	rivemu $(cartridge)

$(cartridge): $(big_main) $(big_header) $(SPRITES) |$(entry)
	$(rivemuexec) riv-mksqfs $(big_main) $(big_header) $(SPRITES) $(entry) $(cartridge) -comp xz

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