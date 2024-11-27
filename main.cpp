#include "onegin.h"

int main()
{
    const char* file1 = "test1.txt";
    
    size_t buf1Size = 0;
    const char** buffer1 = ReadBufferFromFile(file1, &buf1Size);

    for (size_t i = 0; i < buf1Size; i++)
    {
        printf("buf1[%2lu]: '%s'\n", i, buffer1[i]);
    }

    BufferDtor(buffer1);
    //-----------------------------------------------------
    printf("\n\n");
    //-----------------------------------------------------


    const char* file2 = "test2.txt";

    size_t buf2Size = 0;
    const char** buffer2 = ReadBufferFromFile(file2, &buf2Size);

    for (size_t i = 0; i < buf2Size; i++)
    {
        printf("buf2[%2lu]: '%d'\n", i, strtoi(buffer2[i]));
    }

    BufferDtor(buffer2);

    return 0;
}
