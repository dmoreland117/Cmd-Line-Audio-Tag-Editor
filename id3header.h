#ifndef ID3_HEADER_H
#define ID3_HEADER_H

typedef struct {
    char identefier[3];
    char version[2];
    char flags;
    char size[4];
} Id3Header;

int getId3TagHeaderSize(Id3Header* header)
{
    return header->size[0] << 21 | header->size[1] << 14 | header->size[2] << 7 | header->size[3];
}

#endif