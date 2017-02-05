#pragma once
#include <vector>
#include <fstream>
#include <forward_list>
#include <iostream>
#include <ctime>
#include <list>

namespace Encrypt
{
	class Encrypt
	{
	private:
		static const char SIZE_MODS = 4;
		static const char START_LEVEL = 5;
		static const char SIZE_OF_INDEX = 31;
		static const char SIZE_OF_BLOCK = 256;
		static const char SIZE_RAND_KEY = 11;
		std::ifstream file_in;
		std::ofstream file_out;

		std::vector<char> key;

		std::vector<std::vector<bool>> tree_huffman;
		std::list<char> table_MTF;
		std::vector<char> mods;
		

		char step_MTF(char num);
		char back_step_MTF(char num);

		int open_files(const std::string name_file_in, const std::string name_file_out);
		void close_files();
		
		void read_key();
		void write_key();

		void encrypt();
		void decrypt();

		typedef struct point {
			char x, y, z;
		} Evcl;

		int extEvcl(int x, int m);
		char chinaTheorem(char *r);

		// key
		void generation_key();
		int product();
		void mtf(char max_mod);
		int evklid(int a, int b);


		//table
		class Node
		{
		public:
			Node()
			{
				left = right = NULL;
			}

			Node(Node *L, Node *R)
			{
				left = L;
				right = R;
				size = L->size + R->size;
			}

			int size;
			unsigned char ch;
			Node *left, *right;
		};

		struct Compare
		{
			bool operator()(const Node* l, const Node* r) const { return l->size < r->size; }
		};

		void build_table(Node *root, std::vector<bool> *code);
		void genetation_tree_huffman( unsigned char maxmod);
		void index_generation( std::vector<unsigned char> *indexChar, unsigned char maxmod);

		struct DecodingTreeEntry {
			struct DecodingTreeEntry *left;
			struct DecodingTreeEntry *right;
			int value;
		};

		void insertValue(DecodingTreeEntry *where, std::vector<bool> &code, int offset, char value);
		DecodingTreeEntry *translateTableToTree(std::vector<std::vector<bool>> &table);
		char nextModule(char *buf, char &shift, DecodingTreeEntry *vertex);

		void print_huffman();

	public:
		Encrypt();  // генерация хафмана и табл mtf ? Или только во время начала работы? 
		//~Encrypt();
		void encrypt(const std::string name_file_in, const std::string name_file_out);
		void decrypt(const std::string name_file_in, const std::string name_file_out);


	};


}