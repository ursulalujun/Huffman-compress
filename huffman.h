#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstdio>
using namespace std;
#define MAXN 15000
#define INFINITE 214748364

//取出index位，若取出的index位为0，则GET_BYTE值为假，否则为真
#define GET_BYTE(vbyte, index) (((vbyte) & (1 << ((index) ^ 7))) != 0)

//把index位设置为‘1’
#define SET_BYTE(vbyte, index) ((vbyte) |= (1 << ((index) ^ 7)))

//把index位设置为‘0’
#define CLR_BYTE(vbyte, index) ((vbyte) &= (~(1 << ((index) ^ 7))))

typedef struct Tree
{
    char value;
    int frequency;
    struct Tree* left;
    struct Tree* right;
}TreeNode, * pNode;

//可以用结构体数组或者二维数组存放01编码
struct Code
{
    int code[60];
};

typedef struct HUF_FILE_HEAD {
    unsigned char flag[3];				//压缩二进制文件头部标志 huf
    int lastValidBit;		//最后一个字节的有效位数
} HUF_FILE_HEAD;						

void createTXT(int num);//创建文本文件

int compression();//压缩总过程
void getFrequency(int alphabet[27]);//获取字符出现的频率
pNode HuffmanTree(int alphabet[27], pNode Huffman[60]);//构建Huffman树
int selectMin(pNode nodeSet[60], int& min1, int& min2);//构建中用于选出最小的两个元素
void HuffmanCode(pNode now, Code codemap[27], char tempCode[60], int depth);//获取huffman编码
int getlastValidBit(Code codemap[27], int frequency[]);//用于判断最后一个字节的有效位数
void writeZip(Code codeMap[27], int frequency[27]);//写入压缩文件

void decompression();//解压总过程
void readHead(int frequency[27], int& lastValidBit);//读取和解析元数据
void readZip(pNode HuffmanT, int lastValidBit);//读取压缩文件并解压

void test(pNode HuffmanT);//调试输出压缩文件中的01编码

