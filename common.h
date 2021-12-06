/* 
 * Operating Systems (2INCO) Practical Assignment
 * Interprocess Communication
 *
 * Contains definitions which are commonly used by the farmer and the workers
 *
 */

#include "uint128.h"

#ifndef COMMON_H
#define COMMON_H

// Maximum size for any message in the tests
#define MAX_MESSAGE_LENGTH 6

// TODO: put your definitions of the datastructures here
typedef struct
{

    // define data structure of the request message
    char START_CHAR_ALPHABET; // first letter of alphabet taken from settings.h
    char END_CHAR_ALPHABET;   // last letter of alphabet taken from settings.h

    char first_letter;  // first letter of the hash assigned by the farmer
    uint128_t md5_hash; // hash of the string to find

} MQ_REQUEST_MESSAGE;

typedef struct
{
    //define data structure of the response message
    char match[MAX_MESSAGE_LENGTH + 1]; // matched hash

} MQ_RESPONSE_MESSAGE;

#endif
