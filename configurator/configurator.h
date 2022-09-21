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

void int_handler();
bool get_proc_gz();
long input();
void make_kernel();
int interactive_session();
