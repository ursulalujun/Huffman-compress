#include "huffman.h"

void decompression(pNode HuffmanT, int lastValidBit)
{
    FILE* fpIn;
    FILE* fpOut;
    unsigned char value, outValue;
    long fileSize;
    long curLocation;
    int bitIndex = 0;
    pNode root;
    root = HuffmanT;

    fpIn = fopen("zip.huf", "rb");//b -- binary
    if (!fpIn)
        cout << "error opening zip file when decompressing" << endl;
    fpOut = fopen("dezip.txt", "wb");
    if (!fpOut)
        cout << "error opening dezip file when decompressing" << endl;
    fseek(fpIn, 0L, SEEK_END);
    fileSize = ftell(fpIn);	 //文件总长度fileSize(字节数)
    fseek(fpIn, 16 + sizeof(int) * 27, SEEK_SET);
    //略过前面16个字节的元数据，5字节的字符种类和频度
    curLocation = ftell(fpIn); //ftell:返回给定流 stream 的当前文件位置
    fread(&value, sizeof(unsigned char), 1, fpIn);//每次读一个字节
    while (1)
    {
        if (!root->left && !root->right)
        {
            outValue = root->value;
            //cout << outValue << " ";
            fwrite(&outValue, sizeof(unsigned char), 1, fpOut);
            if (curLocation >= fileSize && bitIndex >= lastValidBit)break;
            root = HuffmanT;
        }

        //取出的一个字节从第一位开始看，'0'向左子树走，'1'向右子树走
        //若超过一个字节，8位，则需要读取下一个字节
        if (GET_BYTE(value, bitIndex)) root = root->right;
        else root = root->left;
        if (++bitIndex >= 8)
        {
            bitIndex = 0;
            fread(&value, sizeof(unsigned char), 1, fpIn);
            curLocation = ftell(fpIn);
        }
    }

    fclose(fpIn);
    fclose(fpOut);

}

//测试代码
void test(pNode HuffmanT)
{
    FILE* fpIn;
    unsigned char value, outValue;
    long fileSize;
    long curLocation;
    int bitIndex = 0;
    pNode root;
    root = HuffmanT;
    fpIn = fopen("zip.huf", "rb");//b -- binary
    if (!fpIn)
        cout << "error opening zip file when decompressing" << endl;
    fseek(fpIn, 0L, SEEK_END);
    fileSize = ftell(fpIn);	//文件总长度fileSize
    fseek(fpIn, 0L, SEEK_SET);	//略过前面16个字节的元数据，5字节的字符种类和频度
    curLocation = ftell(fpIn);//ftell:返回给定流 stream 的当前文件位置
    fread(&value, sizeof(unsigned char), 1, fpIn);//每次读一个字节
    while (1)
    {
        if (curLocation >= fileSize) break;

        if (GET_BYTE(value, bitIndex)) {
            cout << "1";
        }
        else {
            cout << "0";
        }
        if (++bitIndex >= 8) {
            bitIndex = 0;
            fread(&value, sizeof(unsigned char), 1, fpIn);
            curLocation = ftell(fpIn);
        }
    }
    fclose(fpIn);
}