#ifndef ONEGIN_H
#define ONEGIN_H

#include <stdio.h>
#include <malloc.h>

// По моему стандарту, файл, обрабатываемый функцией ReadBufferFromFile
// 1) НЕ должен начинаться на пробел или \n;
// 2) ДОЛЖЕН заканчиваться на \n;
// если данные условия не выполняются, программа будет завершена аварийно.

// функция ReadBufferFromFile возвращает указатель на массив строк,
// который строится внутри нее. Значение агрумента bufSize после исполнения
// функции ReadBufferFromFile будет равным количеству строк в массиве,
// на который она возвращает указатель.

// strtint преобразовывает строку в число,
// в случае если не вся строка является числом, программа будет завершена аварийно.

// если флаги компиляции позволяют не освобождать память 
// перед завершением программы, функция BufferDtor может не использоваться
// в протвном случае - BufferDtor должна быть использована.

char** ReadBufferFromFile (const char* file, size_t* bufSize);
int    strtint           (char* str);
void   BufferDtor         (char** buffer);


#define FREE(ptr) free(ptr); ptr = NULL;

#endif
