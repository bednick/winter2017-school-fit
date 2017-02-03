// MathFuncsLib.cpp
// compile with: cl /c /EHsc MathFuncsLib.cpp
// post-build command: lib MathFuncsLib.obj

#include "Encrypt.h"

using namespace std;

namespace Encrypt
{
	char Encrypt::step_MTF(char num)
	{
		forward_list<char>::iterator p = table_MTF.begin();
		char i;
		for (i = 0; *p != num; p++, i++);
		char modul = *p;
		table_MTF.remove(modul);
		table_MTF.push_front(modul);
		return i;
	};
	char Encrypt::back_step_MTF(char num)
	{
		forward_list<char>::iterator p = table_MTF.begin();
		for (int i = 0; i < num; p++, i++);
		char modul = *p;
		table_MTF.remove(modul);
		table_MTF.push_front(modul);
		return modul;
	}
	void Encrypt::step_huffman(char num){};
	void Encrypt::back_step_huffman(){}

	void Encrypt::generation_key() // заглушка тип
	{
		//key.reserve(13);
		std::cerr << key.size() << std::endl;

		key.push_back(2);// = 2;
		std::cerr << key.size() << std::endl;
		key[1] = 5;
		key[2] = 7;
		key[3] = 9;
		key[4] = 0;
		key[5] = 1;
		key[6] = 2;
		key[7] = 3;
		key[8] = 4;
		key[9] = 5;
		key[10] = 6;
		key[11] = 7;
		key[12] = 8;
	};
	void Encrypt::generation_table_MTF() // заглушка тип
	{
		int i;
		//table_MTF.resize(8);
		for (i = 0; i < 8; ++i) {
			table_MTF.push_front(key[i + 4]);
		}
	};
	void Encrypt::genetation_tree_huffman()
	{
		int i, j;
		char sttb[10][2] = {  // max -> min Если мы читаем с конца, то считываем от max и заканчиваем min
			{ 1,0 },  // 01
			{ 7,0 },  // 111
			{ 6,0 },  // 110
			{ 1,0 },  // 00001
			{ 4,0 },  // 100
			{ 1,0 },  // 001
			{ 11,0 }, // 1011
			{ 0,0 },  // 00000
			{ 10,0 }, // 1010
			{ 1,0 }   // 0001

		};
		int sizes[10] = { 2, 3, 3, 5, 3, 3, 4, 5, 4, 4 };
		tree_huffman.reserve(8);
		for (i = 0; i < 8; ++i)
		{
			tree_huffman[i].reserve(sizes[i]);
			for (j = 0; j < sizes[i]; ++j) 
			{
				tree_huffman[i][j] = (sttb[j / 8][0] & (1 << (j % 8))) != 0;
			}
		}
	};
	char Encrypt::get_mod_1() 
	{
		return key[0];
	};
	char Encrypt::get_mod_2() 
	{
		return key[1];
	};
	char Encrypt::get_mod_3() 
	{
		return key[2];
	};
	char Encrypt::get_mod_4() 
	{
		return key[3];
	};
	void Encrypt::encrypt(std::ifstream& file_in, std::ofstream& file_out)
	{
		
		char byte_in, i, j, byte_out = 0, position = 0;
		char rems[4];
		std::cerr << " start" << std::endl;
		generation_key();
		std::cerr << " start" << std::endl;
		generation_table_MTF();
		std::cerr << " start" << std::endl;
		genetation_tree_huffman();
		std::cerr << " start" << std::endl;
		while (!file_in) {
			file_in.read(&byte_in, 1);
			rems[0] = byte_in % get_mod_1();
			rems[1] = byte_in % get_mod_2();
			rems[2] = byte_in % get_mod_3();
			rems[3] = byte_in % get_mod_4();

			for (i = 0; i < 4; ++i) 
			{
				char rem = step_MTF(rems[i]);
				for (j = tree_huffman[rem].size() - 1; j >= 0; --j)
				{
					byte_out = byte_out << 1;
					if (tree_huffman[rem][j]) {
						byte_out = byte_out | 0x01;
					}
					++position;

					if (position == 8) {
						file_out.write(&byte_out, 1);
						position = 0;
						byte_out = 0;
					}
				}
			}
		}
		if (position != 0) {
			file_out.write(&byte_out, 1);
			file_out.write(&position, 1);
		}

		 //ЗАглушка
		/*file_out << (char)11;
		std::copy(
			std::istreambuf_iterator<char>(file_in),
			std::istreambuf_iterator<char>(),
			std::ostreambuf_iterator<char>(file_out));
			*/
	};
	void Encrypt::decrypt(std::ifstream& file_in, std::ofstream& file_out)
	{
		char buf;
		file_in.read(&buf, 1);
		std::copy(
			std::istreambuf_iterator<char>(file_in),
			std::istreambuf_iterator<char>(),
			std::ostreambuf_iterator<char>(file_out));
	};

	Encrypt::Encrypt() {}
	Encrypt::~Encrypt() {}
	void Encrypt::encrypt(const std::string file_in, const std::string file_out)
	{
		if (file_in == file_out) {
			std::cerr << "Error: the same names file" << std::endl;// exeption
			return;
		}
		std::ifstream stream_file_in(file_in, ios_base::in | ios_base::binary);
		if (!stream_file_in.is_open()) {
			std::cerr << "Error open file " << file_in << std::endl;// exeption
			return;
		}
		std::ofstream stream_file_out(file_out, ios_base::out | ios_base::trunc | ios_base::binary);
		if (!stream_file_out.is_open()) {
			std::cerr << "Error open/create file " << file_out << std::endl;// exeption
			return;
		}

		encrypt(stream_file_in, stream_file_out);
		stream_file_in.close();
		stream_file_out.close();
	};
	void Encrypt::decrypt(const std::string file_in, const std::string file_out)
	{
		if (file_in == file_out) {
			std::cerr << "Error: the same names file" << std::endl;// exeption
			return;
		}
		std::ifstream stream_file_in(file_in, ios_base::in | ios_base::binary);
		if (!stream_file_in.is_open()) {
			std::cerr << "Error open file " << file_in << std::endl;// exeption
			return;
		}
		std::ofstream stream_file_out(file_out, ios_base::out | ios_base::trunc | ios_base::binary);
		if (!stream_file_out.is_open()) {
			std::cerr << "Error open/create file " << file_out << std::endl;// exeption
			return;
		}
		decrypt(stream_file_in, stream_file_out);
		stream_file_in.close();
		stream_file_out.close();
	};
}