#include "sb.h"

int main () {
	Table table;
	initTable(&table);
	Key key;
	initKey(&key, 8);
	/**/
	printTable(table);
	





	/**/
	destroyTable(&table);
	destroyKey(&key);
}


void initTable(Table* table) {
	int i;
	char sttb[10][2] = {  // max -> min Если мы читаем с конца, то считываем от max и заканчиваем min
		{ 1,0 },  // 01
		{ 7,0 },  // 111
		{ 6,0 },  // 110
		{ 4,0 },  // 100
		{ 1,0 },  // 001
		{ 11,0 }, // 1011
		{ 10,0 }, // 1010
		{ 1,0 },  // 0001
		{ 1,0 },  // 00001
		{ 0,0 }   // 00000
	};
	int sizes[10] = { 2, 3, 3, 3, 3, 4, 4, 4, 5, 5 };
	table->elements = (struct TableElement *)malloc(sizeof(struct TableElement) * 10);
	table->size = 10;

	for (i = 0; i < 10; ++i) {
		initTableElement(& (table->elements)[i], i, sizes[i], sttb[i]);
	}
}

void destroyTable(Table* table) {
	int i;
	for (i = 0; i < 10; ++i) {
		destroyTableElement(& (table->elements)[i]);
	}
	free(table->elements);
}

void printTable(Table table) {
	int i, j;
	fprintf(stderr, "PRINT Table:\n");
	fprintf(stderr, "size: %d\n", table.size);
	for (i = 0; i < table.size; ++i) {
		fprintf(stderr, "el=%d ", table.elements[i].number);
		fprintf(stderr, "bits: ");
		for (j = 0; j < table.elements[i].size; ++j) {
			fprintf(stderr, "%d", getBit(table.elements[i], j));
		}
		fprintf(stderr, "\n");
	}
	
}

/*void initTable(struct TableElement* table) {
	int i;
	char sttb[10][2] = {  // max -> min Если мы читаем с конца, то считываем от max и заканчиваем min
		{ 1,0 },  // 01
		{ 7,0 },  // 111
		{ 6,0 },  // 110
		{ 4,0 },  // 100
		{ 1,0 },  // 001
		{ 11,0 }, // 1011
		{ 10,0 }, // 1010
		{ 1,0 },  // 0001
		{ 1,0 },  // 00001
		{ 0,0 }   // 00000
	};

	int sizes[10] = {2, 3, 3, 3, 3, 4, 4, 4, 5, 5};
	for (i = 0; i < 10; ++i) {
		initTableElement(&table[i], i, sizes[i], sttb[i]);
	}
}
*/
void initTableElement(struct TableElement * m, unsigned char number, unsigned char sizeBit, char* valueBit) {
	m->size = sizeBit;
	m->valueBit = malloc(sizeBit % 8 ? (sizeBit / 8)*8 + 1 : sizeBit); // ТАКНАДАНИТРОГАЙ
	m->number = number;
	memcpy(m->valueBit, valueBit, m->size % 8 ? m->size / 8 + 1 : m->size / 8);
}

void destroyTableElement(struct TableElement * m) {
	free(m->valueBit);
}

int getBit(struct TableElement m, unsigned char index) {
	char * pointer = (char*)m.valueBit;
	return ( pointer[index/8] & (1 << (index % 8)) ) != 0;
}

void encrypt(FILE* inFile, FILE* outFile) {
	Table table;
	initTable(&table);
	Key key;
	initKey(&key, 8);


}

void decrypt(FILE* inFile, FILE* outFile) {

}

void initKey(Key * key, char sizeBlock) {
	int i;
	char mods[5] = {2, 3, 5, 7, 11};
	key->sizeBlock = sizeBlock;
	key->sizeMods = 5;
	key->mods = (char*)malloc(sizeof(char) * key->sizeMods);
	for (i = 0; i < key->sizeMods; ++i) {
		(key->mods)[i] = mods[i];
	}
	key->table = NULL;
	key->zeroBits = -1;
}

void destroyKey(Key * key) {
	free(key->mods);
}

