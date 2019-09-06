TARGET = Common
HEADERS=$(wildcard *.h)
OBJECTS=$(addprefix $(OBJ_DIR)/,$(patsubst %.cc,%.o,$(wildcard *.cc)))
LIBS := $(TARGET)$(DL_APPEND)

ALL:$(TARGET) ECHO

ECHO:
	@echo "compile $(TARGET)$(DL_APPEND) succeed ..."

$(TARGET):$(OBJECTS)
	$(CC) $(CPPFLAGS) -shared $(OBJECTS) -o $(LIB_DIR)/$(TARGET)$(DL_APPEND)

$(OBJ_DIR)/%.o:%.cc
	$(CC) $(CPPFLAGS) -c $< -o $@
	
