#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <ctime>
#include <list>

namespace Encrypt
{
	class Table_MTF
	{
	public:
		void clear();
		void push_back(char value);
		char raise_by_index(char value);
		char raise_by_value(char value);
		char get(char index);
		Table_MTF();
		~Table_MTF();
	private:
		typedef struct Node_MTF
		{
			struct Node_MTF* next;
			char value;
		} Node_MTF;

		Node_MTF* head;
		Node_MTF* tail;


	};

	class Encrypt
	{
	private:
		static const char SIZE_MODS = 4;
		static const char START_LEVEL = 5;
		static const char SIZE_OF_INDEX = 31;
		static const int SIZE_OF_BLOCK = 256;
		static const char SIZE_RAND_KEY = 11;
		std::ifstream file_in;
		std::ofstream file_out;

		std::vector<char> key;

		std::vector<std::vector<bool>> tree_huffman;
		Table_MTF table_MTF;
		std::vector<char> mods;
		std::vector<std::vector<char>> value_mods;
		

		char step_MTF(char num);
		char back_step_MTF(char num);

		int open_files(const char *name_file_in, const char *name_file_out);
		void close_files();
		
		void read_key();
		void write_key();

		char max_mod();
		void init_value_mods();

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
		//mtf
		
	public:
		Encrypt();  // ãåíåðàöèÿ õàôìàíà è òàáë mtf ? Èëè òîëüêî âî âðåìÿ íà÷àëà ðàáîòû? 
		//~Encrypt();
		void encrypt(const char *name_file_in, const char *name_file_out);
		void decrypt(const char *name_file_in, const char *name_file_out);
	};
	
#ifdef _cplusplus
 extern C {
#endif

	void decrypt_f(const char *name_file_in, const char *name_file_out);
	void encrypt_f(const char *name_file_in, const char *name_file_out);

#ifdef _cplusplus
 }
#endif
}
