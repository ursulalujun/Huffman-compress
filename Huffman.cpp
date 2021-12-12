#include "huffman.h"

int main()
{
    int frequency[27] = { 0 };//记载频率，0对应空格，1-26对应26个字母
    pNode HuffmanT;
    createTXT(MAXN);
    getFrequency(frequency);
    pNode Huffman[60];
    //要在main函数中开辟空间存haffman树，在函数里面开的话生存周期只在函数内
    for (int i = 0;i < 60;i++)
    {
        Huffman[i] = (pNode)malloc(sizeof(TreeNode));
        Huffman[i]->left = NULL;
        Huffman[i]->right = NULL;
    }
    HuffmanT = HuffmanTree(frequency, Huffman);
    if (!HuffmanT)
    {
        cout << "construct tree error" << endl;return 0;
    }
    Code codemap[27];
    char tempCode[60];
    for (int i = 0;i < 60;i++)
        tempCode[i] = 2;
    for (int i = 0;i < 27;i++)
    {
        for (int j = 0;j < 60;j++)
            codemap[i].code[j] = 2;
    }
    HuffmanCode(HuffmanT, codemap, tempCode, 0);
    //输出编码的调试代码
    char t = 'a';
    for (int i = 0;i < 27;i++)
    {
        cout << "\n";
        if (i)
            cout << t++ << " ";
        for (int j = 0;j < 10;j++)
            cout <<codemap[i].code[j];
    }
    
    compression(codemap);
    decompression(HuffmanT);
    test(HuffmanT);
}

void createTXT(int num)
{
    char txt[MAXN];
    int i, temp;
    srand((int)time(0));
    //用随机会产生ASCII的方式生成文档
    for (i = 0;i < num;i++)
    {
        temp=rand() % 27 + 97;
        if (temp == 123)
            temp = 32;//产生空格
        txt[i] = temp;
    }
    ofstream destFile("test.txt", ios::out); //以文本模式打开test.txt备写
    if (!destFile) {
        cout << "error opening file when creating text." << endl;
        return;
    }
    for (i = 0;i < num;i++)
    {
        destFile << txt[i];
    }
    destFile.close();
}

void getFrequency(int frequency[27])
{
    int cnt=0;
    char temp;
    ifstream srcFile("test.txt", ios::in); //以文本模式打开test.txt备读
    if (!srcFile) { //打开失败
        cout << "error opening source file." << endl;
        return;
    }
    //srcFile >> temp无法读取空格，只能用get或者getline
    while ((temp=srcFile.get())!=EOF)
    {
        if (temp == ' ')
            frequency[0]++;
        else
        {
            frequency[temp - 96]++;
        }
        cnt++;
    }
    for (int i = 0;i < 27;i++)
    {
        cout << frequency[i] << " ";
    }
    cout << cnt;
    srcFile.close();
}

pNode HuffmanTree(int alphabet[27], pNode Huffman[60])
{
    int i = 0, newValue, cnt = 27;
    int min1 = INFINITE, min2 = INFINITE;
    //初始化节点 
    for (i = 0;i < 60;i++)
    {
        if (i < 27)
        {
            if (i == 0) Huffman[i]->value = 32;
            else Huffman[i]->value = i + 96;
            Huffman[i]->frequency = alphabet[i];
        }
        else
        {
            Huffman[i]->value = 0;
            Huffman[i]->frequency = INFINITE;
        }
            
    }   
    while (cnt != 53)
    {
        newValue = selectMin(Huffman, min1, min2);
        Huffman[cnt]->frequency = newValue;
        Huffman[cnt]->left = Huffman[min1];
        Huffman[cnt]->right = Huffman[min2];
        cnt++;
    }    
    return Huffman[cnt-1];
    
    
}

int selectMin(pNode nodeSet[60],int& min1,int& min2)
{
    int minValue = INFINITE, newValue=0;
    for (int i = 0;i<60;i++)
    {
        if (nodeSet[i]->frequency < minValue)
        {
            minValue = nodeSet[i]->frequency;
            min1 = i;
        }           
    }
    nodeSet[min1]->frequency = INFINITE;//实际节点应该有27+26个
    newValue = minValue;
    minValue = INFINITE;
    for (int i = 0;i < 60;i++)
    {
        if (nodeSet[i]->frequency < minValue)
        {
            minValue = nodeSet[i]->frequency;
            min2 = i;
        }
    }
    nodeSet[min2]->frequency = INFINITE;
    newValue += minValue;
    return newValue;
}

void HuffmanCode(pNode now, Code codemap[27],char tempCode[60], int depth)//深度优先遍历的过程中编码
{
    //到叶子节点
    if (!now->left && !now->right)
    {
        int i = 0, key;
        if (now->value == 32) key = 0;
        else key = now->value - 96;
        if (key > 26 || key < 0)
        {
            cout << "encode error:overflow" << endl;
            return;
        }
        while (tempCode[i] != 2)
        {
            codemap[key].code[i] = tempCode[i];
            i++;
        }
        return;
    }
    //非叶子节点
    if (now->left)
    {
        tempCode[depth] = 0;
        HuffmanCode(now->left, codemap, tempCode, depth+1);
    }
    if (now->right)
    {
        tempCode[depth] = 1;
        HuffmanCode(now->right, codemap, tempCode, depth+1);
    }
    tempCode[depth] = 2;//擦除
    return;
}

void compression(Code codemap[27])
{
    FILE* fpIn;
    FILE* fpOut;
    int cnt = 0;
    char ch;
    unsigned char value;
    int bitIndex = 0;
    HUF_FILE_HEAD fileHead = { 'y', 'c', 'y' };

    fpIn = fopen("test.txt", "rb");
    if (!fpIn)
        cout << "error opening test file when compressing" << endl;
    fpOut = fopen("zip.huf", "wb");
    if (!fpOut)
        cout << "error opening zip file when compressing" << endl;
    //fileHead.alphaVariety = (unsigned char)alphaVariety;
    //fileHead.lastValidBit = getlastValidBit(huffmanTab, alphaVariety);

    
    //给文件头部写入元数据
    //fwrite(&fileHead, sizeof(HUF_FILE_HEAD), 1, fpOut);
    //给元数据后写字符种类和频度，解压缩时需要用这些生成一模一样新的哈夫曼树
    //fwrite(alphaFreq, sizeof(ALPHA_FREQ), alphaVariety, fpOut);

    ch = fgetc(fpIn);
    while (!feof(fpIn)) {
        //把每个字符的哈夫曼编码一个一个过。
        //如果是字符'0'，就转换为二进制的0
        //如果是字符'1'，就转换为二进制的1
        int hufCode = 0, i = 0;
        cout << ch << " ";
        while(1){
            if (ch == 32)
                hufCode = codemap[0].code[i];
            else
                hufCode = codemap[ch - 96].code[i];
            //cout << hufCode;
            if (hufCode == 2) break;//跳出内层循环读写一个
            if (!hufCode) 
                //value为一个字节
                //从第1位依次赋值，若大于八位（一个字节）了，就写入文件中
                CLR_BYTE(value, bitIndex);           
            else 
                SET_BYTE(value, bitIndex);
            
            bitIndex++;
            if (bitIndex >= 8) {
                bitIndex = 0;
                fwrite(&value, sizeof(unsigned char), 1, fpOut);
            }
            i++;
        }
        ch = fgetc(fpIn);
        cnt++;
    }
    //如果最后一次不满一个字节，依然需要写到文件中，注意：写入的最后一个字节可能会存在垃圾位
    if (bitIndex) {
        fwrite(&value, sizeof(unsigned char), 1, fpOut);
    }
    cout <<" " << cnt;//调试输出读入的字符数是否正确
    fclose(fpIn);
    fclose(fpOut);
}

void decompression(pNode HuffmanT)
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
    fileSize = ftell(fpIn);	//文件总长度fileSize
    fseek(fpIn, 0L, SEEK_SET);	//略过前面16个字节的元数据，5字节的字符种类和频度
    curLocation = ftell(fpIn);//ftell:返回给定流 stream 的当前文件位置
    fread(&value, sizeof(unsigned char), 1, fpIn);//每次读一个字节
    while (1) 
    {
        if (!root->left && !root->right) 
        {
            outValue = root->value;
            //cout << outValue << " ";
            fwrite(&outValue, sizeof(unsigned char), 1, fpOut);
            //if (curLocation >= fileSize && index >= fileHead.lastValidBit)
            if (curLocation >= fileSize) break;
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




