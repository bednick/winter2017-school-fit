#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct TableElement
{
	char number; // число, хранящееся 
	char size;  // количество бит
	void * valueBit;
};

typedef struct Table  {
	struct TableElement* elements;
	char size;
} Table;

typedef struct Key
{
	char sizeBlock;
	char zeroBits;
	char sizeMods;
	char* mods;
	struct Table * table;
	
} Key;

void initTable(Table* table);
void printTable(Table table);
void initTableElement(struct TableElement * m, unsigned char number, unsigned char sizeBit, char* valueBit);
int getBit(struct TableElement m, unsigned char index);
void encrypt(FILE* inFile, FILE* outFile);
void decrypt(FILE* inFile, FILE* outFile);
void destroyTable(Table* table);
void destroyTableElement(struct TableElement * m);
void initKey(Key * key, char sizeBlock);
void destroyKey(Key * key);

