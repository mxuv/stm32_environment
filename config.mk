# Toolchain options
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
AR = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

# Compile options
# CPU
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mthumb
CFLAGS += -mfloat-abi=soft

# Compiler
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -std=gnu11
CFLAGS += --specs=nano.specs
CFLAGS += -MMD -MP

ASFLAGS += -x assembler-with-cpp
ASFLAGS += -MMD -MP

LDFLAGS += -Tsrc/startup/STM32F030F4PX_FLASH.ld
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -static
LDFLAGS += -Wl,--start-group -lc -lm -Wl,--end-group

# Optimization
OFLAGS = -Os

# Debug level
GFLAGS = -g3

# Warnings
WFLAGS += -Wall 
WFLAGS += -Wextra 
# WFLAGS += -Wpedantic

# Debug defs
DDEFS += -DDEBUG
DDEFS += -DCONSOLE_LOGGING

# Names
DEBUG_NAME = output/stm32env_f030f4p6_debug
RELEASE_NAME = output/stm32env_f030f4p6
