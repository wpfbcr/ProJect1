
//#include"filecompare.h"
#include"heap.h"

template<class T>
struct huffnode{
	T _weight;
	huffnode<T> *_left;
	huffnode<T> *_right;

	huffnode(const T &weight)
		:_weight(weight),
		_left(NULL),
		_right(NULL)
	{}
};
struct charinfo{
	char _ch;
	unsigned int _count;
	string code;
	charinfo(unsigned int count = 0)
		:_count(count)
	{}
	charinfo(const charinfo &c)
	{
		for (int i = 0; i < 256; i++)
		{
			_ch = c._ch;
			_count = c._count;
			code = c.code;
		}
	}
	bool operator<(const charinfo &c) const
	{
		return _count<c._count;
	}
	charinfo operator+(const charinfo &c)
	{
		return charinfo(_count + c._count);
	}
	charinfo& operator=(const charinfo &c)
	{
		for (int i = 0; i < 256; i++)
		{
			_ch = c._ch;
			_count = c._count;
			code = c.code;
		}
		return *this;
	}
};
template<class T>
class Huffman{
public:	
	typedef huffnode<T> node;
	struct compare{
		bool operator()(node *w, node *p)
		{
			return w->_weight<p->_weight;
		}
	};
	void get_huffman(const charinfo _info[], int size) 
	{
		Huffman(_info, size);
	}
	Huffman()
	{}
	 Huffman(const charinfo _info[], int size)
		:_root(NULL)
	{

		for (int i = 0; i<size; ++i)
		{
			if (_info[i]._count>0)
			{
				charinfo fo = _info[i];
				huffnode<charinfo> *in = new huffnode<charinfo>(fo);
				_huff.push(in);
			}
		}
	}
	huffnode<charinfo> * getroot()
	{
		return _root;
	}
	void CreateHuffman()
	{
		_CreateHuffman(_root,_huff);
	}
protected:
	void _CreateHuffman(huffnode<charinfo> *root, Heap<node*, compare> huff)
	{
		while (huff.size() > 1)
		{
			node *left = huff.top();
			huff.Pop();

			node *right = huff.top();
			huff.Pop();

			node *parent = new node(left->_weight + right->_weight);

			parent->_left = left;
			parent->_right = right;

			huff.push(parent);
		}
		_root = huff.top();
	}

private:
	Heap<node*,compare> _huff;
	huffnode<charinfo> *_root;
};
