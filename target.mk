include tools/setup.mk

TARGET := tiny
MCU := atmega328p
BUILD_DIR := ./build
DEVICE_PACK := Microchip.ATmega_DFP.2.3.126

# dwdebug or avrdude
UPLOAD_TYPE := dwdebug
AVRDUDE_PROGRAMMER_TYPE := atmelice_isp
AVRDUDE_PROGRAMMER_ARGS :=
DWDEBUG_DEVICE ?= ttyUSB0
BLOOM_DEBUG_TOOL := atmel-ice
BLOOM_DEBUG_INTERFACE := debug-wire
BLOOM_INSIGHT_ENABLED := false

DEFINES := \
  F_CPU=8000000UL \

# http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p&LOW=E2&HIGH=9E&EXTENDED=FF&LOCKBIT=FF
FUSES := \
  lfuse=0xE2 \
  hfuse=0x9E \
  efuse=0xFF \

include tools/defaults.mk

CXXFLAGS += \
  -Wno-effc++ \

CPPFLAGS += \
  -flto \
  -fwhole-program \

SRC_DIRS := \
  src \
  src/hardware \
  lib/cpp/src \

SYS_INC_DIRS += \
  lib/cpp/include \

tiny_cpp_EXTERNAL_SYS_INC_DIRS:=lib/cpp/include
include lib/tiny.cpp/lib_tiny_cpp.mk

.PHONY: all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex
	@$(SIZE) $<

include tools/tools.mk
