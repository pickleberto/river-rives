game_name = rives-raid
game = $(game_name).elf
cartridge = $(game_name).sqfs
rivemuexec = rivemu -quiet -no-window -sdk -workspace -exec
SPRITES := $(wildcard sprites/*.png)
SOURCES := $(wildcard src/*.c)
HEADERS := $(wildcard src/*.h)

OBJDIR := objs
OBJS := $(patsubst src/%.c,$(OBJDIR)/%.o,$(wildcard src/*.c))

play: $(cartridge)
	rivemu -sdk $(cartridge)

$(game): $(OBJS)
	$(rivemuexec) 'gcc -o $(game) $(OBJS) $$(riv-opt-flags -Ospeed -Osize)'
	$(rivemuexec) riv-strip $(game)

$(cartridge): $(game) $(SPRITES)
	$(rivemuexec) riv-mksqfs $(game) $(SPRITES) $(cartridge) -comp xz

$(OBJDIR)/%.o : src/%.c
	$(rivemuexec) 'gcc $< -o $@ -c $$(riv-opt-flags -Ospeed -Osize)'

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -r $(OBJDIR)
	rm $(cartridge) $(game)