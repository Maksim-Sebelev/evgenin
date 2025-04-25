#include <stdlib.h>
#include "fileread/fileread.hpp"
#include "log/log.hpp"

int main()
{
    printf("Start\n");
    OPEN_LOG();

    const char* file1 = "tests/test1.txt";

    
    WordArray words = ReadBufferFromFile(file1);
    
    LOG_PRINT(Red, "First Test Result\n\n");

    LOG_PRINT(Blue, "size = '%lu'\n", words.size);
    
    for (size_t i = 0; i < words.size; i++)
    {
        Word word = words.words[i];
        LOG_PRINT(Yellow, "word[%1lu] = \n{", i);
        LOG_PRINT(Green, "\tint word = '%f'\n\tlen = %lu\n\t%s:%lu:%lu\n", WordToDouble(&word), word.len, file1, word.line, word.inLine);
        LOG_PRINT(Yellow, "}\n\n");
    }


    BufferDtor(&words);
        
    //-----------------------------------------------------
    LOG_NS(); LOG_NS(); LOG_NS();
    //-----------------------------------------------------

    const char* file2 = "tests/test2.txt";

    words = ReadBufferFromFile(file2);
    
    LOG_PRINT(Red, "Second Test Result\n\n");

    LOG_PRINT(Blue, "size = '%lu'\n", words.size);
    
    for (size_t i = 0; i < words.size; i++)
    {
        Word word = words.words[i];
        LOG_PRINT(Yellow, "word[%1lu] = \n{", i);
        LOG_PRINT(Green, "\tword = '%s'\n\tlen = %lu\n\t%s:%lu:%lu\n", word.word, word.len, file2, word.line, word.inLine);
        LOG_PRINT(Yellow, "}\n\n");
    }


    BufferDtor(&words);

    CLOSE_LOG();

    printf("\nEnd\n");
    return EXIT_SUCCESS;
}
