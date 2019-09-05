DEPT_OBJECTS=PluginManager.o EMDynLib.o

OBJECTS_DIR = ../../bin
OBJECTS=$(addprefix $(OBJECTS_DIR)/,$(patsubst %.cc,%.o,$(wildcard *.cc)) $(DEPT_OBJECTS))
CC=g++
INCLUDE_PATH=../../
CFLAGS=-std=c++11
OUT_DIR=../../lib
TARGET=world_server_plugin.so

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -I $(INCLUDE_PATH) -shared $(OBJECTS) -o $(OUT_DIR)/$(TARGET)

$(OBJECTS_DIR)/%.o:%.cc
	$(CC) $(CFLAGS) -I $(INCLUDE_PATH) -c $< -o $@

clean:
	@rm -f $(OUT_DIR)/$(TARGET) $(OBJECTS)