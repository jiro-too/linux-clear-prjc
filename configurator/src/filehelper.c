
/**
 * @file filehelper.c
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
#include <string.h>
#include <assert.h>
#include <fcntl.h> 
#include <unistd.h>

#include <zlib.h>
#include "configurator.h"
#include <signal.h>

#define CHUNK 32768
static ssize_t processed;

/**
 * @brief Hard exits process with passed signal. 
 * 
 * @param signal signal to exit with
 */
static void 
interrupt_handler(int signal)
{
    eprintf("Caught termination signal %d\n Processed blocks %zd",signal,processed);
    _exit(signal);
}

/**
 * @brief Check if a file exists.
 * 
 * @param argc number of arguments
 * @param ... files to check for
 * @return int -1 if error, otherwise 0
 */

int 
file_check(
        int argc,
        ...)
{
    va_list argv;
    va_start(argv,argc);

    for (int i=0;i<argc;++i)
    {
        char *path = va_arg(argv,char*);
        if (access(path,F_OK) !=0){
            return -1;
        }
    }
    return 0;
}

/**
 * @brief Copies file from source to destination
 * 
 * @param source Path of file to copy
 * @param destination Path where to copy 
 * @return int 0 on success
 */
int 
file_copy (char *source, char *destination)
{
   
    int fs;
    
    fs = open(source,O_RDONLY); //just exists to create file if not exist
                                //
    if (file_check(1,source) != 0){
        _vprintf("Couldnt access files trying to copy existing config.\n");
        exit(1);
    }

    _vprintf("Opened file for reading\n");
    FILE *fd = fopen(destination,"w+");
    FILE *fs_read = fopen(source,"r+");
    _vprintf("Opening file for writing\n");


    char buf[8192] = {};
    long n;
    
    while (read(fs,buf,sizeof(buf))>0){
        fwrite(buf,1,sizeof(buf),fd); 
    }
    _vprintf("[+] Data written to %s\n",destination);
    fflush(fd);
    fclose(fd);
    return 0;
}

/**
 * @brief Reimplimentation of ZCAT
 * 
 * @param input File to read from
 * @param output File to output to (stdout works)
 * 
 */
int 
zcat_impl (FILE *input,FILE *output)
{
    z_stream strm = {};
    int ret = inflateInit2(&strm,16+MAX_WBITS);
    assert(ret == Z_OK);

    unsigned char in[CHUNK];
     for (;;) {
        // Keep reading until the end of the input file or an error.
        if (strm.avail_in == 0) 
        {
            strm.avail_in = fread(in, 1, CHUNK, input);
            if (strm.avail_in == 0)
                break;
            strm.next_in = in;
        }

        // Decompress all of what's in the input buffer.
        do {
            fflush(stdout);

            signal(SIGINT,
                    (void (*)(int))interrupt_handler); //typecast

            // Decompress as much as possible to the CHUNK output buffer.
            unsigned char out[CHUNK];
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);

            // Write to the output file whatever inflate() left in the output
            // buffer. Return with an error if the write does not complete.
            size_t got = CHUNK - strm.avail_out;
            size_t put = fwrite(out, 1, got, output);
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
            processed+=got;
            // Continue until everything in the input buffer is consumed.
        } while (strm.avail_in > 0);
    }

    // Successfully decompressed all of the input file. Clean up and return.
    assert(
            inflateEnd(&strm) == Z_OK
        );

    return ret;
}
