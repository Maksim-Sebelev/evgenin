#include <stdio.h>
#include <malloc.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "onegin.h"

static char   GetArrElem    (char* arr, size_t elemIndex);
static char*  GetArrElemPtr (char* arr, size_t elemIndex);
static size_t CalcFileLen   (const char* FileName);
static void   SetLine       (char** split_buffer, size_t* word_i, char* setLine);
static bool   IsPassSymbol  (char c);

//---------------------------------------------------------------------------------------------------------------------------------------------------

int strtint(char* str)
{
    long res = 0;
    char* strEnd = nullptr;
    res = strtol(str, &strEnd, 10);

    assert(strlen(str) == (size_t) (strEnd - str)); // проверка на то, вся ли строка является числом

    return (int) res;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

char** ReadBufferFromFile(const char* file, size_t* bufSize)
{
    FILE* filePtr = fopen(file, "rb");
    assert(filePtr);

    size_t bufferLen = CalcFileLen(file);

    char*  buffer       = (char*)   calloc(bufferLen + 1, sizeof(char));
    char** split_buffer = (char**)  calloc(bufferLen + 1, sizeof(char*));

    fread(buffer, sizeof(char), bufferLen + 1, filePtr);
    buffer[bufferLen] = '\0';

    fclose(filePtr);

    char bufElem = GetArrElem(buffer, 0);

    assert(bufElem != ' ');
    assert(bufElem != '\n');

    bufElem = GetArrElem(buffer, bufferLen - 1);

    assert(bufElem == '\n');

    size_t word_i = 0;

    SetLine(split_buffer, &word_i, GetArrElemPtr(buffer, 0));

    for (size_t buffer_i = 0; buffer_i <= bufferLen; buffer_i++)
    {
        bufElem = GetArrElem(buffer, buffer_i);
        if (IsPassSymbol(bufElem))
        {
            while (IsPassSymbol(bufElem) && buffer_i <= bufferLen)
            {
                buffer[buffer_i] = '\0';
                buffer_i++;
                bufElem = GetArrElem(buffer, buffer_i);
            }
    
            SetLine(split_buffer, &word_i, GetArrElemPtr(buffer, buffer_i));
        }
    }

    *bufSize = word_i - 1;

    split_buffer = (char**) realloc(split_buffer, (*bufSize) * sizeof(char*));

    assert(split_buffer);
    assert(*split_buffer);

    assert(split_buffer);
    return split_buffer;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

void BufferDtor(char** buffer)
{
    assert(buffer);

    FREE(*buffer)
    FREE(buffer);

    assert(!buffer);

    return;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

static size_t CalcFileLen(const char* FileName)
{
    struct stat buf = {};
    stat(FileName, &buf);
    return (size_t) buf.st_size;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

static char GetArrElem(char* arr, size_t elemIndex)
{
    assert(arr);
    return arr[elemIndex];
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

static char* GetArrElemPtr(char* arr, size_t elemIndex)
{
    assert(arr);
    return &arr[elemIndex];
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

static void SetLine(char** split_buffer, size_t* word_i, char* setLine)
{
    assert(split_buffer);
    assert(setLine);
    assert(word_i);

    split_buffer[*word_i] = setLine;
    (*word_i)++;
    return;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

static bool IsPassSymbol(char c)
{
    return (c == ' ') || (c == '\n');
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
