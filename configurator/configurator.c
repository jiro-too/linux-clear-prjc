#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#include <zlib.h>

#include <unistd.h>

/* 
 * Re implementation of zcat to grab kernel config details*
*/

#define SET_BINARY_MODE(file)
#define CHUNK 16384
#define bool int 
#define true 1
#define false 0

bool USE_ZCAT = true;
extern int zcat_impl(FILE *input,FILE *output);

int main(int argc,char **argv)
{
  uid_t uid=getuid();
  if (uid!=0){
    printf("not root\n");
  }
  SET_BINARY_MODE(stdio);
  SET_BINARY_MODE(stdout);

  int opt;
  char *path = malloc(1024*2*sizeof(char));
  path = "/proc/config.gz";
  char *out_path = malloc(1024*sizeof(char));

  while ((opt = getopt(argc,argv,"i:hg:"))){
    switch(opt){
      case 'h':
        
  printf("clear-prjc configurator\n\
  -i        Kernel config location \n\
  -g        Sets flag to auto-extract kernel config from /proc/config.gz\n\
  -h        Show this message\n");
        return 0;

      case 'i':
        printf("Kernel config argument found: %s\n",optarg);
        USE_ZCAT = false;
        break;
      case ':': 
          printf("Option -i requires a value to proceed.\n"); 
          return 0;  
      
      case '?':
          printf("Unrecognized option\n");
          return 0;

  #ifdef CUSTOM_GZ_FILE
      case 'g':
          printf("Custom gz file location overriden to : %s\n",optarg);
          path = optarg;
          out_path = "./config";
  #endif
    }
    break;
  }

  if (USE_ZCAT){
    FILE *inFile = fopen(path,"rb");
    FILE *outFile = fopen(out_path,"w+");

    char *extensionMark = strstr( path, ".gz" );

    if( extensionMark == NULL )
    {
        fprintf(stderr, "File %s is not in .gz format\n",path);
        return 1;
    }

    if( *extensionMark != path[strlen(path)-3] )
    {
        fprintf(stderr, "File is not in .gz format\n");
        return 2;
    }

    inFile = fopen(path, "rb");

    if( inFile == NULL )
    {
        fprintf(stderr, "Cannot open file %s\n", path);
        return 3;
    }

    zcat_impl(inFile,outFile);
  }
}


