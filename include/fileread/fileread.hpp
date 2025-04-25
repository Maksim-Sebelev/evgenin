#ifndef ONEGIN_HPP
#define ONEGIN_HPP

#include <stdio.h>
#include <malloc.h>


#define FREE(ptr) free((char*)ptr); ptr = nullptr


// функция ReadBufferFromFile возвращает указатель на массив строк,
// который строится внутри нее. Значение агрумента bufSize после исполнения
// функции ReadBufferFromFile будет равным количеству строк в массиве,
// на который она возвращает указатель.

// strtoi преобразовывает строку в число,
// в случае если не вся строка является числом, программа будет завершена аварийно.

// если флаги компиляции позволяют не освобождать память 
// перед завершением программы, функция BufferDtor может не использоваться
// в протвном случае - BufferDtor должна быть использована.

struct Word
{
    const char* word;
    size_t      len;
    size_t      line;
    size_t      inLine;
};

struct WordArray
{
    Word*  words;
    size_t size;
};

WordArray ReadBufferFromFile(const char* file);

int          strtoi             (const char* const str   );
void         BufferDtor         (WordArray* wordArray);





#endif //ONEGIN_HPP
