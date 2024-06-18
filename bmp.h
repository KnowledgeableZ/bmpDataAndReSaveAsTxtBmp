#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
} RGBQUAD;

typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;

#pragma pack(pop)

int readBMP(const char *filename, BITMAPFILEHEADER *fileHeader, BITMAPINFOHEADER *infoHeader, RGBQUAD *palette,unsigned char **imageData);
int writeBMP(const char *filename, const BITMAPFILEHEADER *fileHeader, const BITMAPINFOHEADER *infoHeader,
 const RGBQUAD *palette,const unsigned char *imageData);
void bmpDataSaveToTXT(unsigned char *bmppic,  BITMAPINFOHEADER *infoheader,const char*filename);
void accordTXTSaveBMP(const char *src_filename,const BITMAPFILEHEADER *fileHeader, const BITMAPINFOHEADER *infoHeader,
 const RGBQUAD *palette);
#endif
