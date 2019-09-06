TARGET = Common.so
HEADERS=$(wildcard *.h)
OBJECTS=$(addprefix $(OBJ_DIR)/,$(patsubst %.cc,%.o,$(wildcard *.cc)))
LIBS += $(TARGET)

ALL:$(TARGET) ECHO

ECHO:
	@echo "compile $(TARGET) succeed ..."

$(TARGET):$(OBJECTS)
	$(CC) $(CPPFLAGS) -shared $(OBJECTS) -o $(LIB_DIR)/$(TARGET)

$(OBJ_DIR)/%.o:%.cc
	$(CC) $(CPPFLAGS) -c $< -o $@
	
