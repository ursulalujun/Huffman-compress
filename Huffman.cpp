#include "huffman.h"

int main()
{
    int frequency[27] = { 0 };//����Ƶ�ʣ�0��Ӧ�ո�1-26��Ӧ26����ĸ
    pNode HuffmanT;
    createTXT(MAXN);
    getFrequency(frequency);
    pNode Huffman[60];
    //Ҫ��main�����п��ٿռ��haffman�����ں������濪�Ļ���������ֻ�ں�����
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
    //�������ĵ��Դ���
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
    //����������ASCII�ķ�ʽ�����ĵ�
    for (i = 0;i < num;i++)
    {
        temp=rand() % 27 + 97;
        if (temp == 123)
            temp = 32;//�����ո�
        txt[i] = temp;
    }
    ofstream destFile("test.txt", ios::out); //���ı�ģʽ��test.txt��д
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
    ifstream srcFile("test.txt", ios::in); //���ı�ģʽ��test.txt����
    if (!srcFile) { //��ʧ��
        cout << "error opening source file." << endl;
        return;
    }
    //srcFile >> temp�޷���ȡ�ո�ֻ����get����getline
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
    //��ʼ���ڵ� 
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
    nodeSet[min1]->frequency = INFINITE;//ʵ�ʽڵ�Ӧ����27+26��
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

void HuffmanCode(pNode now, Code codemap[27],char tempCode[60], int depth)//������ȱ����Ĺ����б���
{
    //��Ҷ�ӽڵ�
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
    //��Ҷ�ӽڵ�
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
    tempCode[depth] = 2;//����
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

    
    //���ļ�ͷ��д��Ԫ����
    //fwrite(&fileHead, sizeof(HUF_FILE_HEAD), 1, fpOut);
    //��Ԫ���ݺ�д�ַ������Ƶ�ȣ���ѹ��ʱ��Ҫ����Щ����һģһ���µĹ�������
    //fwrite(alphaFreq, sizeof(ALPHA_FREQ), alphaVariety, fpOut);

    ch = fgetc(fpIn);
    while (!feof(fpIn)) {
        //��ÿ���ַ��Ĺ���������һ��һ������
        //������ַ�'0'����ת��Ϊ�����Ƶ�0
        //������ַ�'1'����ת��Ϊ�����Ƶ�1
        int hufCode = 0, i = 0;
        cout << ch << " ";
        while(1){
            if (ch == 32)
                hufCode = codemap[0].code[i];
            else
                hufCode = codemap[ch - 96].code[i];
            //cout << hufCode;
            if (hufCode == 2) break;//�����ڲ�ѭ����дһ��
            if (!hufCode) 
                //valueΪһ���ֽ�
                //�ӵ�1λ���θ�ֵ�������ڰ�λ��һ���ֽڣ��ˣ���д���ļ���
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
    //������һ�β���һ���ֽڣ���Ȼ��Ҫд���ļ��У�ע�⣺д������һ���ֽڿ��ܻ��������λ
    if (bitIndex) {
        fwrite(&value, sizeof(unsigned char), 1, fpOut);
    }
    cout <<" " << cnt;//�������������ַ����Ƿ���ȷ
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
    fileSize = ftell(fpIn);	//�ļ��ܳ���fileSize
    fseek(fpIn, 0L, SEEK_SET);	//�Թ�ǰ��16���ֽڵ�Ԫ���ݣ�5�ֽڵ��ַ������Ƶ��
    curLocation = ftell(fpIn);//ftell:���ظ����� stream �ĵ�ǰ�ļ�λ��
    fread(&value, sizeof(unsigned char), 1, fpIn);//ÿ�ζ�һ���ֽ�
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




