game_name = rives-raid
cartridge = $(game_name).sqfs
rivemuexec = rivemu -quiet -no-window -sdk -workspace -exec
entry = 0-entry.sh
SPRITES := $(wildcard sprites/*.png)
SOURCES := $(wildcard src/*.c)
HEADERS := $(wildcard src/*.h)
C_FILES := $(patsubst src/%.c,%.c,$(wildcard src/*.c))

play: $(cartridge)
	rivemu $(cartridge)

$(cartridge): $(SOURCES) $(HEADERS) $(SPRITES) |$(entry)
	$(rivemuexec) riv-mksqfs $(SOURCES) $(HEADERS) $(SPRITES) $(entry) $(cartridge) -comp xz

$(entry):
	echo "riv-jit-c $(C_FILES)" > $(entry)
	chmod +x $(entry)

clean:
	rm $(cartridge) $(entry)