CC = g++
CPPFLAGS = -std=c++11 -fPIC
OBJ := main.o
TOP_DIR := $(PWD)
BUILD_DIR := $(TOP_DIR)/build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
LIB_DIR := $(BUILD_DIR)/lib
BIN := test
SRC := src
SUB_DIR := Common src Plugins/WorldServerPlugin
INCLUDE := src Common
INCLUDE_FLAGS =
LD_FLAGS := 
COMMON_OBJECTS = $(OBJ_DIR)/EMDynLib.o $(OBJ_DIR)/PluginManager.o
DL_APPEND := .so
# 
#	Plugins/WorldServerPlugin 

export CC SRC INCLUDE OBJ TOP_DIR OBJ_DIR BIN_DIR LIB_DIR BIN CPPFLAGS LIBS COMMON_OBJECTS DL_APPEND

all:CHECKDIR $(SUB_DIR) GAME_SERVER

CHECKDIR:
	mkdir -p $(SUB_DIR) $(BIN_DIR) $(OBJ_DIR) $(LIB_DIR) $(INCLUDE) $(BUILD_DIR)
$(SUB_DIR):ECHO
	make -C $@
ECHO:
	@echo start compile $(SUB_DIR) ...

GAME_SERVER:
	$(CC) $(CPPFLAGS) $(addprefix -I ./,$(wildcard $(INCLUDE))) $(OBJ_DIR)/main.o $(COMMON_OBJECTS) -ldl -o $(BIN_DIR)/game_server

clean:
	rm -rf $(BUILD_DIR)
