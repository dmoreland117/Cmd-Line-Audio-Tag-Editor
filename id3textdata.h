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
    char data[];
} Id3TextData;

/// @brief Gets a string from bytes in a buffer.
/// @param data pointer to the buffer data.
/// @param outStr Pointer to where to store the string.
/// @param from 
/// @param to The length of the buffer.
/// @return the length of the string.
int getStringFromBuffer(Id3TextData* data, char* outStr[], int from, int to)
{
    char* char_ptr = NULL;
    char newString[100];
    int stringLength = 0;

    switch (data->encoding)
    {
    case UTF_16:
        printf("Found String: UTF-16.   \n");
        char_ptr = &data->data[2];

        while (1)
        {
            if ((*char_ptr == 0x00))
            {
                if (char_ptr[1] == 0x00)
                {
                    char* str_Ptr = malloc(stringLength);
                    memcpy(str_Ptr, &newString, stringLength);
                    *outStr = str_Ptr;
                    return stringLength;
                }

                char_ptr++;
            }

            newString[stringLength] = *char_ptr;

            stringLength++;
            char_ptr++;
        }
        break;

    case ISO_8859_1:
        printf("Found String: UTF-8.   \n");
        char_ptr = &data->data[0];

        while (stringLength < (to - 1))
        {
            newString[stringLength] = *char_ptr;

            stringLength++;
            char_ptr++;
        }
        char* str_Ptr = malloc(stringLength);
        memcpy(str_Ptr, &newString, stringLength);
        *outStr = str_Ptr;
        return stringLength;
        

    default:
        break;
    }
}

#endif