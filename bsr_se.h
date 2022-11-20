#include "main_libs.h"

struct SEHeader
{
	uint32_t sign;
	WORD seCnt;
	bool isSign()
	{
		return sign == 0x30444e53;
	}
};

struct SEEntry
{
	std::string id;
	BYTE unk0, unk1;
	std::string sndFile;
};

void SEDump(const char*);
//void SEComp(const char*);