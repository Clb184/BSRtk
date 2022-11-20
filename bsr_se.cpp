#include "bsr_se.h"

void SEDump(const char* fName)
{
	
	Clb::file file(false);
	file.loadFile(fName);
	file.setFSkip(&strSkip);
	SEHeader SEH;
	file.getff(&SEH, sizeof(SEHeader));
#ifdef CLB_DEBUG
	printf("Entries: %d\n", SEH.seCnt);
#endif // CLB_DEBUG
	if(!SEH.isSign())
	{
		printf("Not a SND file.\n");
		exit(-4);
	}
	for (int i = 0; i < SEH.seCnt; i++)
	{
		SEEntry SEE;
		SEE.id = file.getString();
		file.getff(&SEE.unk0, 2);
		SEE.sndFile = file.getString();
		std::cout << "\"" << SEE.id << "\" : " << (WORD)SEE.unk0 << ", " << (WORD)SEE.unk1 << " \"" << SEE.sndFile << "\"\n";
	}
}
