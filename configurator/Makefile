CC = gcc
USE=-D CUSTOM_GZ_FILE
PATCHES=-D CPU_SCHEDULER_PDS\
				-D CLEAR_PATCHES\
				-D ALLOW_SYSCTL_DISALLOW_USER_PRIVELAGE\
				-D ADD_ACS_OVERRIDES_IOMMU_PATCH\
				-D FSYNC_VIA_FUTEX\
				-D WINESYNC_SUPPORT\
				-D MISC_PATCHES\
				-D SUPPORT_LRU

CXXFLAGS = -fno-stack-protector -O2 -lz $(USE) -Wno-unused-result
OBJS= build/configurator.o build/filehelper.o build/patcher.o 
configurator: $(OBJS)
	$(CC) $(CXXFLAGS) $(USE) $(OBJS) -o configurator

build/configurator.o: src/configurator.c
	cd src;make

build/patcher.o: src/patcher.c
	cd src;make

build/filehelper.o: src/filehelper.c
	cd src;make

clean:
	rm -rfv build/*.o
	rm -rfv configurator
	find . -exec touch {} \;
