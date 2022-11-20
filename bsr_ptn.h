#include "main_libs.h"

struct PTNHeader
{
	uint32_t sign;
	WORD ptnTot;
	WORD texCnt;
	bool checkSign() { return sign == 0x314e5450; }
	void initialize() { sign = 0x314e5450; ptnTot = 0; texCnt = 0; };
};

struct ptnEntry
{
	std::string texName;
	WORD ptnCnt;
};

struct spriteDef
{
	std::string ptnName;
	int x1, y1, x2, y2;
	float w, h;
	uint32_t colorARGB;
	float w2, h2;
	int uint0, uint1, uint2;
	float uflt0, uflt1;
	int uint3;
};

bool PTNformatStop(char);

void PTNDump(const char*);
void PTNComp(const char*, const char*);