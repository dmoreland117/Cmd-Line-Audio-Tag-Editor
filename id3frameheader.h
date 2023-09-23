#ifndef ID3_FRAME_HEADER_H
#define ID3_FRAME_HEADER_

typedef struct {
    char identifier[4];
    char size[4];
    short flags;
} Id3FrameHeader;

int getId3FrameHeaderSize(Id3FrameHeader* header)
{
    return header->size[0] << 21 | header->size[1] << 14 | header->size[2] << 7 | header->size[3];
}

#endif