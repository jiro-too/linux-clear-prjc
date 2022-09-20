#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
void patch_kernel(char *kernel_dir,char *patch_path)
{
  char command[1024] = {};
  sprintf(command,"cd %s && patch -p1 < %s",kernel_dir,patch_path);
  system(command);
}
