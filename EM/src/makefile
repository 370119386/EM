TARGET = main.o
ALL:$(TARGET) ECHO

ECHO:
	@echo "compile $(TARGET) succeed ..."

$(TARGET):main.cc
	$(CC) $(CPPFLAGS) -c $< -o $(OBJ_DIR)/$@
	
