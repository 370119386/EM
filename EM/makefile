CC = g++
CPPFLAGS = -std=c++11
OBJ := main.o
TOP_DIR := $(PWD)
BUILD_DIR := $(TOP_DIR)/build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
LIB_DIR := $(BUILD_DIR)/lib
BIN := test
SRC := src
SUB_DIR := Common src
INCLUDE := src Common
INCLUDE_FLAGS =
LD_FLAGS := 
# 
#	Plugins/WorldServerPlugin 

export CC SRC INCLUDE OBJ TOP_DIR OBJ_DIR BIN_DIR LIB_DIR BIN CPPFLAGS LIBS

all:CHECKDIR $(SUB_DIR) GAME_SERVER

CHECKDIR:
	mkdir -p $(SUB_DIR) $(BIN_DIR) $(OBJ_DIR) $(LIB_DIR) $(INCLUDE) $(BUILD_DIR)
$(SUB_DIR):ECHO
	make -C $@
ECHO:
	@echo start compile $(SUB_DIR) ...

GAME_SERVER:
	$(CC) $(addprefix -I ./,$(wildcard $(INCLUDE))) $(OBJ_DIR)/*.o -L lib $(addprefix -l ,$(wildcard $(LIBS))) -o $(BIN_DIR)/game_server

clean:
	rm -rf $(BUILD_DIR)