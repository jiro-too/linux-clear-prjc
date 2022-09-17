CC = gcc
USE=CUSTOM_GZ_FILE
CXXFLAGS = -fno-stack-protector -O2 -lz -D $(USE)
configurator: zcat.o configurator.c
	$(CC) $(CXXFLAGS) configurator.c zcat.o -o configurator

zcat.o: zcat.c
	$(CC) zcat.c -c
