OBJECTS=EMDynLib.o PluginLoader.o PluginManager.o
CC=g++
CFLAGS=-I ../ -std=c++11

plugin_loader: $(OBJECTS)
	$(CC) $(CFLAGS) -o plugin_loader $(OBJECTS)

EMDynLib.o: EMDynLib.h EMDynLib.cc
	$(CC) $(CFLAGS) -c EMDynLib.cc

PluginLoader.o: PluginLoader.cc
	$(CC) $(CFLAGS) -c PluginLoader.cc

PluginManager.o: PluginManager.h PluginManager.cc
	$(CC) $(CFLAGS) -c PluginManager.cc

clean:
	rm $(OBJECTS) plugin_loader

