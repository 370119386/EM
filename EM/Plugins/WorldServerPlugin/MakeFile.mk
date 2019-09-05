OBJECTS=WorldServerPlugin.o dllmain.o
CC=g++
CFLAGS=-I ../../ -I ../../PluginLoader  -std=c++11
PROJECT_NAME=world_server_plugin.so

$(PROJECT_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -shared -o $(PROJECT_NAME) $(OBJECTS) ../../PluginLoader/PluginManager.o

dllmain.o: dllmain.cc
	$(CC) $(CFLAGS) -c dllmain.cc

WorldServerPlugin.o: WorldServerPlugin.h WorldServerPlugin.cc
	$(CC) $(CFLAGS) -c WorldServerPlugin.cc

clean:
	rm $(OBJECTS) $(PROJECT_NAME)

