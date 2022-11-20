#include "main_libs.h"
#include "bsr_ptn.h"
#include "bsr_se.h"
#include "bsr_ECL.h"
#include "ac6_main.h"

#ifdef CLB_DEBUG
int main()
{
	const char* pMap = NULL;
	std::string fName, f2, map;
	char op;
	printf("File name: "); std::cin >> fName;
	printf("Mode [A c6 | P tn | S e | E cl | M ap]: "); std::cin >> op;
	switch (op)
	{
	case 'A':
		//Nmlgc's AC6 extractor
		ac6_main(fName.c_str());
		break;
	case 'E':
		printf("Use Map? [Y / Any]: "); std::cin >> op;
		if (op == 'Y')
		{
			printf("File name: "); std::cin >> map;
			pMap = map.c_str();
		}
		printf("Mode [Compile | Dump]: "); std::cin >> op;
		if (op == 'D')
			ECLdump(fName.c_str(), pMap);
		else if (op == 'C')
		{
			printf("Output: "); std::cin >> f2;
			ECLComp(fName.c_str(), f2.c_str(), pMap);
		}
		break;
	case 'M':
		SCLIns = initMap(fName.c_str());
		break;
	case 'P':
		printf("Mode [Compile | Dump]: "); std::cin >> op;
		if (op == 'D')
			PTNDump(fName.c_str());
		else if (op == 'C')
		{
			printf("Output: "); std::cin >> f2;
			PTNComp(fName.c_str(), f2.c_str());
		}
		break;
	case 'S':
		SEDump(fName.c_str());
		break;
	}
}
#endif

#ifndef CLB_DEBUG

int main(int argc, char* argv[])
{
	//printf("aaaaaaaaaa %s, %s, %s", argv[0], argv[1], argv[2]);
	//printf("%s %s %s %s", argv[0], argv[1], argv[2], argv[3], argv[4]);
	if (argc < 3)
	{
		printf("Usage: %s [ ad | pd | ed | ec | sd] input [output | dir] [map] \n", argv[0]);
		exit(0);
	}
	std::string par = argv[1];
	
	if (par == "ad")
	{
		//Nmlgc's AC6 extractor
		ac6_main(argc, argv);
	}
	else if (par == "ed")
	{	if(argc > 2)
			ECLdump(argv[2], argv[3]);
		else
		printf("Usage: %s ed in [map]", argv[0]);
	}
	else if (par == "ec")
	{
		if (argc > 3)
		{
			ECLComp(argv[2], argv[3], argv[4]);
		}
		else
			printf("Usage: %s ec in out [map]", argv[0]);
	}
	else if (par == "pd")
	{
		if(argc > 2)
			PTNDump(argv[2]);
		else
			printf("Usage: %s pd input\n", argv[0]);
	}
	else if (par == "sd")
	{
		SEDump(argv[2]);
	}
}
#endif // !CLB_DEBUG
