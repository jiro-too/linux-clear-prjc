#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "configurator.h"
#include "patcher.h"



char *kver;

void patch_kernel(char *kernel_dir,char *patch_path)
{
  char command[1024] = {};
  sprintf(command,"patch -d /%s -p1 < %s",kernel_dir,patch_path);

  int ret =system(command);
}

void patch_prompt(char *kernel_path)
{
  fprintf(stdout,(
"Enter kernel version to patch:\n\t\
1)5.19\n"
          ));

  int opt = input();
  switch (opt){
    case 1:
      kver="5.19";
  }
  
#ifdef CPU_SCHEDULER_PDS
  printf("\nAlternative CPU schedulers. Recommended for gaming.[0/1]\n");
  opt=input();
  if (opt){
    patch_kernel(kernel_path,"../linux-patches/5.19/projectc.patch");
    patch_kernel(kernel_path,"../linux-patches/5.19/0003-glitched-base.patch");
    printf("Patch applied successfully\n");
  }
#endif
#ifdef ALLOW_SYSCTL_DISALLOW_USER_PRIVELAGE
  printf("\nPatch kernel to allow sysctl to disallow unprivelaged CLONE_NEWUSER by default? [0/1]\n");
  opt = input();
  if (opt){
    patch_kernel(kernel_path,"../linux-patches/5.19/allow-sysctl-unprivelaged-user-namespace-clone.patch");
    printf("Patch applied successfully\n");
  }
#endif

#ifdef CLEAR_PATCHES
  printf("\nPatch kernel with Xanmod and intel Clear patches. Recommended for gaming [0/1]\n");
  opt=input();
  if (opt){
    patch_kernel(kernel_path,"../linux-patches/5.19/0001-mm-Support-soft-dirty-flag-reset-for-VA-range.patch");
    patch_kernel(kernel_path,"../linux-patches/5.19/0002-clear-patches.patch");
    patch_kernel(kernel_path,"../linux-patches/5.19/0002-mm-Support-soft-dirty-flag-read-with-reset.patch");
    printf("Patch applied successfully\n");
    
  }
#endif
  
#ifdef ADD_ACS_OVERRIDES_IOMMU_PATCH
  printf("\nAdd acs-overrides-iommu patches. Recommended [0/1]\n");
  opt=input();
  if (opt){
    patch_kernel(kernel_path,"../linux-patches/5.19/0006-add-acs-overrides_iommu.patch");
    printf("Patch applied successfully\n");
  }
#endif 

#ifdef FSYNC_VIA_FUTEX
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
  printf("\nAdd option to support Winesync in the kernel. [0/1]\n");
  opt=input();
  if (opt){
    patch_kernel(kernel_path,"../linux-patches/5.19/0007-v5.19-winesync.patch");
    printf("Patch applied successfully\n");
  }
#endif

#ifdef SUPPORT_LRU
  printf(("\
\nclearing the accessed bit in a PTE usually triggers a page fault in\n\
non x86/x86_64 or arm64 architectures. Recommended if you dont have arm64 or x86 [0/1]\n"
        ));
  opt=input();
  if(opt){
    patch_kernel(kernel_path,"../linux-patches/5.19/0010-lru_5.19.patch");
    printf("Patch applied successfully\n");
  }
#endif
#ifdef MISC_PATCHES
  printf("\nMisc patches? [0/1]\n");
  opt=input();
  if (opt){
    patch_kernel(kernel_path,"../linux-patches/5.19/0012-misc-additions.patch");
    printf("Patch applied successfully\n");
  }
#endif
}
