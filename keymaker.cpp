#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#define NOMS 4
using namespace std;
typedef unsigned char uc_t;

uc_t mods[NOMS];
int product() {
	unsigned int p = 1;
	for (int j = 0; j < NOMS; ++j)
		p *= mods[j];
	return p;
}


vector<uc_t> generate()
{
	vector<uc_t> key;

	uc_t id;
	srand(time(NULL));
	id = rand()%31;
	uc_t max_mod = 1;
	uc_t index[31] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127};
	for (int i = 0; i < NOMS; ++i)
		mods[i] = 1;

	for (uc_t i = 0; i < NOMS; ++i)
	{
		mods[i] = index[id];
		index[id] = 0;
		while (!index[id]) {
			id = rand() % 31;
		}
		if (mods[i] > max_mod)
			max_mod = mods[i];
	}



	while (product() < 256) 
	{

		for (uc_t i = 0; i < NOMS; ++i) 
		{
			while ((mods[i] * index[id]) < 128 && index[id] != 0) 
			{
				mods[i] *= index[id];
				index[id] = 0;
				id = rand() % 31;
				if (mods[i] > max_mod)
					max_mod = mods[i];
			}
		}
	}

	for (uc_t i = 0; i < NOMS; ++i)
		key.push_back(mods[i]);

	uc_t mtf[256];
	for (uc_t i = 0; i < max_mod; ++i)
		mtf[i] = 251;
	id = rand() % max_mod;
	for (uc_t i = 0; i < max_mod; ++i)
	{
		while (mtf[id]!=251)
			id = rand() % max_mod;
		mtf[id] = i;
	}

	for (uc_t i = 0; i < max_mod - 1; ++i)
		key.push_back(mtf[i]);
	return key;
}

int main()
{
	ofstream fout;
	fout.open("key.cthdhcdvvf7iokeyvgud");
	fout << rand() % 555 + 11;
	vector<uc_t> secretKey = generate();
	for (vector<uc_t>::iterator it = secretKey.begin(); it != secretKey.end(); ++it)
		fout << *it;
	fout.close();
	return 0;
}