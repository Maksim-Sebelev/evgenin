#include <stdio.h>
#include <malloc.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "fileread/fileread.hpp"
#include "log/log.hpp"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Pointer
{
    size_t wp; // word pointer
    size_t lp; // line pointer
    size_t sp; // str pointer (char pos in line)
    size_t bp; // buffer pointer
    size_t wbp; // previous word begin pointer
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static size_t CalcFileLen      (const char* const fileName);
static void SetNullWord(Word** split_buffer, const char* buffer);
static void SetPreviousWordLen(Word* split_buffer, const Pointer* pointer, size_t wordLen);
static void SetWordAndFilePosition(Word* split_buffer, const char* word, Pointer* pointer);

static size_t GetPreviousWordLen(const Pointer* pointer);
static bool   IsPassSymbol     (const char c, Pointer* pointer);
static void   Fread            (char* buffer, size_t bufferLen, FILE* filePtr);
static void   ReadBufRealloc   (WordArray* wordArray);
static bool   IsInt            (const char* const str, const char* const strEnd);
static bool   IsSpace          (const char c);
static bool   IsSlashN         (const char c);
static bool   IsSlashR         (const char c);
static bool   IsSlashT(const char c);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int strtoi(const char* const str)
{
    assert(str);

    long res = 0;
    char* strEnd = NULL;
    res = strtol(str, &strEnd, 10);

    if (!IsInt(str, strEnd))
    {
        printf("\n\n'%s' - IS NOT A NUMBER.\n\n", str);
        assert(0 && "try to convert not int str to int.");
    }

    return (int) res;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static bool IsInt(const char* const str, const char* const strEnd)
{
    assert(str);
    assert(strEnd);

    return (int) strlen(str) == (strEnd - str);
}

//============================ Read File ==============================================================================================================

WordArray ReadBufferFromFile(const char* file)
{
    assert(file);

    FILE* filePtr = fopen(file, "rb");

    if (!filePtr)
    {
        fprintf(stderr, "failed open '%s'.\n", file);
        return {};
    }

    size_t bufferLen     = CalcFileLen(file);
    size_t realBufferLen = bufferLen + 2;

    char* buffer = (char*) calloc (realBufferLen, sizeof(*buffer));
    Word* words  = (Word*) calloc (realBufferLen, sizeof(*words));

    assert(buffer);
    assert(words);

    Fread(buffer, bufferLen, filePtr);
    fclose(filePtr);

    buffer[bufferLen    ] = ' ';
    buffer[bufferLen + 1] = '\0';


    SetNullWord(&words, buffer);

    Pointer pointer = 
    {
        .wp  = 0,
        .lp  = 1,
        .sp  = 1,
        .bp  = 0,
        .wbp = 0,
    };

    SetWordAndFilePosition(words, &buffer[pointer.bp], &pointer);

    for (; pointer.bp < realBufferLen; pointer.bp++)
    {
        if (!IsPassSymbol(buffer[pointer.bp], &pointer))
        {
            pointer.sp++;
            continue;
        }

        size_t previousWordLen = GetPreviousWordLen(&pointer);

        do
        {
            buffer[pointer.bp] = '\0';  
            pointer.bp++;
        } while (IsPassSymbol(buffer[pointer.bp], &pointer));
        
        SetPreviousWordLen(words, &pointer, previousWordLen);
        SetWordAndFilePosition(words, &buffer[pointer.bp], &pointer);

        pointer.sp++;
        pointer.wbp = pointer.bp;
    }

    size_t wordsQuant = pointer.wp - 1;

    WordArray wordArray = {words, wordsQuant};

    ReadBufRealloc(&wordArray);

    return wordArray;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static size_t GetPreviousWordLen(const Pointer* pointer)
{
    assert(pointer);

    return pointer->bp - pointer->wbp;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void BufferDtor(WordArray* wordArray)
{
    assert(wordArray);
    assert(wordArray->words);

    wordArray->size = 0;

    wordArray->words--;
    assert(wordArray->words);

    FREE(wordArray->words[0].word);
    FREE(wordArray->words);

    return;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void Fread(char* buffer, size_t bufferLen, FILE* filePtr)
{   
    assert(buffer);
    assert(filePtr);

    size_t freadReturn = fread(buffer, sizeof(char), bufferLen, filePtr);
    assert(freadReturn == bufferLen);
    return;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void ReadBufRealloc(WordArray* wordArray)
{
    assert(wordArray);
    assert(wordArray->words);

    wordArray->words--;
    assert(wordArray->words);

    Word*  words = wordArray->words;
    size_t size  = wordArray->size + 1;

    wordArray->words = (Word*) realloc(words, size * sizeof(*words));
    assert(wordArray->words);

    wordArray->words++;

    assert(wordArray->words);
    return;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static size_t CalcFileLen(const char* const fileName)
{
    assert(fileName);
    struct stat buf = {};
    stat(fileName, &buf);
    return (size_t) buf.st_size;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void SetWordAndFilePosition(Word* split_buffer, const char* word, Pointer* pointer)
{
    assert(split_buffer);
    assert(pointer);

    size_t wordPointer = pointer->wp;

    split_buffer[wordPointer].word   = word;
    split_buffer[wordPointer].line   = pointer->lp;
    split_buffer[wordPointer].inLine = pointer->sp;

    pointer->wp++;

    return;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void SetPreviousWordLen(Word* split_buffer, const Pointer* pointer, size_t wordLen)
{
    assert(split_buffer);
    assert(pointer);
    assert(pointer->wp >= 1);

    size_t wordPointer = pointer->wp - 1;

    split_buffer[wordPointer].len = wordLen;

    return;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void SetNullWord(Word** split_buffer, const char* buffer)
{
    assert(split_buffer);

    split_buffer[0]->word = buffer;
    (*split_buffer)++; //

    return;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static bool IsPassSymbol(const char c, Pointer* pointer)
{
    assert(pointer);

    if (IsSpace (c))
    {
        pointer->sp++;
        return true;
    }

    if (IsSlashN(c)) 
    {
        pointer->lp++;
        pointer->sp = 1;
        return true;
    }

    if (IsSlashT(c))
    {
        pointer->sp += 4;
        return true;
    }

    if (IsSlashR(c))
    {
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static bool IsSpace(const char c)
{
    return (c == ' ');
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static bool IsSlashN(const char c)
{
    return (c == '\n');
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static bool IsSlashR(const char c)
{
    return (c == '\r');
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static bool IsSlashT(const char c)
{
    return (c == '\t');
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
