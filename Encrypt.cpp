// MathFuncsLib.cpp
// compile with: cl /c /EHsc MathFuncsLib.cpp
// post-build command: lib MathFuncsLib.obj

#include "Encrypt.h"

using namespace std;

namespace Encrypt
{
	char Encrypt::step_MTF(char num)
	{
		list<char>::iterator p = table_MTF.begin();
		char i;
		for (i = 0; *p != num; p++, i++)
			;
		char modul = *p;
		table_MTF.remove(modul);
		table_MTF.push_front(modul);
		return i;
	};
	char Encrypt::back_step_MTF(char num)
	{
		list<char>::iterator p = table_MTF.begin();
		for (int i = 0; i < num; p++, i++);
		char modul = *p;
		table_MTF.remove(modul);
		table_MTF.push_front(modul);
		return modul;
	}

	void Encrypt::encrypt()
	{
		char i, j, byte_out = 0, position = 0;
		char byte_in;
		unsigned char byte_in_u;
		char rems[SIZE_MODS], max_mods = 0;

		generation_key();
		for (i = 0; i < SIZE_MODS; ++i)
		{
			if (max_mods < mods[i]) 
			{
				max_mods = mods[i];
			}
		}

		genetation_tree_huffman(max_mods);

		write_key();
		file_in.read(&byte_in, 1);
		byte_in_u = byte_in;
		while (file_in.good()) 
		{
			
			std::cerr << "read : " << (int)byte_in_u << std::endl;
			std::cerr << "step : ";
			for (i = 0; i < SIZE_MODS; ++i ) {
				rems[i] = byte_in_u % mods[i];
				std::cerr << (int)rems[i] << " ";
			}
			

			std::cerr << std::endl;
			//std::cerr << "mtf : ";
			for (i = 0; i < SIZE_MODS; ++i)
			{
				char rem = step_MTF(rems[i]);
				std::cerr << (int)rem << std::endl;
				//std::cerr << (int)rem  << " ";
				//for (j = tree_huffman[rem].size() - 1; j >= 0; --j)
				for (j = 0; j < tree_huffman[rem].size(); ++j)
				{
					byte_out = byte_out << 1;
					if (tree_huffman[rem][j]) {
						byte_out = byte_out | 0x01;
					}
					++position;
					if (position == 8) {
						file_out.write(&byte_out, 1);
						std::cerr << "			wr "<< (int)byte_out << std::endl;
						position = 0;
						byte_out = 0;
					}
				}
			}
			//std::cerr << std::endl;
			file_in.get(byte_in);
			byte_in_u = byte_in;
		}
		if (position != 0) {
			byte_out <<= 8 - position;
			file_out.write(&byte_out, 1);
			//std::cerr << "			wr " << (int)byte_out << std::endl;
			//std::cerr << "			wr " << (int)position << std::endl;
		}
		//file_out.write(&position, 1);
		//std::cerr << " pos = " << (int)position << " byte "<< (int)(byte_out) << std::endl;
		print_huffman();
		std::cerr << " finish encr" << std::endl;
	};
	void Encrypt::decrypt()
	{
		char  max_mods = 0, i;
		read_key();
		for (i = 0; i < SIZE_MODS; ++i)
		{
			if (max_mods < mods[i])
			{
				max_mods = mods[i];
			}
		}

		genetation_tree_huffman(max_mods);
		DecodingTreeEntry *root = translateTableToTree(tree_huffman);
		char rems[SIZE_MODS];
		char buf_mtf[SIZE_MODS];
		char buf[2];
		char shift = 0;
		//print_huffman();
		
		file_in.read(buf, sizeof (buf));
		std::cerr << "       read : " <<  (int)buf[0] << std::endl;
		std::cerr << "       read : " <<  (int)buf[1] << std::endl;
		do {
			
			for (i = 0; i < SIZE_MODS; ++i) 
			{
				char next = nextModule(buf, shift, root);
				std::cerr << (int)next << std::endl;
				buf_mtf[i] = back_step_MTF(next);
				//std::cerr << (int)next << " mtf : " << (int)buf_mtf[i] << std::endl;
			}
			std::cerr << "step ";
			for (i = 0; i < SIZE_MODS; ++i)
			{
				std::cerr << (int)buf_mtf[i] << " ";
			}
			std::cerr << std::endl;
			char t = chinaTheorem(buf_mtf);
			file_out.write(&t, 1);
			std::cerr << "		write " << (int)(t) << std::endl;
			
		} while (file_in.good());
		std::cerr << std::endl;
	};
	void Encrypt::read_key()
	{
		int i;
		char buff, max_mod = 0;
		key.clear();
		mods.clear();
		table_MTF.clear();
		for (i = 0; i < SIZE_RAND_KEY; ++i)
		{
			file_in.get(buff);
			key.push_back(buff);
		}

		for (i = 0; i < SIZE_MODS; ++i)
		{
			file_in.get(buff);
			mods.push_back(buff);
		}
		for (i = 0; i < SIZE_MODS; ++i)
		{
			if (max_mod < mods[i])
			{
				max_mod = mods[i];
			}
		}
		for (i = 0; i < max_mod; ++i)
		{
			file_in.get(buff);
			key.push_back(buff);
			table_MTF.push_back(buff);
		}
	}
	void Encrypt::write_key()
	{
		int i;
		//std::cerr << "key.size() " << key.size() << std::endl;

		for (i =0; i < key.size(); ++i)
		{
			file_out.write(&key[i], 1);
			//std::cerr << (int)key[i] << " " ;
		}
		//std::cerr << std::endl;
	}

	int Encrypt::extEvcl(int x, int m) {
		Evcl V, U, T;
		char t;
		x = x%m;
		V.x = m; V.y = 1; V.z = 0;
		U.x = x; U.y = 0; U.z = 1;
		while (U.x > 0) {
			T.x = V.x % U.x;
			t = V.x / U.x;
			T.y = V.y - (U.y*t);
			T.z = V.z - (U.z*t);
			V = U;
			U = T;
		}
		return V.z > 0 ? V.z : m + V.z;
	}
	char Encrypt::chinaTheorem(char *r) {
		int M = 1;
		int tmp;
		char rs;
		tmp = 0;
		int Mi, Mi1;
		for (char i = 0; i < SIZE_MODS; i++) M *= mods[i]; //Произведение модулей
		for (char i = 0; i < SIZE_MODS; i++) { //По формуле
			Mi = M / mods[i];
			Mi1 = extEvcl(Mi, mods[i]);
			tmp += (r[i] * Mi * Mi1) % M;
		}
		rs = tmp % M;
		return rs;
	}

	int  Encrypt::evklid(int a, int b) {
		while (a && b)
			if (a >= b)
				a %= b;
			else
				b %= a;
		return a | b;
	}

	void  Encrypt::mtf(char max_mod) {
		srand(time(NULL));
		vector<int> reversibles;
		for (int i = 2; i < max_mod; ++i) {
			if (evklid(i, max_mod) == 1)
				reversibles.push_back(i);
		}
		int a = reversibles[rand() % reversibles.size()];


		int b = rand() % max_mod;


		for (int x = 0; x < max_mod; ++x) {
			table_MTF.push_back( (a*x + b) % max_mod);
		}
	}


	int Encrypt::product() {
		int p = 1;
		for (int j = 0; j < SIZE_MODS; ++j)
			p *= mods[j];
		return p;
	}

	void Encrypt::generation_key()
	{
		char max_mod = 0;
		char id;
		srand(time(NULL));
		id = rand() % START_LEVEL;
		char index2[SIZE_OF_INDEX] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127 };
		char index[SIZE_OF_INDEX] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127 };
		mods.clear();
		mods.resize(SIZE_MODS);
		for (int i = 0; i < SIZE_MODS; ++i)
			mods[i] = 1;

		for (char i = 0; i < SIZE_MODS; ++i)
		{
			mods[i] = index[id];
			index[id] = 0;
			while (!index[id]) {
				id = rand() % START_LEVEL;
			}
			if (mods[i] > max_mod)
				max_mod = mods[i];
		}

		while (product() < 500)
		{
			for (char i = 0; i < SIZE_MODS; ++i)
			{
				id = rand() % SIZE_OF_INDEX;
				while ((mods[i] * index2[id]) < 128)
				{
					if (index[id] != 0 || mods[i] % index2[id] == 0) // id - 8
					{
						mods[i] *= index2[id];
						index[id] = 0;
					}
					id = rand() % SIZE_OF_INDEX;
					if (mods[i] > max_mod)
						max_mod = mods[i];
				}
			}
		}
		int i = 0;
		for (; i < 5; ++i)
			key.push_back(rand() % 128);


		key.push_back(rand() % 78 + 50);
		++i;

		for (; i < SIZE_RAND_KEY; ++i)
			key.push_back(rand() % 128);



		for (char i = 0; i < SIZE_MODS; ++i)
			key.push_back(mods[i]);
		mtf(max_mod);
		list<char>::iterator p = table_MTF.begin();
		for (char i = 0; i < max_mod; ++i) {
			key.push_back(*p);
			++p;
		}
		//std::cerr << " generic key" << std::endl;
		vector<char>::iterator w = key.begin();
		//char cou = 0;
		//for (cou = 0; cou < key.size(); ++ cou)
		//{
			//td::cerr << (int)key[cou] << " ";
		//}
		//std::cerr << std::endl;
		//return key;
	}


	void Encrypt::build_table(Node *root, std::vector<bool> *code)
	{
		if (root->left != NULL)
		{
			(*code).push_back(false);

			build_table(root->left, code);
		}

		if (root->right != NULL)
		{
			(*code).push_back(true);

			build_table(root->right, code);
		}

		if (root->left == NULL && root->right == NULL)
		{
			for (unsigned char i = 0; i < (*code).size(); ++i)
			{
				tree_huffman[root->ch].push_back((*code)[i]);
			}
		}

		if ((*code).size() != 0)
		{
			(*code).pop_back();
		}
	}

	void Encrypt::genetation_tree_huffman( unsigned char maxmod)
	{
		tree_huffman.clear();
		tree_huffman.resize(maxmod);

		std::vector<unsigned char> indexChar;

		index_generation(&indexChar, maxmod);

		std::list<Node*> tree;

		for (unsigned char i = 0; i < maxmod; ++i)
		{
			Node *l = new Node;
			l->size = indexChar[i];
			l->ch = i;
			tree.push_back(l);
		}

		while (tree.size() != 1)
		{
			tree.sort(Compare());

			Node *sheetL = tree.front();
			tree.pop_front();

			Node *sheetR = tree.front();
			tree.pop_front();

			Node *parent = new Node(sheetL, sheetR);
			tree.push_back(parent);
		}

		Node *root = tree.front();

		std::vector<bool> code;

		build_table(root, &code);
	}

	void Encrypt::index_generation(std::vector<unsigned char> *indexChar, unsigned char maxmod)
	{
		for (int i = 0; i < 5; ++i)
		{
			indexChar->push_back((key)[i + 6]);
		}

		for (int i = 5; i < maxmod; ++i)
		{
			(*indexChar).push_back((
				(*indexChar)[i - 1] * (key)[4] +
				(*indexChar)[i - 2] * (key)[3] +
				(*indexChar)[i - 3] * (key)[2] +
				(*indexChar)[i - 4] * (key)[1] +
				(*indexChar)[i - 5] * (key)[0]) %
				(key)[5]);
		}
	}


	int Encrypt::open_files(const std::string name_file_in, const std::string name_file_out)
	{
		if (name_file_in == name_file_out) {
			std::cerr << "Error: the same names file" << std::endl;// exeption
			return -1;
		}
		file_in.open(name_file_in, ios_base::in | ios_base::binary);
		if (!file_in.is_open()) {
			std::cerr << "Error open file " << std::endl;// exeption
			return -1;
		}
		file_out.open(name_file_out, ios_base::out | ios_base::trunc | ios_base::binary);
		if (!file_out.is_open()) {
			file_in.close();
			std::cerr << "Error open/create file " << std::endl;// exeption
			return -1;
		}
		return 0;
	};
	void Encrypt::close_files() 
	{
		file_in.close();
		file_out.close();
	}
	Encrypt::Encrypt() {};
	void Encrypt::encrypt(const std::string name_file_in, const std::string name_file_out)
	{
		if (open_files(name_file_in, name_file_out) == -1)
			return;
		encrypt();
		close_files();
	};
	void Encrypt::decrypt(const std::string name_file_in, const std::string name_file_out)
	{
		if (open_files(name_file_in, name_file_out)) return;
		decrypt();
		close_files();
	};


	void Encrypt::insertValue(Encrypt::DecodingTreeEntry *where, std::vector<bool> &code, int offset, char value) {
		if (offset == code.size()) {
			where->value = value;
			where->left = NULL;
			where->right = NULL;
			return;
		}
		if (!code[offset]) {
			if (where->left == NULL) {
				where->left = (DecodingTreeEntry *)malloc(sizeof(DecodingTreeEntry));
				where->left->left = NULL;
				where->left->right = NULL;
				where->left->value = -1;
			}
			insertValue(where->left, code, offset + 1, value);
		}
		else {
			if (where->right == NULL) {
				where->right = (DecodingTreeEntry *)malloc(sizeof(DecodingTreeEntry));
				where->right->left = NULL;
				where->right->right = NULL;
				where->right->value = -1;
			}
			insertValue(where->right, code, offset + 1, value);
		}
	}

	Encrypt::DecodingTreeEntry *Encrypt::translateTableToTree(std::vector<std::vector<bool>> &table) {
		DecodingTreeEntry *root = (DecodingTreeEntry *)malloc(sizeof(DecodingTreeEntry));
		root->left = NULL;
		root->right = NULL;
		root->value = -1;
		for (int i = 0; i < table.size(); ++i) {
			vector<bool> code = table[i];
			insertValue(root, code, 0, i);
		}
		return root;
	}

	char Encrypt::nextModule(char *buf, char& shift, Encrypt::DecodingTreeEntry *vertex) {
		if (shift == 8) {
			for (int i = 0; i < 1; i++) {
				buf[i] = buf[i + 1];
			}
			file_in.read(&(buf[1]), 1);
			std::cerr << "      read : " <<  (int) (buf[1]) << std::endl;

			shift = 0;
		}
		//if (file_in.eof()) {
			//shift = 8 - buf[2];
			//buf[0] = buf[0] << 8 - buf[1];
		//}
		if ((vertex->left == NULL) && (vertex->right == NULL)) {
			return vertex->value;
		}
		if (buf[0] & 128) {
			vertex = vertex->right;
		}
		else {
			vertex = vertex->left;
		}
		buf[0] = buf[0] << 1;
		++shift;

		
		return (nextModule(buf, shift, vertex));
	}

	void Encrypt::print_huffman()
	{
		std::cout << std::endl;

		for (unsigned char i = 0; i < tree_huffman.size(); ++i)
		{
			std::cout << (int)i << ": ";

			for (unsigned char j = 0; j < tree_huffman[i].size(); ++j)
			{
				std::cout << (int)tree_huffman[i][j];
			}

			std::cout << std::endl;
		}
	}
}