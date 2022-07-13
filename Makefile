TARGET = puzzle

MAC = 1
DEBUG = 0
OPT = -O2

BUILD_DIR = build

C_SOURCES = \
Src/main.c \
Src/graph.c \
Src/util.c \

CC = gcc
AS = gcc -x assembler-with-cpp
CP = objcopy
SZ = size

C_DEFS =

C_INCLUDES =

CFLAGS = $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -Werror

ifeq ($(DEBUG), 1)
CFLAGS += -g
endif

CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LIBS = -lpthread
ifeq ($(MAC), 1)
LDFLAGS = $(LIBS)
ASFLAGS =
else
LDFLAGS = $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref
ASFLAGS = -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst))
endif

all: $(BUILD_DIR)/$(TARGET)

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR):
	mkdir $@

clean:
	-rm -fR $(BUILD_DIR)

run: $(BUILD_DIR)/$(TARGET)
	cd $(BUILD_DIR) && ./$(TARGET)

-include $(wildcard $(BUILD_DIR)/*.d)