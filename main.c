// id3 tag parser and editor

#include <stdio.h>
#include <stdlib.h>

#include "id3header.h"
#include "id3frameheader.h"
#include "id3textdata.h"
#include "audioinfo.h"
#include "help.h"

const char HELP_TEXT[] = \
    "=================================================  \n"
    "  Welcome to dm tag editor, \n"
    "=================================================  \n"
    "   \n"
    "Args:  \n"
    "   -f <filepath>      Path to an audio file.   \n"
    "   -d <dump type>     Dump data to file for more help type -h -d.   \n"
    "   -v <>              Prints the program version.   \n"
    "   \n"
    "Editing Args"
    "   -a <'New Name'>    Change Artist name.   \n"
    "   -al <'New Name'>   Change Album name.   \n"
    "   -s <'New Name'>    Change Song name.   \n"
    "   -w <>              Write Changes to disk.   \n"
    ;
const char PRINT_AUDIO_INFO_TEXT[] = \
    "Song: %s   \n"
    "Album: %s  \n"
    "Artist: %s \n"
    "   \n"
    "To dump album art use:     \n"
    "   tagedit -f <file> -d art    \n"
    ;

int parseFile(char* filePath, AudioInfo* outInfo)
{
    printf("Parsing file: %s    \n", filePath);

    // open file and load the first 10 bytes (tag header).
    FILE* currentFile = fopen(filePath, "r");

    Id3Header tagHeader;
    AudioInfo songInfo;
    
    int tagSize;
    char* framesBuffer;

    // error checking.
    if(!currentFile)
    {
        printf("Error reading file. \n");
        return 0;
    }
    fread(&tagHeader, sizeof(Id3Header), 1, currentFile);

    // if the first 3 bytes of the file are not "ID3".
    if (strcmp("ID3", tagHeader.identefier) == 1)
    {
        printf("Error could not find a tag. \n");
        return 0;
    }
    // converts the sync-safe int to a normal int.
    tagSize = getId3TagHeaderSize(&tagHeader);

    printf("found ID3 tag version: %d.%d    \n", tagHeader.version[0], tagHeader.version[1]);
    printf("header size is: %d bytes.   \n", tagSize + 10);

    // alocating and setting a buffer with the tag data - 10 bytes(header size).
    framesBuffer = malloc(tagSize);
    fread(framesBuffer, tagSize, 1, currentFile);  // reading data to the buffer.

    // iterate through the headers and add the info to audioinfo
    int idx = 0;
    while (idx < tagSize)
    {
        // TODO: add pointers to the headers for editing and change the way this is done.
        Id3FrameHeader* frameHeader = &framesBuffer[idx];
        // Song name
        if (strcmp(frameHeader->identifier, "TIT2") == 0){
            Id3TextData* text = &framesBuffer[idx + 10];

            getStringFromBuffer(text, &songInfo.name, 0, getId3FrameHeaderSize(frameHeader));

        }
        // Song Artist.
        if (strcmp(frameHeader->identifier, "TPE1") == 0){
            Id3TextData* text = &framesBuffer[idx + 10];

            getStringFromBuffer(text, &songInfo.artist, 0, getId3FrameHeaderSize(frameHeader));

        }
        // Song Album.
        if (strcmp(frameHeader->identifier, "TALB") == 0){
            Id3TextData* text = &framesBuffer[idx + 10];

            getStringFromBuffer(text, &songInfo.album, 0, getId3FrameHeaderSize(frameHeader));

        }
        if (strcmp(frameHeader->identifier, "APIC") == 0){
            printf("Found image of type: Cover  \n");

            break;
        }
        // Set to the beginning of next frame.
        idx += (getId3FrameHeaderSize(frameHeader) + 10);
    }

    *outInfo = songInfo;

    return 1;
}

int main(int argc, char* argv[])
{
    char* filePath = NULL;
    AudioInfo info;

    // geting command line args.
    for (size_t i = 0; i < argc; i++)
    {
        if(strcmp("-f", argv[i]) == 0)
        {
            filePath = argv[i + 1];
        }
        if(strcmp("-h", argv[i]) == 0)
        {
            showHelp();
        }
        if(strcmp("-v", argv[i]) == 0)
        {
            printf("Version pre alpha.  \n");
            return;
        }
    }

    if (filePath == NULL)
    {
        printf("No file provided.    \n");
        return -1;
    }

    if (!parseFile(filePath, &info))
    {
        printf("Unable to parse file.");
        return -1;
    }

    printf(PRINT_AUDIO_INFO_TEXT, info.name, info.album, info.artist);

    return 0;
}
