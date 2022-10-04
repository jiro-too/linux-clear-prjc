
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
#include "filehelper.h"
#include <signal.h>
#include <setjmp.h>


#define CHUNK 32768
static ssize_t processed;
static void *terminate_addr = NULL;
/**
 * @brief Hard exits process with passed signal. 
 * 
 * @param signal signal to exit with
 */
static void 
terminate_intr(int signal)
{
    eprintf("\nCaught termination signal %d\n Processed blocks %zd",signal,processed);
    _exit(signal);
}

/**
 * @brief Captures term signals sent to the program.
 * Redirects to terminate_intr
 *
 * @param term_addr Misc option. Defaults to *0x0
 */
static void capture_terminate(jmp_buf term_addr)
{
    terminate_addr = term_addr;
    signal(SIGHUP, terminate_intr);
    signal(SIGINT, terminate_intr);
    signal(SIGPIPE, terminate_intr);
    signal(SIGTERM, terminate_intr);
    signal(SIGUSR1, terminate_intr);
    signal(SIGUSR2, terminate_intr);
}
/**
 * @brief Return all capture handles to system.
 *
 */
static void uncapture_terminate(void)
{
    terminate_addr = NULL;
    signal(SIGHUP, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    signal(SIGPIPE, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    signal(SIGUSR1, SIG_DFL);
    signal(SIGUSR2, SIG_DFL);
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
        if (access(path,F_OK) ==0){
            return SUCCESS;
        }
        else {
            return NO_EXIST;
        }
    }
}

FILE* 
access_file(char *file_name, const char *perms, int code)
{

    int ret = file_check(1,file_name);
    if (ret == SUCCESS)
    {
        assert(ret==SUCCESS);
        _vprintf("Trying to access file %s with perms %s\n", file_name,perms);
        FILE *to_ret = fopen(file_name,perms);
        _vprintf("File pointer at %p\n",to_ret);
        return to_ret;
    }
    else if (ret == NO_EXIST)
    {
        if (code == HARD_EXIST)
        {
            eprintf("HARD EXIT: Critical file %s doesnt exist\n",file_name);
            exit(ERROR);
        }
        else if (code == CREATE_NO_EXIST)
        {
            FILE *to_ret = fopen(file_name,"w+");
            return to_ret;
        }
        else if (code  == SOFT_EXIST)
        {
            return NULL;
        }
        _vprintf("Error in accessing file with path %s\n",file_name);
        return NULL;
    }
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
    fs = open(source,O_RDONLY); 
    FILE *fs_read = access_file(source,"r+",SOFT_EXIST);
    if (fs_read == NULL)
    {
        return NO_EXIST;
    }
    if (file_check(1,source) != SUCCESS){
        _vprintf("Couldnt access files trying copy to existing config.\n");
        return ERROR;
    }

    _vprintf("Opened file \"%s\" at %p for reading\n",source,fs_read);
    // FILE *fd = fopen(destination,"w+");
    // FILE *fs_read = fopen(source,"r+");
    FILE *file_destination = access_file(destination,"w+",CREATE_NO_EXIST);
    _vprintf("Opening file \"%s\" at %p for writing\n",destination,file_destination);


    char buf[8192] = {};
    long n;
    
    while (read(fs,buf,sizeof(buf))>0){
        fwrite(buf,1,sizeof(buf),file_destination); 
    }
    _vprintf("Data written to %s\n",destination);
    fflush(file_destination);
    fclose(file_destination);
    fclose(fs_read);
    return SUCCESS;
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
    capture_terminate(NULL);
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
    
    uncapture_terminate();

    return ret;
}
