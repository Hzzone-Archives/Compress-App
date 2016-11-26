//
// Created by Hzzone on 2016/10/30.
//
#include "component.h"
bool cmp(const HuffTree<unsigned char>* x1, const HuffTree<unsigned char>* x2){
    return x1->weight()>x2->weight();
}
template <typename E>
HuffTree<E>* buildHuff(const vector<HuffTree<E>*>& word_table){
    vector<HuffTree<E>*> forest(word_table);
    HuffTree<E> *temp1, *temp2, *temp3 = NULL;
    make_heap(forest.begin(), forest.end(), cmp);
    while(forest.size()>1){
        make_heap(forest.begin(), forest.end(), cmp);
        temp1 = forest.front();
        forest.erase(forest.begin());
        make_heap(forest.begin(), forest.end(), cmp);
        temp2 = forest.front();
        forest.erase(forest.begin());
        temp3 = new HuffTree<unsigned char>(temp1, temp2);
        forest.push_back(temp3);
        delete temp1;
        delete temp2;
    }
    return temp3;
}
//构造词汇表
void buildWordTable(HuffNode<unsigned char>* tree, map<unsigned char, string>& word_table, string code){
    if(tree->isLeaf()){
        // cout<<word_table[tree->root()->val()]<<endl;
        // cout<<static_cast<LeafNode<unsigned char>*>(tree)->val()<<":"<<code<<endl;
        word_table[static_cast<LeafNode<unsigned char>*>(tree)->val()] += code;
        return;
    }
    if(!tree->isLeaf()){
        buildWordTable(static_cast<IntlNode<unsigned char>*>(tree)->left(), word_table, code+'0');
        buildWordTable(static_cast<IntlNode<unsigned char>*>(tree)->right(), word_table, code+'1');
    }
}

//由文件生成huffman编码，返回格式为一个string
string HuffmanCode(char* filename, map<unsigned char, string>& word_table){
    //以二进制形式读文件
    ifstream fin(filename, ios::binary);
    if(!fin.is_open()){
        cout<<"1:open error!"<<endl;
        exit(2);
    }
    string huffmanCode = "";
    unsigned char c;
    //这个只能用peek，如果用eof会多循环一次
    while(fin.peek()!=EOF){
        fin.read((char*)&c, sizeof(char));
        huffmanCode += word_table[c];
    }
    fin.close();
//    cout<<huffmanCode<<endl;
    return huffmanCode;
}

//将huffman编码写入文件, 并返回编码的长度
//filename 为压缩文件
<<<<<<< HEAD
=======
//第一个int为词汇表的个数，然后依次为char键，string长度(没有加一), string, 压缩文件的bit数,最后为Huffman编码
>>>>>>> c8e53e6cebb25bd81c804c3626305eefe06d7f69
void writeToFile(char* filename, const map<unsigned char, string>& table, const string& code, char* secret, const char* fileFormat){
    ofstream fout(filename, ios::binary);
    if(!fout.is_open()){
        cout<<"2:open error!"<<endl;
        exit(3);
    }
    //写入关键信息的一行
    char isCompr = -1;
    char endline = '\n';
    bool t = true;
    bool f = false;
    fout.write(&isCompr, 1);
    if(strlen(secret)==0){
        fout.write((char*)&f, sizeof(bool));
        fout.write(&endline, 1);
    }else{
        fout.write((char*)&t, sizeof(bool));
        fout.write(secret, strlen(secret)+1);
        fout.write(&endline, 1);
    }
    fout.write(fileFormat, strlen(fileFormat)+1);
    fout.write(&endline, 1);
    int table_size = table.size();
    fout.write((char*)&table_size, sizeof(int));
    for(auto it=table.begin();it!=table.end();it++){
        unsigned char c = it->first;
        //将字符写入文件
        fout.write((char*)&c, sizeof(char));
        int Huffcode_size = it->second.size();
        //将huffman编码的长度写入文件
        fout.write((char*)&Huffcode_size, sizeof(int));
        //复制huffman编码到一个char数组中以写入文件
        unsigned char* temp = new unsigned char[Huffcode_size+1];
        for(int i=0;i<Huffcode_size;i++)
            temp[i] = it->second[i];
        fout.write((char*)temp, Huffcode_size+1);
        delete[] temp;
    }
    //将哈夫曼编码的长度写入文件，解压的时候可以将哈夫曼编码提取出来
    int encode_later_bits_length = code.size();
//    cout<<encode_later_bits_length<<endl;
    fout.write((char*)&encode_later_bits_length, sizeof(int));
    //将huffman编码写入文件,以unsigned char
    int num=0;
    for(int i=0;i<encode_later_bits_length;i++){
        num += (code[i]-'0')*pow(2, 7-i%8);
        if(i%8==7||i==encode_later_bits_length-1){
            unsigned char temp = num;
            fout.write((char*)&temp, sizeof(unsigned char));
            num = 0;
        }
    }
    fout.close();
}
//传入一个文件，将其压缩并输出到另一个文件
void encode(char* input_filename, char* output_filename, char* secret, const char* fileFormat){
    //以二进制形式读文件
    ifstream fin(input_filename, ios::binary);
    if(!fin.is_open()){
        cout<<"3:open error!"<<endl;
        exit(1);
    }
    //记录每个unsigned char出现的次数
    map<unsigned char, int> char_num;
    unsigned char c;
    while(fin.peek()!=EOF){
        fin.read((char*)&c, sizeof(unsigned char));
        if(char_num.find(c)==char_num.end()){
            char_num[c] = 1;
            continue;
        }
        char_num[c]++;
    }
    fin.close();
    //构造huffman树
    vector<HuffTree<unsigned char>*> v;
    map<unsigned char, string> table;
    for(auto it=char_num.begin();it!=char_num.end();it++){
        v.push_back(new HuffTree<unsigned char>(it->first, it->second));
        //初始化词汇表
        table[it->first] = "";
    }
    HuffTree<unsigned char>* tree = buildHuff(v);
    //构造词汇表
    string code = "";
    buildWordTable(tree->root(), table, code);
    //构建文本文件的huffman编码
    code = HuffmanCode(input_filename, table);
    // cout<<code<<endl;
    writeToFile(output_filename, table, code, secret, fileFormat);
}

//将十进制转化为二进制字符串
string transfer(int num){
    string code = "";
    int n = num%2;
    num /= 2;
    code += n+'0';
    while(num!=0){
        n = num%2;
        num /= 2;
        code += n+'0';
    }
    //补齐八位剩余的位数
    while(code.size()<8)
        code += '0';
    reverse(code.begin(), code.end());
    return code;
}
//读出huffman编码
string getHuffmancode(ifstream& fin, const int& length){
    string code = "";
    int bits_it = 8;
    while(fin.peek()!=EOF){
        unsigned char c;
        fin.read((char*)&c, sizeof(char));
        int num = c;
        string temp = transfer(num);
//        cout<<"temp:"<<temp<<endl;
        if(bits_it>8*(length/8)){
            for(int i=0;i<length-8*(length/8);i++)
                code += temp[i];
            break;
        }
        code += temp;
        bits_it += 8;
    }
//    cout<<"gethuffman:"<<code<<endl;
    return code;
}

//input_filename 压缩文件
//output_filename 解压文件
void decode(char* input_filename, char* output_filename){
    //以二进制形式读文件
    ifstream fin(input_filename, ios::binary);
    if(!fin.is_open()){
        cout<<"open error:"<<input_filename<<endl;
        exit(5);
    }
    //如果是压缩文件,则忽略第一行文件信息，第二行文件格式
    fin.ignore(100, '\n');
    fin.ignore(100, '\n');
    //读入词汇表,以map存储
    int table_size;
    fin.read((char*)&table_size, sizeof(int));
    map<string, unsigned char> table;
    for(int i=0;i<table_size;i++){
        unsigned char c;
        fin.read((char*)&c, 1);
        int huffmancode_size;
        fin.read((char*)&huffmancode_size, sizeof(int));
        unsigned char* huffmancode_char = new unsigned char[huffmancode_size+1];
        fin.read((char*)huffmancode_char, huffmancode_size+1);
        string huffmancode = "";
        for(int j=0;j<huffmancode_size;j++){
            huffmancode += huffmancode_char[j];
        }
        table[huffmancode] = c;
    }
    int length;
    fin.read((char*)&length, sizeof(int));
    string huffmancode = getHuffmancode(fin, length);
    //text为压缩文件中的内容
    string text = "";
    string code = "";
    for(unsigned long i=0;i<huffmancode.size();i++){
        code += huffmancode[i];
        if(table.find(code)!=table.end()){
            text += table[code];
            code = "";
        }
    }
    fin.close();
    ofstream fout(output_filename, ios::binary);
    for(unsigned long i=0;i<text.size();i++){
        unsigned char c = text[i];
        fout.write((char*)&c, 1);
    }
    fout.close();
}


/*
 * 检查模块
 * 判断是否是压缩文件
 * 判断是加密，如果加密则返回字符串，否则返回空
 */

//是否是压缩文件
bool isCompressFile(char* filename){
    ifstream fin(filename, ios::binary);
    if(!fin.is_open()){
        cout<<"isCompressFile open error"<<endl;
        exit(1);
    }
    char isCompressFile;
    fin.read(&isCompressFile, 1);
    fin.close();
    if(isCompressFile==-1)
        return true;
    return false;
}

//是否是加密文件
const char* isSecretFile(char* filename){
    ifstream fin(filename, ios::binary);
    if(!fin.is_open()){
        cout<<"isCompressFile open error"<<endl;
        exit(1);
    }
    fin.ignore(1);
    bool isSecretFile;
    fin.read((char*)&isSecretFile, sizeof(bool));
    if(isSecretFile){
        char* s = new char[50];
        fin.getline(s, 49);
        const char* s1 = s;
        return  s1;
    }
    fin.close();
    string p = "";
    return p.c_str();
}

//读取文件格式
const char* fileFormat(char* filename){
    ifstream fin(filename, ios::binary);
    if(!fin.is_open()){
        cout<<"fileFormat:open error"<<endl;
        exit(1);
    }
    //忽略第一行
    fin.ignore(100, '\n');
    char* format = new char[100];
    //读取文件格式
    fin.getline(format, 99);
    fin.close();
    const char* f = format;
    return f;
}
