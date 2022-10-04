#
# This file is part of linux-clear-prjc.
#
# Developed as a helper to build custom kernels.
# This product includes software developed by Soham Nandy @ DPSRPK 
# (soham.nandy2006@gmail.com).
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

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
OBJS=../build/filehelper.o ../build/patcher.o 

../build/configurator.o: $(OBJS) configurator.c configurator.h
	$(CC) $(CXXFLAGS) $(USE) -c configurator.c $(OBJS) -o ../build/configurator.o

../build/filehelper.o: filehelper.c filehelper.h
	$(CC) filehelper.c -c -o ../build/filehelper.o

../build/patcher.o: patcher.c patcher.h
	$(CC) $(PATCHES) patcher.c -c -o ../build/patcher.o

