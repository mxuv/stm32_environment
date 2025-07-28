.PHONY: all debug clean

-include config.mk

SRC += $(wildcard src/*.c)
SRC += $(wildcard src/hal/*.c)
SRC += $(wildcard src/sys/*.c)
SRC += $(wildcard src/sys/drivers/*.c)
SRC += $(wildcard src/sys/hardware/*.c)
SRC += $(wildcard src/sys/lib/*.c)
SRC += $(wildcard src/sys/os/*.c)
INCS = -Isrc \
	   -Isrc/cmsis \
	   -Isrc/config \
	   -Isrc/hal \
	   -Isrc/startup \
	   -Isrc/sys \
	   -Isrc/sys/drivers \
	   -Isrc/sys/hardware \
	   -Isrc/sys/lib \
	   -Isrc/sys/os

SRS += $(wildcard src/startup/*.s)
OBJS = $(SRC:.c=.o) $(SRS:.s=.o)
DEPS = $(OBJS:.o=.d)

debug: CFLAGS += $(WFLAGS) $(GFLAGS)
debug: $(DEBUG_NAME).elf $(DEBUG_NAME).bin $(DEBUG_NAME).hex $(DEBUG_NAME).lss

release: $(RELEASE_NAME).elf $(RELEASE_NAME).bin \
	$(RELEASE_NAME).hex $(RELEASE_NAME).lss

$(DEBUG_NAME).bin: $(DEBUG_NAME).elf
	$(OBJCOPY) -O binary $< $@

$(DEBUG_NAME).hex: $(DEBUG_NAME).elf
	$(OBJCOPY) -O ihex $< $@

$(DEBUG_NAME).lss: $(DEBUG_NAME).elf
	$(OBJDUMP) -h -S $< > $@

$(RELEASE_NAME).bin: $(RELEASE_NAME).elf
	$(OBJCOPY) -O binary $< $@

$(RELEASE_NAME).hex: $(RELEASE_NAME).elf
	$(OBJCOPY) -O ihex $< $@

$(RELEASE_NAME).lss: $(RELEASE_NAME).elf
	$(OBJDUMP) -h -S $< > $@

$(DEBUG_NAME).elf: $(OBJS)
	$(CC) $(OFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(RELEASE_NAME).elf: $(OBJS)
	$(CC) $(OFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(INCS) $(OFLAGS)  $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(INCS) $(CFLAGS) $(OFLAGS) $(ASFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) \
		$(DEBUG_NAME).elf \
		$(DEBUG_NAME).bin \
		$(DEBUG_NAME).hex \
		$(DEBUG_NAME).lss \
		$(RELEASE_NAME).elf \
		$(RELEASE_NAME).bin \
		$(RELEASE_NAME).hex \
		$(RELEASE_NAME).lss
