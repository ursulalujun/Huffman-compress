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
    fileSize = ftell(fpIn);	 //�ļ��ܳ���fileSize(�ֽ���)
    fseek(fpIn, 16 + sizeof(int) * 27, SEEK_SET);
    //�Թ�ǰ��16���ֽڵ�Ԫ���ݣ�5�ֽڵ��ַ������Ƶ��
    curLocation = ftell(fpIn); //ftell:���ظ����� stream �ĵ�ǰ�ļ�λ��
    fread(&value, sizeof(unsigned char), 1, fpIn);//ÿ�ζ�һ���ֽ�
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

        //ȡ����һ���ֽڴӵ�һλ��ʼ����'0'���������ߣ�'1'����������
        //������һ���ֽڣ�8λ������Ҫ��ȡ��һ���ֽ�
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

//���Դ���
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
    fileSize = ftell(fpIn);	//�ļ��ܳ���fileSize
    fseek(fpIn, 0L, SEEK_SET);	//�Թ�ǰ��16���ֽڵ�Ԫ���ݣ�5�ֽڵ��ַ������Ƶ��
    curLocation = ftell(fpIn);//ftell:���ظ����� stream �ĵ�ǰ�ļ�λ��
    fread(&value, sizeof(unsigned char), 1, fpIn);//ÿ�ζ�һ���ֽ�
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