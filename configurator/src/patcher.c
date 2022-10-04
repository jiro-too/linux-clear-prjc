/**
 * @file patcher.c
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "configurator.h"
#include "patcher.h"



char *kver;
int k6_0_0 = 0;
static void 
patch_kernel(char *kernel_dir,char *patch_path)
{
  char command[1024] = {};
  sprintf(command,"patch -d /%s -p1 < %s",kernel_dir,patch_path);

  int ret =system(command);
}

void patch_prompt(char *kernel_path)
{
  fprintf(stdout,(
"Enter kernel version to patch:\n\t\
1)5.19\n\t\
2)6.0\n"
          ));

  int opt = input();
  switch (opt){
    case 1:
      kver="5.19";
    case 2:
      kver="6.0";
      printf("\n 6.0 KERNEL WILL BE PATCHED WITH OLDER 5.19 PATCHES THAT STILL WORK\nPATCHES THAT DONT WORK WILL BE SKIPPED");
      k6_0_0++;
  }
  
#ifdef CPU_SCHEDULER_PDS
#pragma message "Custom CPU scheduler patch enabled\n"
  printf("\nAlternative CPU schedulers. Recommended for gaming.[0/1]\n");
  opt=input();
  if (opt){
    char prjc[2048];
    char gitchbase[2048];

    sprintf(prjc,"../linux-patches/%s/projectc.patch",kver);
    sprintf(gitchbase,"../linux-patches/%s/0003-glitched-base.patch",kver);
    patch_kernel(kernel_path,prjc);
    patch_kernel(kernel_path,gitchbase);
    printf("Patch applied successfully\n");
  }
#endif
#ifdef ALLOW_SYSCTL_DISALLOW_USER_PRIVELAGE
#pragma message "ALLOW_SYSCTL_DISALLOW_USER_PRIVELAGE Enabled"
  printf("\nPatch kernel to allow sysctl to disallow unprivelaged CLONE_NEWUSER by default? [0/1]\n");
  opt = input();
  if (opt){
    patch_kernel(kernel_path,"../linux-patches/5.19/allow-sysctl-unprivelaged-user-namespace-clone.patch");
    printf("Patch applied successfully\n");
  }
#endif

#ifdef CLEAR_PATCHES
#pragma message "Clear patches enabled"
  printf("\nPatch kernel with Xanmod and intel Clear patches. Recommended for gaming [0/1]\n");
  opt=input();
  if (opt){
    patch_kernel(kernel_path,"../linux-patches/5.19/0001-mm-Support-soft-dirty-flag-reset-for-VA-range.patch");
    patch_kernel(kernel_path,"../linux-patches/5.19/0002-clear-patches.patch");
    patch_kernel(kernel_path,"../linux-patches/5.19/0002-mm-Support-soft-dirty-flag-read-with-reset.patch");
    printf("Patch applied successfully\n");
    
  }
#endif;


#ifdef FSYNC_VIA_FUTEX
#pragma message "FSYNC_VIA_FUTEX ENABLED"
  printf("\n\
Add option to wait on mulitple futex. Allows old proton versions to keep using fsync\n\
in newer versions of the kernel. Recommended [0/1]\n");

  opt=input();
  if (opt){
    patch_kernel(kernel_path,"../linux-patches/5.19/0007-v5.19-fsync1_via_futex_waitv.patch");
    printf("Patch applied successfully\n");
  }
#endif

#ifdef WINESYNC_SUPPORT
#pragma message "WINESYNC_SUPPORT ENABLED"
  printf("\nAdd option to support Winesync in the kernel. [0/1]\n");
  opt=input();
  if (opt){
    patch_kernel(kernel_path,"../linux-patches/5.19/0007-v5.19-winesync.patch");
    printf("Patch applied successfully\n");
  }
#endif

#ifdef SUPPORT_LRU
#pragma message "SUPPORT_LRU ENABLED"
if (!k6_0_0){
    printf(("\
\nclearing the accessed bit in a PTE usually triggers a page fault in\n\
non x86/x86_64 or arm64 architectures. Recommended if you dont have arm64 or x86 [0/1]\n"
        ));
  opt=input();
  if(opt){
    patch_kernel(kernel_path,"../linux-patches/5.19/0010-lru_5.19.patch");
    printf("Patch applied successfully\n");
  }
}
#endif
#ifdef MISC_PATCHES
#pragma message "MISC_PATCHES ENABLED"
  printf("\nMisc patches? [0/1]\n");
  opt=input();
  if (opt){
    patch_kernel(kernel_path,"../linux-patches/5.19/0012-misc-additions.patch");
    printf("Patch applied successfully\n");
  }
#endif
}
