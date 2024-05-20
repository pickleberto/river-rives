game_name = rives-raid
cartridge = $(game_name).sqfs
rivemuexec = rivemu -quiet -no-window -sdk -workspace -exec
entry = 0-entry.sh
SPRITES := $(wildcard sprites/*.png)
SOURCES := $(wildcard sf/*.c)
HEADERS := $(wildcard sf/*.h)

play: $(cartridge)
	rivemu $(cartridge)

$(cartridge): $(SOURCES) $(HEADERS) $(SPRITES) |$(entry)
	$(rivemuexec) riv-mksqfs $(SOURCES) $(HEADERS) $(SPRITES) $(entry) $(cartridge) -comp xz

$(entry):
	echo "riv-jit-c main.c" > $(entry)
	chmod +x $(entry)

clean:
	rm $(cartridge) $(entry)