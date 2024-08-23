// ***************************************************************************************
//    Project: Multi Threaded Directory Search in C
//    File: main.c
//    Date: 2024-08-04
//    Author: Navid Dezashibi
//    Contact: navid@dezashibi.com
//    Website: https://www.dezashibi.com | https://github.com/dezashibi
//    License:
//     Please refer to the LICENSE file, repository or website for more information about
//     the licensing of this work. If you have any questions or concerns,
//     please feel free to contact me at the email address provided above.
// ***************************************************************************************
// *  Description: Read the readme file for more information
// ***************************************************************************************

#define DTHREAD_IMPL
#include "../dthreads/dthread.h"

#if defined(_WIN32) || defined(_WIN64)
#include "dirent.h"
#define PATH_SEP "\\"
#else
#include <dirent.h>
#include <sys/stat.h>
#define PATH_SEP "/"
#endif

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////
/////// CONSTANTS
///////////////////////////////////////////////////////////////////////////////////////////

#define MAX_THREADS 8
#define INITIAL_BUFFER_SIZE 128

///////////////////////////////////////////////////////////////////////////////////////////
/////// GLOBALS and DATA STRUCTURES
///////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief structure to hold data needed for each thread
 */
typedef struct
{
    char file_path[PATH_MAX];
    const char* keyword;
} ThreadData;

DThread threads[MAX_THREADS];
ThreadData* data_pool[MAX_THREADS];
int thread_count = 0;

///////////////////////////////////////////////////////////////////////////////////////////
/////// FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief This is the function passed to each thread
 *
 * Searches the file for the keyword
 *
 * @param arg a void* to the ThreadData
 */
dthread_define_routine(search_in_file_thread)
{
    ThreadData* thread_data = data; // cast data to ThreadData*

    FILE* file = fopen(thread_data->file_path, "r");
    if (!file)
    {
        fprintf(stderr, "'fopen' call error for file '%s' in 'search_in_file_thread' from '%s':%d\n", thread_data->file_path, __FILE__, (__LINE__ - 3));
        return NULL;
    }

    if (file_contains_word(file, thread_data->keyword))
    {
        printf("Found keyword in file: %s\n", thread_data->file_path);
    }

    fclose(file); // close the file

    return NULL;
}

/**
 * @brief Joins all the threads and frees used data then resets the count
 */
void join_threads_and_reset(void)
{
    for (int i = 0; i < thread_count; ++i)
    {
        dthread_join(&threads[i]);
        free(data_pool[i]);
    }

    thread_count = 0;
}

/**
 * @brief Searches the directory
 *
 * @param dirpath starting point path
 * @param keyword the keyword which must be searched in each file
 */
void search_in_directory(const char* dirpath, const char* keyword)
{
    DIR* d;
    struct dirent* dir;

    // Try to open the directory path
    d = opendir(dirpath);
    if (!d)
    {
        fprintf(stderr, "'opendir' call error for directory '%s' in 'search_in_directory' from '%s':%d\n", dirpath, __FILE__, (__LINE__ - 3));
        return;
    }

    /**
     * We traverse in the current directory and all the children directories
     * Then create threads for each MAX_THREADS number of files
     *
     * If the number of threads reaches the maximum number of threads
     * It must be wait until the previous threads join the main thread the reset the thread_count
     */
    while ((dir = readdir(d)) != NULL)
    {
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
        {
            char path[PATH_MAX];
            snprintf(path, PATH_MAX, "%s" PATH_SEP "%s", dirpath, dir->d_name);

            struct stat statbuf;
            if (stat(path, &statbuf) == -1)
            {
                fprintf(stderr, "'stat' call error for directory '%s' in 'search_in_directory' from '%s':%d\n", path, __FILE__, (__LINE__ - 2));
                continue;
            }

            // if it's a directory call itself recursively
            if (S_ISDIR(statbuf.st_mode))
            {
                search_in_directory(path, keyword);
            }
            else
            {
                data_pool[thread_count] = (ThreadData*)malloc(sizeof(ThreadData));
                strncpy(data_pool[thread_count]->file_path, path, PATH_MAX);
                data_pool[thread_count]->keyword = keyword;

                // create new thread and increment thread_count by one
                threads[thread_count] = dthread_init_thread(search_in_file_thread, data_pool[thread_count]);
                if (dthread_create(&threads[thread_count], NULL) != 0)
                {
                    fprintf(stderr, "'dthread_create' call error in 'search_in_directory' from '%s':%d\n", __FILE__, (__LINE__ - 2));
                }

                ++thread_count;

                // wait for the threads to join and reset the thread_count
                if (thread_count >= MAX_THREADS)
                    join_threads_and_reset();
            }
        }
    }

    closedir(d);

    // wait for the remained thread to join and reset the thread_count
    join_threads_and_reset();
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <directory> <keyword>\n", argv[0]);
        return 1;
    }

    const char* dirpath = argv[1];
    const char* keyword = argv[2];

    search_in_directory(dirpath, keyword);

    return 0;
}
