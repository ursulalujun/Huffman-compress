#include "huffman.h"
//��ȡͷ�ļ�����дƵ�ȱ���

void readHead( int frequency[27], int &lastValidBit)
{
    FILE* fp;
    fp = fopen("zip.huf", "rb");//b -- binary
    if (!fp)
        cout << "error opening zip file when decompressing" << endl;
    unsigned char value[20];
    fread(&value, sizeof(unsigned char)*4, 1, fp);
    fread(&lastValidBit, sizeof(int), 1, fp);
    if (value[0] == 'H' && value[1] == 'U' && value[2] == 'F')
    {
        for (int i = 0;i < 27;i++)
            fread(&frequency[i], sizeof(int), 1, fp);
    }
    else
    {
        cout << "\nThe file format is incorrect and cannot be decompressed.\n";
        return;
    }
    fclose(fp);
}

void readZip(pNode HuffmanT, int lastValidBit)
{
    FILE* fpOut,*fpIn;
    fpIn = fopen("zip.huf", "rb");//b -- binary
    if (!fpIn)
        cout << "error opening zip file when decompressing" << endl;
    unsigned char value, outValue;
    long fileSize;
    long curLocation;
    int bitIndex = 0;
    pNode root = HuffmanT;

    fpOut = fopen("dezip.txt", "wb");
    if (!fpOut)
        cout << "error opening dezip file when decompressing" << endl;

    fseek(fpIn, 0L, SEEK_END);
    fileSize = ftell(fpIn);	 //�ļ��ܳ���fileSize(�ֽ���)
    fseek(fpIn, 116L, SEEK_SET); //�Թ�ǰ��116���ֽڵ�Ԫ���ݣ��ַ������Ƶ��
    curLocation = ftell(fpIn); //ftell:���ظ����� stream �ĵ�ǰ�ļ�λ��
    fread(&value, sizeof(unsigned char), 1, fpIn);//ÿ�ζ�һ���ֽ�
    while (1)
    {
        if (!root->left && !root->right)
        {
            outValue = root->value;
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
    fclose(fpOut);
    fclose(fpIn);
}

void decompression()
{  
    pNode HuffmanT;
    pNode root;
    pNode Huffman[60];
    
    int CopyFrequency[27], lastValidBit;  //����Ƶ�ȱ���
    readHead(CopyFrequency, lastValidBit);//��ȡԪ����

    //���ٿռ��haffman��
    for (int i = 0;i < 60;i++)
    {
        Huffman[i] = (pNode)malloc(sizeof(TreeNode));
        Huffman[i]->left = NULL;
        Huffman[i]->right = NULL;
    }
    HuffmanT = HuffmanTree(CopyFrequency, Huffman);
    if (!HuffmanT)
    {
        cout << "construct tree error" << endl;return;
    }

    readZip( HuffmanT, lastValidBit);
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
