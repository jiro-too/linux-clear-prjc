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
bool _extraction;
bool _pretend;


static const char * program_name = "kernel-configurator";
char *gzed_config_path;
char *final_config_out_path = "../linux/.config";

static void usage(){
    fprintf(stderr, 
"Usage:\n\
-i \tCustom kernel config file location\n"
"-g \tAuto-extracts config from /proc/config.gz unless specified otherwise with {l} flag\n"
"-b \tGets current kernel config from /boot/ directory\n"
"-h \tShows this message\n"
"-p \tPretend option. Dry-run without root access\n"
"-o \tChange final .config option output location\n"
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
    zcat_impl(inFile,stdout);
    return 0;
}


int main(int argc,char **argv)
{
  

if (argc > 1){  
    int opt;
    while ((opt = getopt(argc,argv,"g:i:b:ph")) != EOF){
        switch(opt){
        case 'h':
            usage();

        case 'i': 
            printf("Kernel config argument found: %s\n",optarg);
            _extraction = false;
            break; 
       
    
#ifdef CUSTOM_GZ_FILE
        case 'g':
           _extraction++;
           if (optarg && *optarg){
            printf("[+] Override config.gz file location to: %s\n",optarg);
            gzed_config_path=optarg;
           }
           else{
               gzed_config_path="/proc/config.gz";
               printf("Using /proc/config.gz\n");
           }
           break;
#endif
        }

    fflush(stdout);
    break;
     }
  }


  
  SET_BINARY_MODE(stdio);
  SET_BINARY_MODE(stdout);

  if (_extraction){
      get_proc_gz();
  }
  file_copy("./configurator.c","./configurator.c.copy");
}


