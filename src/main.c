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

#include "cthreads.h"

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

#define MAX_THREADS 15
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

struct cthreads_mutex mutex;

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
void* search_in_file_thread(void* arg)
{
    ThreadData* data = arg; // cast arg to ThreadData*

    // lock the thread to access the data
    cthreads_mutex_lock(&mutex);
    FILE* file = fopen(data->file_path, "r");
    if (!file)
    {
        fprintf(stderr, "'fopen' call error for file '%s' in 'search_in_file_thread' from '%s':%d\n", data->file_path, __FILE__, (__LINE__ - 3));
        return NULL;
    }

    if (file_contains_word(file, data->keyword))
    {
        printf("Found keyword in file: %s\n", data->file_path);
    }
    cthreads_mutex_unlock(&mutex); // finished, unlock the thread

    free(data);   // free the data
    fclose(file); // close the file

    return NULL;
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

    struct cthreads_thread threads[MAX_THREADS];
    struct cthreads_args thread_args[MAX_THREADS];

    int thread_count = 0;

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
                ThreadData* data = malloc(sizeof(ThreadData));
                strncpy(data->file_path, path, PATH_MAX);
                data->keyword = keyword;

                // create new thread and increment thread_count by one
                if (cthreads_thread_create(&threads[thread_count++], NULL, search_in_file_thread, data, &thread_args[thread_count]) != 0)
                {
                    fprintf(stderr, "'cthreads_thread_create' call error in 'search_in_directory' from '%s':%d\n", __FILE__, (__LINE__ - 2));
                }

                // wait for the threads to join and reset the thread_count
                if (thread_count >= MAX_THREADS)
                {
                    for (int i = 0; i < thread_count; ++i)
                    {
                        cthreads_thread_join(threads[i], NULL);
                    }

                    thread_count = 0;
                }
            }
        }
    }

    closedir(d);

    // wait for the remained thread to join
    for (int i = 0; i < thread_count; ++i)
    {
        cthreads_thread_join(threads[i], NULL);
    }
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

    cthreads_mutex_init(&mutex, NULL);

    search_in_directory(dirpath, keyword);

    cthreads_mutex_destroy(&mutex);

    return 0;
}