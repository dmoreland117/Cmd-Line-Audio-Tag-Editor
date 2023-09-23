#ifndef ID3_Text_DATA_H
#define ID3_Text_DATA_H

#include <stdlib.h>

enum Id3StringEncoding {
    ISO_8859_1,
    UTF_16,
    UTF_16BE,
    UTF_8
};

typedef struct {
    char encoding;
    char data;
} Id3TextData;

/// @brief Gets a string from bytes in a buffer.
/// @param data pointer to the buffer data.
/// @param outStr Pointer to where to store the string.
/// @param from 
/// @param to The length of the buffer.
/// @return the length of the string.
int getStringFromBuffer(Id3TextData* data, char* outStr[], int from, int to)
{
    switch (data->encoding)
    {
    case UTF_16:
        printf("Found String: UTF-16.   \n");
        char* char_ptr = &data->data+2;
        char newString[100];

        int i = 0;
        while (1)
        {
            if ((*char_ptr == 0x00))
            {
                if (char_ptr[1] == 0x00)
                {
                    char* str_Ptr = malloc(i);
                    memcpy(str_Ptr, &newString, i);
                    *outStr = str_Ptr;
                    return i;
                }

                char_ptr++;
            }

            newString[i] = *char_ptr;

            i++;
            char_ptr++;
        }
        break;
    
    default:
        break;
    }
}

#endif