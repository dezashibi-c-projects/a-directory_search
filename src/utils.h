// ***************************************************************************************
//    Project: Multi Threaded Directory Search in C
//    File: utils.h
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

#ifndef UTILS__H__
#define UTILS__H__

#include <stdbool.h>
#include <stdio.h>

/**
 * @brief checks whether the file contains the given word or not
 *
 * It returns as soon as it finds the first occurrence of the word
 *
 * @param file FILE* pointer to the file
 * @param word the word to search in file for
 *
 * @return true or false if it finds the word in the given file stream
 */
bool file_contains_word(FILE* file, const char* word);

#endif // UTILS__H__
