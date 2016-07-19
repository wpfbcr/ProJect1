#pragma once;
#include"heap.h"
#include"Huffman.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string>



class filecompare{
public:
	filecompare()
	{}
	void uncompressMain(const char *filename)
	{
		for (int i = 0; i<256; i++)
		{
			info[i]._ch = i;
			info[i]._count = 0;
			info[i].code = "";
		}
		FILE *fout = fopen(filename, "r");
		assert(fout);
		string line;
		while (getconfline(line, fout))
		{
			info[(unsigned char)line[0]]._count = atoi(line.substr(1).c_str());
			line.clear();
		}
		fclose(fout);
		tree = Huffman<charinfo>(info, 256);

		//1、构建huffman树
		tree.CreateHuffman();
		//2、对字符进行编码
		string code = "";
		codeHuffman(tree.getroot(), code);
	}
	bool getconfline(string &line, FILE*fout)
	{
		char ch = fgetc(fout);
		if (ch==EOF)
		{
			return false;
		}
		while (ch!=EOF&&ch!='\n')
		{
			line += ch;
			ch = fgetc(fout);
		}
		return true;
	}
	void compressMain(const char *filename)
	{
		for (int i = 0; i<256; i++)
		{
			info[i]._ch = i;
			info[i]._count = 0;
		}
		FILE *fout = fopen(filename, "r");
		assert(fout);

		char ch = fgetc(fout);
		while (ch != EOF)
		{
			info[ch]._count++;
			ch = fgetc(fout);
		}
		fclose(fout);
		tree=Huffman<charinfo>(info, 256);

		//1、构建huffman树
		tree.CreateHuffman();
		//2、对字符进行编码
		string code = "";
		codeHuffman(tree.getroot(),code);
		//3、压缩文件
		//string filenameout = filename;
		//createCompress(filenamein, filenameout);

		//4、解压缩
		//uncompress(filenameout,tree.getroot());
	}
	void uncompress(string _filenameout)
	{
		string unfilenamein = _filenameout + ".uncompass";
		string filenameout = _filenameout + ".conf";
		FILE *fout = fopen(filenameout.c_str(),"r");
		assert(fout);
		FILE *fin = fopen(unfilenamein.c_str(),"w");
		assert(fin);

		const char *filename = filenameout.c_str();
		uncompressMain(filename);
		fclose(fout);

		huffnode<charinfo> *root = tree.getroot();
		huffnode<charinfo> *cur = root;
		filenameout = _filenameout + ".compass";
		fout = fopen(filenameout.c_str(), "r");
		assert(fout);
		unsigned char ch = fgetc(fout);
		int pos = 7;
		int size = root->_weight._count;
		/*while (size > 0)
		{
			cur = root;
			while (cur->_left != NULL || cur->_right != NULL)
			{
				pos--;
				unsigned char temp = ch >> pos;
				int c = 1 & temp;
				if (c == 0)
				{
					cur = cur->_left;
				}
				else if (c == 1)
				{
					cur = cur->_right;
				}
				if (pos == 0)
				{
					ch = fgetc(fout);
					pos = 8;
				}
			}
			fputc(cur->_weight._ch, fin);

			size--;
		}
		fclose(fin);
		fclose(fout);*/

		while (size>0)
		{			
			if (ch&(1 << pos))
			{
				cur = cur->_right;
			}
			else
			{
				cur = cur->_left;
			}
			if (cur->_left == NULL&&cur->_right == NULL)
			{
				--size;
				fputc(cur->_weight._ch, fin);
				cur = root;
			}
			--pos;
			if (pos < 0)
			{	
				ch = fgetc(fout);
				pos = 7;
			}
		}
		fclose(fin);
		fclose(fout);

	}
	void createCompress(string filenameout)
	{
		const char *filename = filenameout.c_str();
		compressMain(filename);
		string filenamein = filenameout + ".compass";
		FILE *fout = fopen(filenameout.c_str(), "r");
		assert(fout);
		FILE *fin = fopen(filenamein.c_str(), "w");
		assert(fin);
		string file_conf = filenameout + ".conf";
		FILE *fin_conf = fopen(file_conf.c_str(), "w");
		char buf[128];
		for (int i = 0; i < 256; ++i)
		{
			string confout;
			if (info[i]._count > 0)
			{
				confout += info[i]._ch;
				confout += itoa(info[i]._count, buf, 10);
				confout += '\n';
				fputs(confout.c_str(), fin_conf);
			}
		}
		fclose(fin_conf);
		char ch = fgetc(fout);
		int value = 0;
		int index = 0;
		while (ch != EOF)
		{
			string &code = info[(unsigned char)ch].code;
			for (int i = 0; i < code.size(); ++i)
			{
				value <<= 1;
				if (code[i] == '1')
				{
					value |= 1;
				}
				++index;
				if (index == 8)
				{
					fputc(value, fin);
					index = 0;
					value = 0;
				}
			}
			ch = fgetc(fout);
		}
		if (index)
		{
			value <<= (8 - index);
			fputc(value, fin);
		}
		fclose(fout);
		fclose(fin);
}
	void codeHuffman(huffnode<charinfo> *root,string code)
	{
		if (root == NULL)
		{
			return;
		}
		if (root->_left == NULL&&root->_right == NULL)
		{
			info[root->_weight._ch].code = code;
			return;
		}

		codeHuffman(root->_left,code+'0');
		codeHuffman( root->_right,code+'1');

	}
private:
	charinfo info[256];
	Huffman<charinfo> tree;
};
