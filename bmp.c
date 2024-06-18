#include "bmp.h"

int readBMP(const char *filename, BITMAPFILEHEADER *fileHeader, BITMAPINFOHEADER *infoHeader,RGBQUAD *palette, unsigned char **imageData) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Could not open file for reading");
        return 1;
    }
    
    // 打印当前文件指针的位置偏移量
    printf("Current file position: %ld bytes from start\n", ftell(file));
    // 读取文件头
    if (fread(fileHeader, sizeof(BITMAPFILEHEADER), 1, file) != 1) {
        perror("Error reading file header");
        fclose(file);
        return 1;
    }
    
    // 检查文件类型是否为BM
    if (fileHeader->bfType != 0x4D42) {
        fprintf(stderr, "Not a valid BMP file\n");
        fclose(file);
        return 1;
    }
    
    printf("Current file position: %ld bytes from start\n", ftell(file));
    // 读取信息头
    if (fread(infoHeader, sizeof(BITMAPINFOHEADER), 1, file) != 1) {
        perror("Error reading info header");
        fclose(file);
        return 1;
    }
    
    printf("Current file position: %ld bytes from start\n", ftell(file));
    // 读取调色板
    if (fread(palette, sizeof(RGBQUAD), 256, file) != 256) {
        perror("Error reading palette");
        fclose(file);
        return 1;
    }
    // 打印当前文件指针的位置偏移量
    printf("Current file position: %ld bytes from start\n", ftell(file));
    // 分配内存存储图像数据
    *imageData = (unsigned char *)malloc(infoHeader->biSizeImage);
    if (!*imageData) {
        perror("Could not allocate memory for image data");
        fclose(file);
        return 1;
    }

    // 移动文件指针到图像数据开始的位置
    fseek(file, fileHeader->bfOffBits, SEEK_SET);

    // 读取图像数据
    if (fread(*imageData, 1, infoHeader->biSizeImage, file) != infoHeader->biSizeImage) {
        perror("Error reading image data");
        free(*imageData);
        fclose(file);
        return 1;
    }
    // 打印当前文件指针的位置偏移量
    printf("Current file position: %ld bytes from start\n", ftell(file));
    // 关闭文件
    fclose(file);
    return 0;
}



int writeBMP(const char *filename, const BITMAPFILEHEADER *fileHeader, const BITMAPINFOHEADER *infoHeader,
             const RGBQUAD *palette, const unsigned char *imageData) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Could not open file for writing");
        return 1;
    }

    // 写入文件头
    if (fwrite(fileHeader, sizeof(BITMAPFILEHEADER), 1, file) != 1) {
        perror("Error writing file header");
        fclose(file);
        return 1;
    }

    // 写入信息头
    if (fwrite(infoHeader, sizeof(BITMAPINFOHEADER), 1, file) != 1) {
        perror("Error writing info header");
        fclose(file);
        return 1;
    }

    // 写入调色板
    if (fwrite(palette, sizeof(RGBQUAD), 256, file) != 256) {
        perror("Error writing color palette");
        fclose(file);
        return 1;
    }

    // 写入图像数据
    int rowSize = ((infoHeader->biWidth * infoHeader->biBitCount + 31) / 32) * 4;
    for (int i =0 ; i <= infoHeader->biHeight; i++) {
        if (fwrite(imageData + i * infoHeader->biWidth, sizeof(unsigned char), infoHeader->biWidth, file) != infoHeader->biWidth) {
            perror("Error writing image data");
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}



void bmpDataSaveToTXT(unsigned char *imageData, BITMAPINFOHEADER *infoheader,const char*filename)
{
    FILE *fp_s = NULL;
    
    if(NULL == (fp_s = fopen(filename, "w"))) {
        printf("open %s faild\n", filename);
    }
    //按照bmp的格式要求：从左下角开始读数据
    for(int i=infoheader->biHeight-1; i>0; i--) {
        for(int j=0;j<infoheader->biWidth * infoheader->biBitCount / 8; j++) {
            fprintf(fp_s, "%d\t", imageData[i*(infoheader->biWidth * infoheader->biBitCount / 8)+j]);
        }
        fprintf(fp_s, "\n");
    }

    printf("保存完成.\n");
    fclose(fp_s);
    return;
}

void accordTXTSaveBMP(const char *src_filename,const BITMAPFILEHEADER *fileHeader, const BITMAPINFOHEADER *infoHeader,
 const RGBQUAD *palette)
 {
    
    
    FILE *fp_txt = NULL;
    if(NULL == (fp_txt = fopen(src_filename, "r"))) 
    {
        printf("open %s faild\n", src_filename);
        fclose(fp_txt);
        return;
    }
    printf("open TXT fileName: %s\n",src_filename);
    char des_filename[64];
    sprintf(des_filename, "%s_resave.bmp", src_filename);
    FILE *bmpFp = fopen(des_filename, "wb");
    if (!bmpFp) {
        printf("Error opening BMP file.\n");
        fclose(bmpFp);
        return;
    }
    fwrite(fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpFp);
    fwrite(infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpFp);
    fwrite(palette, sizeof(RGBQUAD), 256, bmpFp);

    // unsigned char pixel;//用于读取文件bffer数据
    printf("pixel height:%d,pixel width:%d.\n",infoHeader->biHeight,infoHeader->biWidth);
    
    //创建行buffer，用于后面数据从最后一行开始写入
    unsigned char *rowBuffer = (unsigned char *)malloc(infoHeader->biWidth *infoHeader->biHeight* sizeof(unsigned char));
    if (!rowBuffer) {
        perror("Error allocating memory for row buffer");
        fclose(fp_txt);
        fclose(bmpFp);
        return;
    }

    


    unsigned char pixel;
    for (int i =0 ; i <infoHeader->biHeight*infoHeader->biWidth; i++) {
        
            
            int ret = fscanf(fp_txt, "%hhu", &pixel);
            rowBuffer[i]=pixel;
            //printf("Current file position X %d \n",x);
            if ( ret != 1 && ret != EOF) {
                printf("Current file position: %ld bytes from start\n", ftell(fp_txt));
                printf("Error reading pixel.\n");
                fclose(fp_txt);
                fclose(bmpFp);
                return;
            }
        
    
        
    }
    for(int i=infoHeader->biHeight - 1; i>=0; i--) {
        fwrite(rowBuffer+i*infoHeader->biWidth, sizeof(char) * infoHeader->biWidth, 1, bmpFp);
    }

    free(rowBuffer);
    fclose(fp_txt);
    fclose(bmpFp);
    printf("保存bmp完成.\n");
    return;
 }