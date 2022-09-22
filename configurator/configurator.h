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

#define ERROR_PREMSG "[\033[0;31m*\033[0;37m]"
// _vprintf is used to print verbose output

void eprintf(char *fmt,...);
void _vprintf(char *fmt,...);

long input();
static void make_kernel();
