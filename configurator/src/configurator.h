/**
 * @file configurator.h
 * @author Soham Nandy (you@domain.com)
 * @brief This file is part of linux-clear-prjc.
 * @version 1
 * @date 2022-09-25
Developed as a helper to build custom kernels.
This product includes software developed by Soham Nandy 
 
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * @copyright Soham Nandy (c) 2022
 * 
 */
#ifndef configurator_h
#define configurator_h
#endif

#define SET_BINARY_MODE(file)
#define bool int 
#define true 1
#define false 0
//Signals
#define SIGHUP  1   /* Hangup the process */ 
#define SIGINT  2   /* Interrupt the process */ 
#define SIGQUIT 3   /* Quit the process */ 
#define SIGILL  4   /* Illegal instruction. */ 
#define SIGTRAP 5   /* Trace trap. */ 
#define SIGABRT 6   /* Abort. */

#define ERROR_PREMSG " [\033[0;31m*\033[0;37m] "
#define VERBOSE_PREMSG " [\033[0;33m*\033[0;37m] "


#define SUCCESS 0x0000000
#define ERROR 0x0000001
#define PERMISSION_ERROR 0x000000B
#define NO_EXIST -0x00000001
#define GZ_DATA_ERROR -0x0000003

void eprintf(char *fmt,...);
void _vprintf(char *fmt,...);

long input();
char* g_kver();
static void make_kernel();


