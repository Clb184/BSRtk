#include "bsr_ptn.h"

bool PTNformatStop(char c)
{
	return c == ' ' || c == ',' || c == ';' || c == 0x0d || c == 0x0a || c == ':' || c == '"' || c == '	';
}

void PTNDump(const char* fName)
{
	Clb::file file(false);
	file.loadFile(fName);
	PTNHeader ptnH;
	file.getff(&ptnH, sizeof(PTNHeader));
	if (!ptnH.checkSign())
	{
		printf("Not a PTN file.\n");
		exit(-3);
	}
#ifdef CLB_DEBUG
	printf(	"Sprite Total: %d \n"
			"Entry total: %d \n", ptnH.ptnTot, ptnH.texCnt);
#endif // CLB_DEBUG
	file.setFSkip(&strSkip);
	for (int i = 0; i < ptnH.texCnt; i++)
	{
		ptnEntry ptnE;
		ptnE.texName = file.getString();
		file.getff(&ptnE.ptnCnt, 2);
		std::cout << "\nEntry: \"" << ptnE.texName << "\"\n";
		for (int j = 0; j < ptnE.ptnCnt; j++)
		{
			spriteDef sprDef;
			sprDef.ptnName = file.getString();
			file.getff(&sprDef.x1, sizeof(int) * 15);
			std::cout << "sprite: \"" << sprDef.ptnName << "\", " 
				<< sprDef.x1 << ", " 
				<< sprDef.y1 << ", "
				<< sprDef.x2 << ", " 
				<< sprDef.y2 << ", " 
				<< sprDef.w << "f, " 
				<< sprDef.h << "f, "; 
			printf("0x%x", sprDef.colorARGB); std::cout << ", " 
				<< sprDef.w2 << "f, " << 
				sprDef.h2 << "f, " <<
				sprDef.uint0 << ", " <<
				sprDef.uint1 << ", " <<
				sprDef.uint2 << ", " <<
				sprDef.uflt0 << "f, " <<
				sprDef.uflt1 << "f, " <<
				sprDef.uint3 << "\n";
		}
	}
}

void PTNComp(const char* f, const char* f2)
{
	uint32_t l = 0;
	Clb::file file(true);
	file.loadFile(f);
	PTNHeader ptnH;
	ptnH.initialize();
	std::pair <ptnEntry, std::vector<spriteDef>> piyo;
	std::vector <std::pair <ptnEntry, std::vector<spriteDef>>> hoge;
	std::string tp;
	file.setFSkip(&PTNformatStop);
	file.fSkip();
	tp = file.getString();
	while(!file.isEof())
	{
		if ("Entry" == tp)
		{
			file.fSkip();
			ptnEntry hogera;
			file.setFSkip(&strSkip);
			//file.setPos(file.getPos() + 1);
			hogera.texName = file.getString();
			file.movePos(1);
			hogera.ptnCnt = 0;
			file.setFSkip(&PTNformatStop);
			file.fSkip();
			ptnH.texCnt++;
			std::vector<spriteDef> sprDefSt;
			while (!file.isEof())
			{
				tp = file.getString();
				if ("sprite" == tp)
				{
					std::string hogehoge;
					file.fSkip();
					file.setFSkip(&strSkip);
					spriteDef sprD;
					memset(&sprD, 0x00, sizeof(spriteDef));
					sprD.ptnName = file.getString();

					file.setFSkip(&PTNformatStop);
					file.fSkip();
					hogehoge = file.getString();
					findInt(sprD.x1, hogehoge);
					file.fSkip();
					hogehoge = file.getString();
					findInt(sprD.y1, hogehoge);
					file.fSkip();
					hogehoge = file.getString();
					findInt(sprD.x2, hogehoge);
					file.fSkip();
					hogehoge = file.getString();
					findInt(sprD.y2, hogehoge);
					file.fSkip();
					hogehoge = file.getString();
					findFloat(sprD.w, hogehoge);
					file.fSkip();
					hogehoge = file.getString();
					findFloat(sprD.h, hogehoge);
					file.fSkip();
					hogehoge = file.getString();
					findInt((int&)sprD.colorARGB, hogehoge);
					file.fSkip();
					hogehoge = file.getString();
					findFloat(sprD.w2, hogehoge);
					file.fSkip();
					hogehoge = file.getString();
					findFloat(sprD.h2, hogehoge);
					file.fSkip();

					hogehoge = file.getString();
					findInt(sprD.uint0, hogehoge);
					file.fSkip();
					hogehoge = file.getString();
					findInt(sprD.uint1, hogehoge);
					file.fSkip();
					hogehoge = file.getString();
					findInt(sprD.uint2, hogehoge);
					file.fSkip();
					hogehoge = file.getString();
					findFloat(sprD.uflt0, hogehoge);
					file.fSkip();
					hogehoge = file.getString();
					findFloat(sprD.uflt1, hogehoge);
					file.fSkip();
					hogehoge = file.getString();
					findInt(sprD.uint3, hogehoge);
					file.fSkip();

					sprDefSt.insert(sprDefSt.end(), sprD);
					hogera.ptnCnt++;
					ptnH.ptnTot++;
				}
				else if (tp == "Entry")
				{
					break;
				}
				else
				{
					printf("Error: \n");
					exit(3);
				}
			}
			hoge.insert(hoge.end(), { hogera, sprDefSt });
		}
		else
		{
			printf("Error: \n");
			exit(3);
		}
	}

	FILE* fp = fopen(f2, "wb");
	fwrite(&ptnH, sizeof(PTNHeader), 1, fp);
	for (int i = 0;i < ptnH.texCnt; i++)
	{
		fwrite(hoge[i].first.texName.c_str(), hoge[i].first.texName.length(), 1, fp);
		putc(0x00, fp);
		fwrite(&hoge[i].first.ptnCnt, 2, 1, fp);
		for (int j = 0; j < hoge[i].first.ptnCnt; j++)
		{
			fwrite(hoge[i].second[j].ptnName.c_str(), hoge[i].second[j].ptnName.length(), 1, fp);
			putc(0x00, fp);
			fwrite(&hoge[i].second[j].x1, 4, 9 + 6, fp);
		}
	}
	fclose(fp);
}
