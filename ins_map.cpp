#include "bsr_ECL.h"

static bool MapFormat(char c)
{
	return c == ' ' || c == '\n' || c == ':' || c == '!' || c == ',' || c == ';' || c == 0x0d || c == '	';
}

InsMap initDefMap()
{
	InsMap ret;
	uint8_t i;
	std::string temp = "CMD_", concat;
	for (i = 0; i < 0xca; i++)
	{
		Ins tempIns;
		tempIns.name = temp + std::to_string(i);
		ret.insert({ i, tempIns });
	}
#ifdef CLB_DEBUG
	for (i = 0; i < 0xca; i++)
	{
		//std::cout << "\n" << ret[i];
	}
#endif
	return ret;
}

InsMap initMap(const char* f)
{
	InsMap temp;
	Clb::file file(true);
	std::string tempN, tempStr;
	uint8_t id;

	file.loadFile(f);
	file.setFSkip(&isBlankSpace);
	file.fSkip(); 
	if (file.getString() == "!bsr_ins:")
	{
		file.fSkip();
		while (!file.isEof())
		{
			file.setFSkip(&MapFormat);
			tempN = file.getString();
			if (Clb::getInt(tempN, &id, 1))
			{
				file.setFSkip(&skipSpace);
				file.fSkip();
				if (file.getCurrentChar() == ',')
				{
					file.movePos(1);
					file.fSkip();
					file.setFSkip(&MapFormat);
					tempStr = file.getString();
					file.setFSkip(&skipSpace);
					file.fSkip();
					if (file.getCurrentChar() == ';')
					{
						Ins tempIns;
						tempIns.name = tempStr;
						temp.insert({ id, tempIns });
						file.setFSkip(&MapFormat);
						file.fSkip();
					}
					else
					{
						printf("String not found.\n");
						exit(32);
					}
				}
				else
				{
					printf("Missing comma.\n");
					exit(32);
				}
			}
			else
			{
				printf("Unrecognized number.\n");
				exit(32);
			}
		}
#ifdef CLB_DEBUG
		for (InsMap::iterator i = temp.begin(); i != temp.end(); ++i)
		{
			std::cout << i->second.name << ": 0x" << std::hex << (WORD)i->first << std::dec << "\n";
		}
#endif
	}
	return temp;
}