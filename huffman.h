#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstdio>
using namespace std;
#define MAXN 1500
#define INFINITE 214748364
//ȡ��indexλ����ȡ����indexλΪ0����GET_BYTEֵΪ�٣�����Ϊ��
#define GET_BYTE(vbyte, index) (((vbyte) & (1 << ((index) ^ 7))) != 0)

//��indexλ����Ϊ��1��
#define SET_BYTE(vbyte, index) ((vbyte) |= (1 << ((index) ^ 7)))

//��indexλ����Ϊ��0��
#define CLR_BYTE(vbyte, index) ((vbyte) &= (~(1 << ((index) ^ 7))))

int charNum = 5;
typedef struct Tree
{
    char value;
    int frequency;
    struct Tree* left;
    struct Tree* right;
}TreeNode, * pNode;

//�����ýṹ��������߶�ά������01����
struct Code
{
    int code[60];
};

typedef struct HUF_FILE_HEAD {
    unsigned char flag[3];				//ѹ���������ļ�ͷ����־ ycy
    unsigned char alphaVariety;		//�ַ�����
    unsigned char lastValidBit;		//���һ���ֽڵ���Чλ��
    unsigned char unused[11];			//���ÿռ�
} HUF_FILE_HEAD;						//����ṹ���ܹ�ռ��16���ֽڵĿռ�

void createTXT(int num);//�����ı��ļ�
void getFrequency(int alphabet[27]);//��ȡ�ַ����ֵ�Ƶ��
pNode HuffmanTree(int alphabet[27], pNode Huffman[60]);//����Huffman��
int selectMin(pNode nodeSet[60], int& min1, int& min2);//����������ѡ����С������Ԫ��
void HuffmanCode(pNode now, Code codemap[27], char tempCode[60], int depth);//��ȡhuffman����
void compression(Code codemap[27]);//ѹ���ļ�
void decompression(pNode HuffmanT);//��ѹ�ļ�
void test(pNode HuffmanT);//�������ѹ���ļ��е�01����
