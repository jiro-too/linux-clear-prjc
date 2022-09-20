#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include<signal.h>
#include <zlib.h>

#include <unistd.h>
#include "configurator.h"
/* 
 * Re implementation of zcat to grab kernel config details*
*/

extern int zcat_impl(FILE *input,FILE *output);
extern int file_copy(char *source,char *destination);
extern int patch_kernel(char *kernel_dir,char *patchpath);
bool _extraction;
bool _pretend;
bool _custom_config;
bool _use_dist_config;

static const char * program_name = "kernel-configurator";
char *gzed_config_path;
char *final_config_out_path = "../linux/.config";
char *input_opt_path;
char *kernel_path = "../linux";

static void usage(){
    fprintf(stderr, 
"Usage:\n\
-i \tCustom kernel config file location\n"
"-e \tAuto-extracts from /proc/config.gz\n"
"-g \tSame as -e but allows you to specify custom path\n"
"-b \tGets current kernel config from /boot/ directory\n"
"-h \tShows this message\n"
"-p \tPretend option. Dry-run without root access\n [WIP]"
"-o \tChange final .config option output location\n"
"-k \tChange kernel directory\n [WIP]"
"-x \tStarts interactive mode to download kernel from kernel.org and patch it\n\t\
Config file can be specified within interactive session\n [WIP]"
    );
    exit(1);
} 

bool get_proc_gz(){
    FILE *inFile = fopen(gzed_config_path,"rb");
    FILE *outFile = fopen(final_config_out_path,"w+");

    char *extensionMark = strstr( gzed_config_path, ".gz" );

    if( extensionMark == NULL )
    {
        fprintf(stderr, "File %s is not in .gz format\n",gzed_config_path);
        return 1;
    }

    if( *extensionMark != gzed_config_path[strlen(gzed_config_path)-3] )
    {
        fprintf(stderr, "File is not in .gz format\n");
        return 2;
    }

    inFile = fopen(gzed_config_path, "rb");

    if( inFile == NULL )
    {
        fprintf(stderr, "Cannot open file %s\n", gzed_config_path);
        return 3;
    }
    zcat_impl(inFile,outFile);
    return 0;
}


int main(int argc,char **argv)
{
  

if (argc > 1){  
    int opt;
    while ((opt = getopt(argc,argv,"i:o:g:k:ehp")) != EOF){
        switch(opt){
        case 'h':
            usage();

        case 'i':
            _custom_config++;
            printf("Kernel config argument found: %s\n",optarg);
            _extraction = false;
            input_opt_path=optarg;
            continue;
        case 'o':
            printf("Final path change to %s\n",optarg);
            final_config_out_path=optarg;
            continue; 
        case 'k':
            printf("Overriding kernel directory to %s\n",optarg);
            final_config_out_path=optarg;
            continue;
    
#ifdef CUSTOM_GZ_FILE
        case 'g':
            _extraction=1;
            printf("[+] Override config.gz file location to: %s\n",optarg);
            gzed_config_path=optarg;
            continue; 
        
        case 'e':
            _extraction=1;
            printf("Using default /proc/config.gz \n");
            gzed_config_path = "/proc/config.gz";
            get_proc_gz();
            break;
#endif
        }

    fflush(stdout);
    break;
     }
  }
  SET_BINARY_MODE(stdio);
  SET_BINARY_MODE(stdout);
  if (_custom_config){
      file_copy(input_opt_path,final_config_out_path);
  }
  else if (_extraction){
      get_proc_gz();
  }
}

