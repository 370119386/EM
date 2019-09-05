OBJECTS_DIR = ../bin
OBJECTS = $(addprefix $(OBJECTS_DIR)/, $(patsubst %.cc, %.o, $(wildcard *.cc)))
CC = g++
CFLAGS = -I ../ -std=c++11
TARGET = plugin_loader

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(OBJECTS_DIR)/$(TARGET)

$(OBJECTS_DIR)/%.o:%.cc
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJECTS_DIR)/$(TARGET) $(OBJECTS)