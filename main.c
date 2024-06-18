#include "bmp.h"

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input bmp file> <output bmp file>\n", argv[0]);
        return 1;
    }

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    unsigned char *imageData = NULL;
    char txtName[64];
    sprintf(txtName, "%s_save.txt", argv[1]);//生成目标保存的txt名字
    RGBQUAD *palette = (RGBQUAD *)malloc(256 * sizeof(RGBQUAD));

    // 读取BMP文件
    if (readBMP(argv[1], &fileHeader, &infoHeader,palette, &imageData) != 0) {
        return 1;
    }

    

    // 显示文件头和信息头信息
    printf("*******************文件头*******************************\n");
    printf("File Type: %08d  [bytes reffer:2]\n", fileHeader.bfType);
    printf("File bSize: %08d  [bytes reffer:4]\n", fileHeader.bfSize);
    printf("File bfReserved1: %08d  [bytes reffer:2]\n", fileHeader.bfReserved1);
    printf("File bfReserved2: %08d  [bytes reffer:2]\n", fileHeader.bfReserved2);
    printf("Data Offset: %08d  [bytes reffer:4]\n", fileHeader.bfOffBits);
   

    printf("*******************信息头*******************************\n");
    printf("biSize: %08d [bytes reffer:4]\n", infoHeader.biSize);
    printf("Width: %08d [bytes reffer:4]\n", infoHeader.biWidth);
    printf("Height: %08d [bytes reffer:4] \n", infoHeader.biHeight);

    printf("biPlanes: %08d [bytes reffer:2] \n", infoHeader.biPlanes);
    printf("Bit Count: %08d [bytes reffer:2]\n", infoHeader.biBitCount);

    printf("biCompression: %08d [bytes reffer:4] \n", infoHeader.biCompression);
    printf("Image Size: %08d [bytes reffer:4] \n", infoHeader.biSizeImage);
    printf("biXPelsPerMeter: %08d [bytes reffer:4] \n", infoHeader.biXPelsPerMeter);
    printf("biYPelsPerMeter: %08d [bytes reffer:4] \n", infoHeader.biYPelsPerMeter);
    printf("biClrUsed: %08d [bytes reffer:4] \n", infoHeader.biClrUsed);
    printf("biClrImportant: %08d [bytes reffer:4] \n", infoHeader.biClrImportant);
   

    // 写入BMP文件
    if (writeBMP(argv[2], &fileHeader, &infoHeader,palette,imageData) != 0) {
        free(imageData);
        return 1;
    }
    bmpDataSaveToTXT(imageData,&infoHeader,txtName);

    accordTXTSaveBMP(txtName,&fileHeader, &infoHeader,palette);


    free(palette);
    // 释放图像数据内存
    free(imageData);
    return 0;
}
