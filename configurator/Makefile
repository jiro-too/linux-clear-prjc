CC = gcc
USE=CUSTOM_GZ_FILE
CXXFLAGS = -fno-stack-protector -O2 -lz -D $(USE)
configurator: filehelper.o patcher.o configurator.c 
	$(CC) $(CXXFLAGS) configurator.c filehelper.o patcher.o -o configurator

filehelper.o: filehelper.c
	$(CC) filehelper.c -c

patcher.o: patcher.c
	$(CC) patcher.c -c
clean:
	rm -rf *.o
	rm -f configurator
