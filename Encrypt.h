#pragma once
#include <vector>
#include <fstream>
#include <forward_list>
#include <bitset>
#include <iostream>

namespace Encrypt
{
	class Encrypt
	{
	private:
		//std::vector<char> table_MTF; // список нужен
		std::forward_list<char> table_MTF;
		std::vector<char> mods;
		std::vector<char> key;
		std::vector<std::vector<bool>> tree_huffman;
		

		char step_MTF(char num);
		char back_step_MTF(char num);
		void step_huffman(char num);
		void back_step_huffman();

		void generation_key();
		void generation_table_MTF();
		void genetation_tree_huffman();

		char get_mod_1();
		char get_mod_2();
		char get_mod_3();
		char get_mod_4();

		void encrypt(std::ifstream& file_in, std::ofstream& file_out);
		void decrypt(std::ifstream& file_in, std::ofstream& file_out);

	public:
		Encrypt();  // генерация хафмана и табл mtf ? Или только во время начала работы? 
		~Encrypt();
		void encrypt(const std::string file_in, const std::string file_out);
		void decrypt(const std::string file_in, const std::string file_out);
	};


}