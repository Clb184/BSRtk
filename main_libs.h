#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>


#include <stdlib.h>
#include <string.h>
#include <string>
#ifdef WIN32
#include <windows.h>
#endif

//#include "bsr_ptn.h"
//#include ""

//typedef unsigned int DWORD;
//typedef unsigned short int WORD;
//typedef unsigned char BYTE;

typedef std::map<std::string, int> constantDefI;
typedef std::map<std::string, float> constantDefF;

static uint32_t line;

static constantDefI GlobalConstInt;
static constantDefF GlobalConstFloat;
static constantDefI LocalConstInt;
static constantDefF LocalConstFloat;

static const char* currentFile;

bool strSkip(char);
bool numSkip(char); 
bool skipSpace(char); 
bool isBlankSpace(char);
bool isValidChar(char);

namespace Clb
{
	bool getInt(const std::string&, void*, size_t);
	bool getFloat(const std::string&, float&);

	class file
	{
	public:
		//Managing file
		bool loadFile(const char*);
		bool isEof();
		char getCurrentChar() const;
		char getCurrentChar(size_t) const;
		void getff(void*, size_t);
		void setFSkip(bool (*)(char));
		void setRFormat(bool (*)(char));

		//Managing file (For writting)
		void createFile(const char*);
		void appendFile(void*, size_t);
		void putc(char);
		void writeFile();
		//Getting something
		std::string getString();
		std::string getString(bool(*)(char));
		//

		//Pos manipulation
		size_t getSize() const;
		uint32_t getPos() const;
		void setPos(size_t);
		bool movePos(size_t);
		void fSkip(); //Format skip

		//Constructor and Destructor
		file(bool);
		~file();
	private:
		const char* fName;
		bool (*sfunc)(char);
		bool (*rfunc)(char);
		bool isFormat, isOutput;
		uint32_t pos = 0;
		size_t fsize;
		char* buffer;
	};
}


#define findInt(g, hogehoge) if (Clb::getInt(hogehoge, &g, 4)) {}\
else if (LocalConstInt.find(hogehoge) != LocalConstInt.end())\
g = LocalConstInt[hogehoge];\
else if (GlobalConstInt.find(hogehoge) != GlobalConstInt.end())\
g = GlobalConstInt[hogehoge];\
else \
{printf("Error: \n");\
exit(-1);}


#define findFloat(g, hogehoge )if (Clb::getFloat(hogehoge, g)) {}\
else if (LocalConstFloat.find(hogehoge) != LocalConstFloat.end())\
g = LocalConstFloat[hogehoge];\
else if (GlobalConstFloat.find(hogehoge) != GlobalConstFloat.end())\
g = GlobalConstFloat[hogehoge];\
else \
{printf("Error: \n");\
exit(-1);}


#define findInt2(g, hogehoge, s) if (Clb::getInt(hogehoge, &g, s)) {}\
else if (LocalConstInt.find(hogehoge) != LocalConstInt.end())\
g = LocalConstInt[hogehoge];\
else if (GlobalConstInt.find(hogehoge) != GlobalConstInt.end())\
g = GlobalConstInt[hogehoge];\
else \
{printf("Error: Float constant not found.\n");\
exit(-1);}

#define findVar(g, hogehoge )if (Clb::getInt(hogehoge, &g, 4)) {}\
else if (Clb::getFloat(hogehoge, g)) {}\
else if (LocalConstInt.find(hogehoge) != LocalConstInt.end())\
g = LocalConstInt[hogehoge];\
else if (GlobalConstInt.find(hogehoge) != GlobalConstInt.end())\
g = GlobalConstInt[hogehoge];\
else if (LocalConstFloat.find(hogehoge) != LocalConstFloat.end())\
g = LocalConstFloat[hogehoge];\
else if (GlobalConstFloat.find(hogehoge) != GlobalConstFloat.end())\
g = GlobalConstFloat[hogehoge];\
else \
{printf("Error: Float constant not found.\n");\
exit(-1);}


#define findVar2(g, hogehoge, s )if (hogehoge == "PI") { float p = 3.1415926535897932384626433832795; \
memcpy(&g, &p, 4); } \
else if(Clb::getInt(hogehoge, &g, s)) {} \
else if (Clb::getFloat(hogehoge, (float&)g)) {}\
else if (LocalConstInt.find(hogehoge) != LocalConstInt.end())\
g = LocalConstInt[hogehoge];\
else if (GlobalConstInt.find(hogehoge) != GlobalConstInt.end())\
g = GlobalConstInt[hogehoge];\
else if (LocalConstFloat.find(hogehoge) != LocalConstFloat.end())\
g = LocalConstFloat[hogehoge];\
else if (GlobalConstFloat.find(hogehoge) != GlobalConstFloat.end())\
g = GlobalConstFloat[hogehoge];\
else \
{printf("Error: Constant not found.\n");\
exit(-1);}