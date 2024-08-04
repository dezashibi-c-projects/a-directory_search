// ***************************************************************************************
//    Project: Multi Threaded Directory Search in C
//    File: utils.c
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

#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool file_contains_word(FILE* file, const char* word)
{
    char line[1024];
    bool found = false;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strstr(line, word) != NULL)
        {
            found = true;
            break;
        }
    }

    return found;
}
