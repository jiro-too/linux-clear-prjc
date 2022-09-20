#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include<fcntl.h> 
#include <unistd.h>

#include <zlib.h>
#include "configurator.h"
#include <signal.h>

#define CHUNK 32768

void interrupt_handler(int signal){
    printf("\n\nCaught signal %d. Terminating\n\n",signal);
    _exit(signal);
}



int file_copy(char *source, char *destination)
{
    int fs;
    
    fs = open(source,O_RDONLY);
    printf("[+] Opened file for reading\n");
    FILE *fd = fopen(destination,"w+");
    printf("[+] Opening file for writing\n");


    char buf[8192];
    long n;
    
    while ((n=read(fs,buf,(long)sizeof(buf)))> 0){
        fwrite(buf,1,sizeof(buf),fd); 
    }
    printf("[+] Data written to %s\n",destination);
    return 0;
}
int zcat_impl (FILE *input,FILE *output)
{
    z_stream strm = {};
    int ret = inflateInit2(&strm,16+MAX_WBITS);
    assert(ret == Z_OK);

    unsigned char in[CHUNK];
     for (;;) {
        // Keep reading until the end of the input file or an error.
        if (strm.avail_in == 0) {
            strm.avail_in = fread(in, 1, CHUNK, input);
            if (strm.avail_in == 0)
                break;
            strm.next_in = in;
        }

        // Decompress all of what's in the input buffer.
        do {
            fflush(stdout);

            signal(SIGINT,(void (*)(int))interrupt_handler);
            // Decompress as much as possible to the CHUNK output buffer.
            unsigned char out[CHUNK];
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);

            // Write to the output file whatever inflate() left in the output
            // buffer. Return with an error if the write does not complete.
            size_t got = CHUNK - strm.avail_out;
            size_t put = fwrite(out, 1, got, stdout);
            if (put != got)
                return Z_ERRNO;

            // Check for the end of a gzip member, in which case reset inflate
            // for the next gzip member.
            if (ret == Z_STREAM_END)
                assert(inflateReset(&strm) == Z_OK);

            // Return on a data error.
            else if (ret != Z_OK) {
                assert(ret == Z_DATA_ERROR);
                (void)inflateEnd(&strm);
                return ret;
            }

            // Continue until everything in the input buffer is consumed.
        } while (strm.avail_in > 0);
    }

    // Successfully decompressed all of the input file. Clean up and return.
    assert(inflateEnd(&strm) == Z_OK);
    return ret;
}