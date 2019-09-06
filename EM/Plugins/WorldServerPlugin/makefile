TARGET = WorldServerPlugin.so

HEADERS=$(wildcard *.h)
OBJECTS=$(addprefix $(OBJ_DIR)/,$(patsubst %.cc,%.o,$(wildcard *.cc)))
LIBS += $(TARGET)

ALL:$(TARGET) ECHO

$(TARGET):$(HEADERS) $(OBJECTS)
	$(CC) -I ../../ $(CPPFLAGS) $(OBJECTS) $(COMMON_OBJECTS) -shared -o $(LIB_DIR)/$(TARGET)
$(OBJ_DIR)/%.o : %.cc
	$(CC) -I ../../ $(CPPFLAGS) -c $< -o $@

ECHO:
	@echo "compile $(TARGET) succeed ..."