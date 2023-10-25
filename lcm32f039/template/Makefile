# ---------------------------------------
# toolchain 
# ---------------------------------------
TOOLCHAIN = arm-none-eabi-

CC = $(TOOLCHAIN)gcc
AS = $(TOOLCHAIN)gcc -x assembler-with-cpp
CP = $(TOOLCHAIN)objcopy
SZ = $(TOOLCHAIN)size

# ---------------------------------------
# paths
# ---------------------------------------
THIS_MAKEFILE := $(lastword $(MAKEFILE_LIST))
TOP := $(shell realpath $(THIS_MAKEFILE))
TOP := $(patsubst %Makefile,%,$(TOP))
CURRENT_DIR := $(shell basename $(TOP))

# ---------------------------------------
# project's makefile
# ---------------------------------------
ifneq ($(wildcard $(TOP)/rules.mk),)
	include $(TOP)/rules.mk
endif

# ---------------------------------------
# target settings
# ---------------------------------------
# project target
TARGET ?= $(shell basename $(CURRENT_DIR))
# debug build?
DEBUG ?= no
# lto?
LTO ?= no
# optimization
OPT ?= -Os
# default jlink device
JLINK_DEVICE ?= LCM32F039
# default jlink port
JLINK_PORT ?= swd
# default flash
flash: flash-jlink
# default erase
erase: erase-jlink

# ---------------------------------------
# project directory
# ---------------------------------------
# cmsis directory
CMSIS_DIR ?= cmsis
# mcu driver directory
DRIVER_DIR ?= driver
# project sources directory 
SRC_C_DIRS ?= $(CURRENT_PATH)

# default linker file
LD_FILES ?= $(CMSIS_DIR)/device/startup/gcc/linker/LCM32F039_FLASH.ld

# default startup assembly file
SRC_S ?= $(CMSIS_DIR)/device/startup/gcc/startup_lcm32f039.s

# default project's sources file
SRC_C ?= $(shell find $(SRC_C_DIRS) -name *.c)

# default project's include file
INC ?= $(shell find $(INC_DIRS) -type d)

# ---------------------------------------
# toolchain flags
# ---------------------------------------
# cpu
CPU = -mcpu=cortex-m0
# fpu
FPU ?= 
# float-abi
FLOAT-ABI ?= -mfloat-abi=soft
# mcu
MCU ?= $(CPU) -mthumb $(FPU) $(FLOAT-ABI) -mabi=aapcs -nostdlib -nostartfiles

# include flags
DEPFLAGS ?=

# compiler flags
CFLAGS += \
	$(MCU) \
  $(OPT) \
  -ggdb \
  -fdata-sections \
  -ffunction-sections \
  -fsingle-precision-constant \
  -fno-strict-aliasing \
  -Wdouble-promotion \
  -Wstrict-prototypes \
  -Wextra \
  -Wfatal-errors \
  -Werror-implicit-function-declaration \
  -Wfloat-equal \
  -Wundef \
  -Wshadow \
  -Wwrite-strings \
  -Wsign-compare \
  -Wmissing-format-attribute \
  -Wunreachable-code \
  -Wcast-align \
	-Wall\
	-fdiagnostics-color

# suppress warning caused by vendor mcu driver
CFLAGS += -Wno-error=cast-align -Wno-error=unused-parameter 

# debug
ifeq ($(strip $(DEBUG)), yes)
CFLAGS += -g -gdwarf-2
endif

# lto
ifeq ($(strip $(LTO)), yes)
CFLAGS += -flto
endif

# linker flags
LDFLAGS += \
	-fshort-enums \
	-Wl,-Map=$@.map \
	-Wl,-cref \
	-Wl,-gc-sections \
	-specs=nosys.specs -specs=nano.specs

# libraries
LIBS += -lgcc -lm -lc -lnosys

# ---------------------------------------
# build rules
# ---------------------------------------
OUTPUT_NAME ?= $(TARGET)
BUILD = .build/$(TARGET)
BIN = .build/_bin/$(TARGET)

MKDIR = mkdir
SED = sed
COPY = cp
RM = rm

CFLAGS  += $(addprefix -I,$(INC))
LDFLAGS += $(CFLAGS)
ASFLAGS += $(CFLAGS)

LD_SCRIPT_FLAG := -Wl,-T,

ifeq ("$(V)","1")
$(info CFLAGS   $(CFLAGS)  ) $(info )
$(info DEPFLAGS $(DEPFLAGS)) $(info )
$(info LDFLAGS  $(LDFLAGS) ) $(info )
$(info ASFLAGS  $(ASFLAGS) ) $(info )
endif

SRC_S := $(SRC_S:.S=.s)

BUILD_OBJ = $(BUILD)/obj
OBJ += $(addprefix $(BUILD_OBJ)/, $(SRC_S:.s=.o))
OBJ += $(addprefix $(BUILD_OBJ)/, $(SRC_C:.c=.o))

.DEFAULT_GOAL := build

build: $(BUILD)/$(OUTPUT_NAME).bin $(BUILD)/$(OUTPUT_NAME).hex size

OBJ_DIRS = $(sort $(dir $(OBJ)))
$(OBJ): | $(OBJ_DIRS)
$(OBJ_DIRS):
	@$(MKDIR) -p $@

$(BUILD)/$(OUTPUT_NAME).elf: $(OBJ)
	@echo Linking: $@
	@$(CC) -o $@ $(LDFLAGS) $(addprefix $(LD_SCRIPT_FLAG), $(LD_FILES)) $^ -Wl,--start-group $(LIBS) -Wl,--end-group

$(BUILD)/$(OUTPUT_NAME).bin: $(BUILD)/$(OUTPUT_NAME).elf
	@echo Creating: $@
	@$(CP) -O binary $^ $@

$(BUILD)/$(OUTPUT_NAME).hex: $(BUILD)/$(OUTPUT_NAME).elf
	@echo Creating: $@
	@$(CP) -O ihex $^ $@

size: $(BUILD)/$(OUTPUT_NAME).elf
	-@echo ''
	@$(SZ) $<
	-@echo ''

linkermap: $(BUILD)/$(OUTPUT_NAME).elf
	@linkermap -v $<.map

clean:
	$(RM) -rf $(BUILD)
	$(RM) -rf $(BIN)

$(BIN):
	@$(MKDIR) -p $@

vpath %.c . $(TOP)
$(BUILD_OBJ)/%.o: %.c
	@echo Compiling: $(notdir $(@:.o=.c))
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c -MD -o $@ $<
	@$(COPY) $(@:.o=.d) $(@:.o=.P); \
	  $(SED) -e 's/#.*//' -e 's/^.*:  *//' -e 's/ *\\$$//' \
	      -e '/^$$/ d' -e 's/$$/ :/' < $(@:.o=.d) >> $(@:.o=.P); \
	  $(RM) $(@:.o=.d)

vpath %.s . $(TOP)
$(BUILD_OBJ)/%.o: %.s
	@echo Compiling: $(notdir $(@:.o=.s))
	@$(CC) -x assembler-with-cpp $(ASFLAGS) -c -o $@ $<

vpath %.S . $(TOP)
$(BUILD_OBJ)/%.o: %.S
	@echo Compiling:  $(notdir $(@:.o=.S))
	@$(CC) -x assembler-with-cpp $(ASFLAGS) -c -o $@ $<

print-%:
	@echo $* = $($*)

rebuild: clean build

all: clean build flash

# ---------------------------------------
# programme rules
# ---------------------------------------
ifeq ($(OS),Windows_NT)
  JLINK = JLink.exe
else
  JLINK = JLink
endif

$(BUILD)/$(TARGET).jlink: $(BUILD)/$(OUTPUT_NAME).hex
	@echo halt > $@
	@echo r >> $@
	@echo loadfile $< >> $@
	@echo r >> $@
	@echo go >> $@
	@echo exit >> $@

flash-jlink: $(BUILD)/$(TARGET).jlink
	$(JLINK) -device $(JLINK_DEVICE) -if $(JLINK_PORT) -JTAGConf -1,-1 -speed auto -CommandFile $<

$(BUILD)/$(TARGET)-erase.jlink:
	@echo halt > $@
	@echo r >> $@
	@echo erase >> $@
	@echo exit >> $@

erase-jlink: $(BUILD)/$(TARGET)-erase.jlink
	$(JLINK) -device $(JLINK_DEVICE) -if $(JLINK_PORT) -JTAGConf -1,-1 -speed auto -CommandFile $<

# #-------------------- Flash with STLink --------------------
# flash-stlink: $(BUILD)/$(OUTPUT_NAME).elf
# 	STM32_Programmer_CLI --connect port=swd --write $< --go

# erase-stlink:
# 	STM32_Programmer_CLI --connect port=swd --erase all

# erase-stflash:
# 	st-flash erase

# #-------------------- Flash with pyocd --------------------
# flash-pyocd: $(BUILD)/$(OUTPUT_NAME).hex
# 	pyocd flash -t $(PYOCD_TARGET) $<
# 	pyocd reset -t $(PYOCD_TARGET)

# erase-pyocd:
# 	pyocd erase -t $(PYOCD_TARGET) -c

.PHONY: all rebuild clean size linkermap flash erase
