//Script compiler for "Seihou 3 - Banshiryuu" by Clb184
/*
================================
-2022/11/14 : Start.

================================
*/

#include "bsr_ECL.h"

#define SCLCOMP_ERROR(str) \
{std::cout << currentFile << " - Error in line " << line  << " pos 0x" << std::hex << file.getPos() << std::dec << ": " << str;\
exit(6);}

vAddress vLabl;

static bool ECLFormat(char c)
{
	if (c == '\n')
		line++;
	return c == ' ' || c == '	' || c == '\n' || c == ',' || c == '"' || c == '{' || c == '}' || c == 0x0d;
}

inline bool isKeyword(const std::string& str)
{
	return SCLKeyWord.find(str) != SCLKeyWord.end();
}

inline bool isCompilerDirect(const std::string& str)
{
	return CompilerDirect.find(str) != CompilerDirect.end();
}

inline bool isInstruction(const std::string& str, uint8_t& cmd)
{
	for(InsMap::iterator i = SCLIns.begin(); i != SCLIns.end(); ++i)
	{
		if (i->second.name == str)
		{
			cmd = i->first;
			return true;
		}
	}
	for (InsMap::iterator i = DefSCLIns.begin(); i != DefSCLIns.end(); ++i)
	{
		if (i->second.name == str)
		{
			cmd = i->first;
			return true;
		}
	}
	return false;
}

void ECLComp(const char* in,const char* out, const char* mapfile)
{
	DefSCLIns = initDefMap();
	if (mapfile != NULL)
		SCLIns = initMap(mapfile);

	currentFile = in;

	Clb::file file(true);
	Clb::file binf(false);
	file.loadFile(currentFile);

	CGetAddress(file);

	binf.createFile(out);
	//file.loadFile(currentFile);
	CReadIns(file, binf);
	binf.writeFile();
}

void getIFileAdd(const char* in)
{
	const char* prevf = currentFile;
	uint32_t prevLine = line;
	currentFile = in;

	Clb::file file(true);
	file.loadFile(currentFile);

	CGetAddress(file);
	currentFile = prevf;
	line = prevLine;
}

void getIFileIns(const char* in, Clb::file& out)
{
	const char* prevf = currentFile;
	uint32_t prevLine = line;
	currentFile = in;

	Clb::file file(true);
	file.loadFile(currentFile);

	CReadIns(file, out);
	currentFile = prevf;
	line = prevLine;
}

void CReadIns(Clb::file& file, Clb::file& out)
{
	uint32_t pos = 0;
	uint8_t cmd;
	std::string temp, tempN, tempStr, subName;
	vAddress vLabl;
	bool onSub = false, isConst;
	
	uint8_t tb;
	uint16_t tw;
	uint32_t tdw;
	int tadd;
	float tf;
	char tc;
	short ts;
	int ti;
	for (std::vector<std::string>::iterator i = PTNConst.begin(); i != PTNConst.end(); ++i)
	{
		out.appendFile((void*)i->c_str(), i->length());
		out.putc(0x00);
	}
	out.putc(0x00);
	for (vAddress::iterator i = vConstSubr.begin(); i != vConstSubr.end(); ++i)
	{
		out.appendFile((void*)i->first.c_str(), i->first.length());
		out.putc(0x00);
		out.appendFile(&i->second, 4);
	}
	out.putc(0x00);
#define B 	\
		file.fSkip();\
		file.setFSkip(&ECLFormat);\
		tempN = file.getString();\
		file.setFSkip(&isBlankSpace);\
		findInt2(tb, tempN, 1)\
		out.appendFile(&tb, 1);\
		 pos++;

#define W 	\
		file.fSkip();\
		file.setFSkip(&ECLFormat);\
		tempN = file.getString();\
		file.setFSkip(&isBlankSpace);\
		findInt2(tw, tempN, 2)\
		out.appendFile(&tw, 2);\
		 pos+=2;
#define DW \
		file.fSkip();\
		file.setFSkip(&ECLFormat);\
		tempN = file.getString();\
		file.setFSkip(&isBlankSpace);\
		findInt2(tdw, tempN, 4)\
		out.appendFile(&tdw, 4);\
		 pos+=4;

#define CH 	\
		file.fSkip();\
		file.setFSkip(&ECLFormat);\
		tempN = file.getString();\
		file.setFSkip(&isBlankSpace);\
		findInt2(tc, tempN, 1)\
		out.appendFile(&tc, 1);\
		 pos++;

#define SH 	\
		file.fSkip();\
		file.setFSkip(&ECLFormat);\
		tempN = file.getString();\
		file.setFSkip(&isBlankSpace);\
		findInt2(ts, tempN, 2)\
		out.appendFile(&ts, 2);\
		 pos+=2;
#define IN \
		file.fSkip();\
		file.setFSkip(&ECLFormat);\
		tempN = file.getString();\
		file.setFSkip(&isBlankSpace);\
		findInt2(ti, tempN, 4)\
		out.appendFile(&ti, 4);\
		 pos+=4;

#define F  \
		file.fSkip();\
		file.setFSkip(&ECLFormat);\
		tempN = file.getString();\
		file.setFSkip(&isBlankSpace);\
		findFloat(tf, tempN)\
		out.appendFile(&tf, 4);\
		 pos+=4;

#define H \
		file.fSkip();\
		file.setFSkip(&ECLFormat);\
		tempN = file.getString();\
		file.setFSkip(&isBlankSpace);\
		findVar2(tdw, tempN, 4)\
		out.appendFile(&tdw, 4);\
		 pos+=4;

#define STR \
		file.fSkip(); \
		file.setFSkip(&strSkip); \
	if (file.getCurrentChar() == '\"') \
	{ \
			file.movePos(1); \
			tempStr = file.getString(); \
			if (file.getCurrentChar() == '\"') { \
				file.movePos(1); \
				file.setFSkip(&isBlankSpace); \
				out.appendFile((void*)tempStr.c_str(), tempStr.length());\
				out.putc(0x00);\
				pos += tempStr.length() + 1; } \
			else \
			{ \
				SCLCOMP_ERROR("Incorrect end of string.\n")\
			} \
	} \
	else \
	{ \
		SCLCOMP_ERROR("Start of String not found.\n")\
	}
#define SUB  \
			file.fSkip(); \
			tempStr = file.getString(&isValidChar); \
			file.setFSkip(&isBlankSpace); \
			tadd = vLablContainer[tempStr].address - pos;\
			out.appendFile(&tadd, 4);\
			pos += 4;
#define LAB \
			file.fSkip(); \
			tempStr = file.getString(&isValidChar); \
			file.setFSkip(&isBlankSpace); \
			tadd = vLablContainer[subName].localLab[tempStr] - pos;\
			out.appendFile(&tadd, 4);\
			pos += 4;

#define C	\
		file.setFSkip(&isBlankSpace);\
		file.fSkip();\
		if(file.getCurrentChar() == ',') {file.movePos(1);} else {	SCLCOMP_ERROR("Comma not found.\n")}

	while (file.getPos() < file.getSize() - 2)
	{
		if (!onSub)
		{
			//if (vLabl.size() > 0)
			//{
			//	//vLablContainer.insert({ subName, tempSbr});
			//	vLabl.clear();
			//}
			if (LocalConstFloat.size() > 0)
				LocalConstFloat.clear();
			if (LocalConstInt.size() > 0)
				LocalConstInt.clear();
		}
		file.setFSkip(&isBlankSpace);
		file.fSkip();
		if (file.getCurrentChar() == '#')
		{
			file.movePos(1);
			temp = file.getString(&isValidChar);
			if (isCompilerDirect(temp))
			{
				if (temp == "ptnconst")
				{
					file.fSkip();
					if (file.getCurrentChar() == '\"')
					{
						file.movePos(1);
						file.setFSkip(&strSkip);
						temp = file.getString();
						//PTNConst.insert({ temp, temp });

						if (file.getCurrentChar() == '\"')
						{
							file.movePos(1);
						}
						else
						{
							SCLCOMP_ERROR("End of string (ptnconst) not found.\n")
						}
					}
					else
					{
						SCLCOMP_ERROR("Start of string (ptnconst) not found.\n")
					}
				}
				else if (temp == "define")
				{
					uint32_t tInt;
					float tFloat;
					file.fSkip();
					file.setFSkip(&ECLFormat);
					temp = file.getString();
					file.setFSkip(&isBlankSpace);
					file.fSkip();
					tempN = file.getString();
					if (Clb::getInt(tempN, &tInt, 4))
					{
						if (onSub)
							LocalConstInt.insert({ temp, tInt });
						else
							GlobalConstInt.insert({ temp, tInt });
					}
					else if (Clb::getFloat(tempN, tFloat))
					{
						if (onSub)
							LocalConstFloat.insert({ temp, tFloat });
						else
							GlobalConstFloat.insert({ temp, tFloat });
					}
					//To do: Define a constant as another constant
					//else if (LocalConstInt.find(tempN) != LocalConstInt.end())
					//{
					//
					//}
					//else if (LocalConstFloat.find(tempN) != LocalConstFloat.end())
					//{
					//
					//}
					else
						SCLCOMP_ERROR("Constant not found.\n")
				}
				else if (temp == "include")
				{
					file.fSkip();
					if (file.getCurrentChar() == '\"')
					{
						file.movePos(1);
						file.setFSkip(&strSkip);
						temp = file.getString();
						if (file.getCurrentChar() == '\"')
						{
							file.movePos(1);
							getIFileIns(temp.c_str(), out);
						}
						else
						{
							SCLCOMP_ERROR("End of string (include) not found.\n")
						}
					}
					else
					{
						SCLCOMP_ERROR("Start of string (include) not found.\n")
					}
				}
				else if (temp == "mapfile")
				{
					file.fSkip();
					if (file.getCurrentChar() == '\"')
					{
						file.movePos(1);
						file.setFSkip(&strSkip);
						temp = file.getString();
						if (file.getCurrentChar() == '\"')
						{
							file.movePos(1);
						}
						else
						{
							SCLCOMP_ERROR("End of string (mapfile) not found.\n")
						}
					}
					else
					{
						SCLCOMP_ERROR("Start of string (mapfile) not found.\n")
					}
				}
			}
		}
		else if (file.getCurrentChar() == '}')
		{
			if (onSub)
			{
				file.movePos(1);
				onSub = false;
			}
			else
			{
				SCLCOMP_ERROR("Extra \"}\".")
			}
		}
		else
		{
			temp = file.getString(&isValidChar);
			//file.setPos(file.getPos() - 1);
			if (isKeyword(temp))
			{
				file.setFSkip(&isBlankSpace);
				file.fSkip();
				if (isConst = temp == "static")
				{
					temp = file.getString(&isValidChar);
					file.fSkip();
				}
				if (onSub = temp == "sub")
				{
					subName = file.getString(&isValidChar);
					file.fSkip();
					if (file.getCurrentChar() == '{')
					{
						file.movePos(1);
						//vSubr.insert({ subName, pos });
						if (isConst)
						{
							//vConstSubr.insert({ subName, pos });
							isConst = false;
						}
					}
				}
			}

			else if (isInstruction(temp, cmd))
			{
				out.putc(cmd);
				pos++;
				switch (cmd)
				{
				case 0x00://
					SUB
						break;
				case 0x01:
					B
						break;
				case 0x02: //Relative call?
					SUB
						break;
				case 0x03: //Relative call?
					SUB C W
						break;
				case 0x04: //Relative call?
					SUB
						break;
				case 0x08:
					W
						break;
				case 0x09:
					break;
				case 0x0a:
					B
						break;
				case 0x0b:
					LAB
						break;
				case 0x0c:
					break;
				case 0x0d:
					break;
				case 0x0e:
					break;
				case 0x0f:
					break;
				case 0x10:
					break;
				case 0x11:
					break;
				case 0x12:
					break;
				case 0x13:
					break;
				case 0x14:
					break;
				case 0x15:
					break;
				case 0x16:
					break;
				case 0x17:
					LAB
						break;
				case 0x18:
					LAB
						break;
				case 0x19:
					break;
				case 0x1a:
					break;
				case 0x1b:
					break;
				case 0x1c:
					break;
				case 0x1d:
					break;
				case 0x1e:
					break;
				case 0x1f:
					break;
				case 0x20:
					break;
				case 0x21:
					break;
				case 0x22:
					break;
				case 0x23:
					break;
				case 0x25:
					break;
				case 0x27:
					break;
				case 0x28:
					break;
				case 0x29:
					break;
				case 0x2b:
					break;
				case 0x2c:
					break;
				case 0x2d:
					break;
				case 0x30:
					break;
				case 0x31:
					break;
				case 0x32:
					break;
				case 0x33:
					break;
				case 0x34:
					break;
				case 0x35:
					break;
				case 0x36:
					break;
				case 0x37:
					break;
				case 0x38:
					break;
				case 0x39:
					break;
				case 0x3a:
					break;
				case 0x3b:
					break;
				case 0x3c:
					//B
					break;
				case 0x3d:
					break;
				case 0x3e:
					break;
				case 0x3f:
					break;
				case 0x40:
					W
						break;
				case 0x41:
					//W
					break;
				case 0x42:
					break;
				case 0x43:
					break;
				case 0x46:
					break;
				case 0x47:
					break;
				case 0x48:
					break;
				case 0x49:
					break;
				case 0x4a:
					break;
				case 0x4b:
					break;
				case 0x4c://Another relative jump?
					LAB
						break;
				case 0x4d:
					W
						break;
				case 0x4e:
					break;
				case 0x4f:
					B
						break;
				case 0x50:
					break;
				case 0x51:
					break;
				case 0x52:
					B
						C
						STR
						break;
				case 0x53://Load Stage Background?
					B C STR
						break;
				case 0x54: //Load music?
					B
						C
						STR C
						STR
						break;
				case 0x55:
					W C LAB
						break;
				case 0x56:
					break;
				case 0x57:
					break;
				case 0x58:
					W
						break;
				case 0x59:
					break;
				case 0x5b:
					W
						break;
				case 0x5c:
					B
						break;
				case 0x5e:
					break;
				case 0x61:
					break;
				case 0x63:
					break;
				case 0x64:
					break;
				case 0x65:
					break;
				case 0x66:
					break;
				case 0x67:
					break;
				case 0x68:
					break;
				case 0x69:
					break;
				case 0x6a:
					break;
				case 0x6b:
					break;
				case 0x6c:
					break;
				case 0x6d:
					break;
				case 0x6e:
					break;
				case 0x6f:
					break;
				case 0x70:
					break;
				case 0x71:
					break;
				case 0x72:
					break;
				case 0x73:
					break;
				case 0x74:
					break;
				case 0x75:
					W
						break;
				case 0x76:
					break;
				case 0x77:
					break;
				case 0x7a:
					break;
				case 0x7c:
					break;
				case 0x7d:
					break;
				case 0x7f:
					break;
				case 0x80:
					break;
				case 0x81:
					break;
				case 0x82:
					break;
				case 0x83:
					W C H
						break;
				case 0x84:
					W C H
						break;
				case 0x85:
					W
						C
						B
						break;
				case 0x87:
					W C H
						break;
				case 0x88:
					STR
						break;
				case 0x89:
					SUB C W
						break;
				case 0x8a:
					SUB C W C W
						break;
				case 0x8b:
					break;
				case 0x8c:
					break;
				case 0x8d://Jump? return?
					//file.getff(&dpar1, 4);
					//std::cout << (int)dpar1;
					break;
				case 0x8e:
					break;
				case 0x90:
					SUB
						break;
				case 0x91:
					break;
				case 0x92:
					W
						break;
				case 0x94:
					W
						break;
				case 0x95:
					W
						break;
				case 0x96:
					W
						break;
				case 0x97:
					W
						break;
				case 0x99:
					W
						break;
				case 0x9b:
					W
						break;
				case 0x9d:
					W
						break;
				case 0x9e:
					W
						break;
				case 0x9f:
					W
						break;
				case 0xa0:
					B
						break;
				case 0xa1:
					W
						break;
				case 0xa2:
					B
						break;
				case 0xa3:
					B
						break;
				case 0xa5:
					//W C W
					H
						//if (dpar0 > 0xffffff00)
							//std::cout << (float)dpar0 << "f";
						//else
							//std::cout << wpar0 << ", " << wpar1;
						break;
				case 0xa6:
					W
						break;
				case 0xa7:
					W
						break;
				case 0xa8:
					W
						break;
				case 0xa9:
					W
						break;
				case 0xaa:// return?
					W
						break;
				case 0xad://0404E8FB
					W
						break;
				case 0xae:
					W
						break;
				case 0xb0:
					W

						break;
				case 0xb1://Write line of text
					STR
						break;
				case 0xb2://Return?
					B
						//std::cout <<  "\n";
						break;
				case 0xb3:
					break;
				case 0xb4:
					break;
				case 0xb5: //Load texture?
					STR
						break;
				case 0xb6:
					break;
				case 0xb7://Relative jump if?
					LAB
						//file.getff(&dpar0, 4);
						//std::cout << dpar0;
						break;
				case 0xbc:
					W
						break;
				case 0xc9:
					LAB
						break;
				default:
					std::cout << "Not documented in pos 0x"; printf("%x", file.getPos() - 1);
					_asm
					{
						nop
					}
					break;
				}
			}
			else
			{
				file.fSkip();
				if (file.getCurrentChar() == ':')
				{
					if (onSub)
					{
						file.movePos(1);
						if ((vLabl.find(temp) == vLabl.end()))
							;// vLabl.insert({ temp, pos });
						else
							SCLCOMP_ERROR("Local label \"" << temp << "\" already exists.\n")
					}
					else
						SCLCOMP_ERROR("Can't create local label outsibe subroutine.\n")
				}
				else
				{
					SCLCOMP_ERROR("Error in line " << line << " Not valid keyword.\n")
				}
			}
		}
		//std::cout << "\n";
	}
}

/* 
	First read:
	calculating address, getting const subroutines
	*/
void CGetAddress(Clb::file& file)
{
	uint32_t pos = 0;
	uint8_t cmd;
	std::string temp, tempN, tempStr, subName;
	bool onSub = false, isConst;
	Subroutine tempSbr;

#define B 	\
		file.fSkip();\
		file.setFSkip(&ECLFormat);\
		tempN = file.getString();\
		file.setFSkip(&isBlankSpace);\
		 pos++;
		
#define W 	\
		file.fSkip();\
		file.setFSkip(&ECLFormat);\
		tempN = file.getString();\
		file.setFSkip(&isBlankSpace);\
		 pos+=2;
#define DW \
		file.fSkip();\
		file.setFSkip(&ECLFormat);\
		tempN = file.getString();\
		file.setFSkip(&isBlankSpace);\
		 pos+=4;

#define CH 	B
#define SH 	W
#define IN 	DW

#define F DW
#define H DW

#define STR \
		file.fSkip(); \
		file.setFSkip(&strSkip); \
	if (file.getCurrentChar() == '\"') \
	{ \
			file.movePos(1); \
			tempStr = file.getString(); \
			if (file.getCurrentChar() == '\"') { \
				file.movePos(1); \
				file.setFSkip(&isBlankSpace); \
				pos += tempStr.length() + 1; } \
			else \
			{ \
				SCLCOMP_ERROR("Incorrect end of string.\n")\
			} \
	} \
	else \
	{ \
		SCLCOMP_ERROR("Start of String not found.\n")\
	}
#define SUB  \
			file.fSkip(); \
			tempStr = file.getString(&isValidChar); \
			file.setFSkip(&isBlankSpace); \
			pos += 4;
#define LAB SUB
#define C	\
		file.setFSkip(&isBlankSpace);\
		file.fSkip();\
		if(file.getCurrentChar() == ',') {file.movePos(1);} else {	SCLCOMP_ERROR("Comma not found.\n")}
		
	while (file.getPos() < file.getSize() - 2)
	{
		if (!onSub)
		{
			if (vLabl.size() > 0)
			{
				vLablContainer.insert({ subName,tempSbr });
				vLabl.clear();
				tempSbr.localLab.clear();
			}
			if(LocalConstFloat.size() > 0)
				LocalConstFloat.clear();
			if (LocalConstInt.size() > 0)
				LocalConstInt.clear();
		}
		file.setFSkip(&isBlankSpace);
		file.fSkip();
		if (file.getCurrentChar() == '#')
		{
			file.movePos(1);
			temp = file.getString(&isValidChar);
				if (isCompilerDirect(temp))
				{
					if (temp == "ptnconst")
					{
						file.fSkip();
						if (file.getCurrentChar() == '\"')
						{
							file.movePos(1);
							file.setFSkip(&strSkip);
							temp = file.getString();
							PTNConst.insert(PTNConst.end(), temp);

							if (file.getCurrentChar() == '\"')
							{
								file.movePos(1);
							}
							else
							{
								SCLCOMP_ERROR("End of string (ptnconst) not found.\n")
							}
						}
						else
						{
							SCLCOMP_ERROR("Start of string (ptnconst) not found.\n")
						}
					}
					else if (temp == "define")
					{
						uint32_t tInt;
						float tFloat;
						file.fSkip();
						file.setFSkip(&ECLFormat);
						temp = file.getString();
						file.setFSkip(&isBlankSpace);
						file.fSkip();
						tempN = file.getString();
						//if (Clb::getInt(temp, &tInt, 4))
						//{
						//	if(onSub)
						//		LocalConstInt.insert({ temp, tInt });
						//	else
						//		GlobalConstInt.insert({temp, tInt});
						//}
						//else if (Clb::getFloat(temp, tFloat))
						//{
						//	if (onSub)
						//	LocalConstFloat.insert({ temp, tFloat });
						//	else
						//		GlobalConstFloat.insert({ temp, tFloat });
						//}
						//To do: Define a constant as another constant
						//else if (LocalConstInt.find(temp) != LocalConstInt.end())
						//{
						//
						//}
						//else if (LocalConstFloat.find(temp) != LocalConstFloat.end())
						//{
						//
						//}
						//else
						//	SCLCOMP_ERROR("Constant not found.\n")
					}
					else if (temp == "include")
					{
						file.fSkip();
						if (file.getCurrentChar() == '\"')
						{
							file.movePos(1);
							file.setFSkip(&strSkip);
							temp = file.getString();
							if (file.getCurrentChar() == '\"')
							{
								file.movePos(1);
								getIFileAdd(temp.c_str());
							}
							else
							{
								SCLCOMP_ERROR("End of string (include) not found.\n")
							}
						}
						else
						{
							SCLCOMP_ERROR("Start of string (include) not found.\n")
						}
					}
					else if (temp == "mapfile")
					{
						file.fSkip();
						if (file.getCurrentChar() == '\"')
						{
							file.movePos(1);
							file.setFSkip(&strSkip);
							temp = file.getString();
							if (file.getCurrentChar() == '\"')
							{
								file.movePos(1);
								initMap(temp.c_str());
							}
							else
							{
								SCLCOMP_ERROR("End of string (mapfile) not found.\n")
							}
						}
						else
						{
							SCLCOMP_ERROR("Start of string (mapfile) not found.\n")
						}
					}
				}
		}
		else if (file.getCurrentChar() == '}')
		{
			if (onSub)
			{
				file.movePos(1);
				onSub = false;
			}
			else
			{
				SCLCOMP_ERROR("Extra \"}\".")
			}
		}
		else
		{
			temp = file.getString(&isValidChar);
			//file.setPos(file.getPos() - 1);
			if (isKeyword(temp))
			{
				file.setFSkip(&isBlankSpace);
				file.fSkip();
				if (isConst = temp == "static")
				{
					temp = file.getString(&isValidChar);
					file.fSkip();
				}
				if (onSub = temp == "sub")
				{
					subName = file.getString(&isValidChar);
					file.fSkip();
					if (file.getCurrentChar() == '{')
					{
						file.movePos(1);
						if (vLablContainer.find(subName) == vLablContainer.end())
						{
							tempSbr.address = pos;
							vLablContainer.insert({ subName, tempSbr });
							if (isConst)
							{
								vConstSubr.insert({ subName, pos });
								isConst = false;
							}
						}
						else
						{
							SCLCOMP_ERROR("Duplicated Subroutine [" << subName << "] ")
						}
					}
				}
			}
		
			else if (isInstruction(temp, cmd))
			{
				pos++;
				switch (cmd)
				{
				case 0x00://
					SUB
						break;
				case 0x01:
					B
						break;
				case 0x02: //Relative call?
					SUB
						break;
				case 0x03: //Relative call?
					SUB C W
						break;
				case 0x04: //Relative call?
					SUB
						break;
				case 0x08:
					W
						break;
				case 0x09:
					break;
				case 0x0a:
					B
						break;
				case 0x0b:
					LAB
						break;
				case 0x0c:
					break;
				case 0x0d:
					break;
				case 0x0e:
					break;
				case 0x0f:
					break;
				case 0x10:
					break;
				case 0x11:
					break;
				case 0x12:
					break;
				case 0x13:
					break;
				case 0x14:
					break;
				case 0x15:
					break;
				case 0x16:
					break;
				case 0x17:
					LAB
						break;
				case 0x18:
					LAB
						break;
				case 0x19:
					break;
				case 0x1a:
					break;
				case 0x1b:
					break;
				case 0x1c:
					break;
				case 0x1d:
					break;
				case 0x1e:
					break;
				case 0x1f:
					break;
				case 0x20:
					break;
				case 0x21:
					break;
				case 0x22:
					break;
				case 0x23:
					break;
				case 0x25:
					break;
				case 0x27:
					break;
				case 0x28:
					break;
				case 0x29:
					break;
				case 0x2b:
					break;
				case 0x2c:
					break;
				case 0x2d:
					break;
				case 0x30:
					break;
				case 0x31:
					break;
				case 0x32:
					break;
				case 0x33:
					break;
				case 0x34:
					break;
				case 0x35:
					break;
				case 0x36:
					break;
				case 0x37:
					break;
				case 0x38:
					break;
				case 0x39:
					break;
				case 0x3a:
					break;
				case 0x3b:
					break;
				case 0x3c:
					//B
					break;
				case 0x3d:
					break;
				case 0x3e:
					break;
				case 0x3f:
					break;
				case 0x40:
					W
						break;
				case 0x41:
					//W
					break;
				case 0x42:
					break;
				case 0x43:
					break;
				case 0x46:
					break;
				case 0x47:
					break;
				case 0x48:
					break;
				case 0x49:
					break;
				case 0x4a:
					break;
				case 0x4b:
					break;
				case 0x4c://Another relative jump?
					LAB
						break;
				case 0x4d:
					W
						break;
				case 0x4e:
					break;
				case 0x4f:
					B
						break;
				case 0x50:
					break;
				case 0x51:
					break;
				case 0x52:
					B
						C
						STR
						break;
				case 0x53://Load Stage Background?
					B C STR
						break;
				case 0x54: //Load music?
					B 
						C 
						STR C 
						STR
						break;
				case 0x55:
					W C LAB
						break;
				case 0x56:
					break;
				case 0x57:
					break;
				case 0x58:
					W
						break;
				case 0x59:
					break;
				case 0x5b:
					W
						break;
				case 0x5c:
					B
						break;
				case 0x5e:
					break;
				case 0x61:
					break;
				case 0x63:
					break;
				case 0x64:
					break;
				case 0x65:
					break;
				case 0x66:
					break;
				case 0x67:
					break;
				case 0x68:
					break;
				case 0x69:
					break;
				case 0x6a:
					break;
				case 0x6b:
					break;
				case 0x6c:
					break;
				case 0x6d:
					break;
				case 0x6e:
					break;
				case 0x6f:
					break;
				case 0x70:
					break;
				case 0x71:
					break;
				case 0x72:
					break;
				case 0x73:
					break;
				case 0x74:
					break;
				case 0x75:
					W
						break;
				case 0x76:
					break;
				case 0x77:
					break;
				case 0x7a:
					break;
				case 0x7c:
					break;
				case 0x7d:
					break;
				case 0x7f:
					break;
				case 0x80:
					break;
				case 0x81:
					break;
				case 0x82:
					break;
				case 0x83:
					W C H
						break;
				case 0x84:
					W C H
						break;
				case 0x85:
					W 
						C 
						B
						break;
				case 0x87:
					W C H
						break;
				case 0x88:
					STR
						break;
				case 0x89:
					SUB C W
						break;
				case 0x8a:
					SUB C W C W
						break;
				case 0x8b:
					break;
				case 0x8c:
					break;
				case 0x8d://Jump? return?
					//file.getff(&dpar1, 4);
					//std::cout << (int)dpar1;
					break;
				case 0x8e:
					break;
				case 0x90:
					SUB
						break;
				case 0x91:
					break;
				case 0x92:
					W
						break;
				case 0x94:
					W
						break;
				case 0x95:
					W
						break;
				case 0x96:
					W
						break;
				case 0x97:
					W
						break;
				case 0x99:
					W
						break;
				case 0x9b:
					W
						break;
				case 0x9d:
					W
						break;
				case 0x9e:
					W
						break;
				case 0x9f:
					W
						break;
				case 0xa0:
					B
						break;
				case 0xa1:
					W
						break;
				case 0xa2:
					B
						break;
				case 0xa3:
					B
						break;
				case 0xa5:
					//W C W
					H
						//if (dpar0 > 0xffffff00)
							//std::cout << (float)dpar0 << "f";
						//else
							//std::cout << wpar0 << ", " << wpar1;
						break;
				case 0xa6:
					W
						break;
				case 0xa7:
					W
						break;
				case 0xa8:
					W
						break;
				case 0xa9:
					W
						break;
				case 0xaa:// return?
					W
						break;
				case 0xad://0404E8FB
					W
						break;
				case 0xae:
					W
						break;
				case 0xb0:
					W

						break;
				case 0xb1://Write line of text
					STR
						break;
				case 0xb2://Return?
					B
						//std::cout <<  "\n";
						break;
				case 0xb3:
					break;
				case 0xb4:
					break;
				case 0xb5: //Load texture?
					STR
						break;
				case 0xb6:
					break;
				case 0xb7://Relative jump if?
					LAB
						//file.getff(&dpar0, 4);
						//std::cout << dpar0;
						break;
				case 0xbc:
					W
						break;
				case 0xc9:
					LAB
						break;
				default:
					std::cout << "Not documented in pos 0x"; printf("%x", file.getPos() - 1);
					_asm
					{
						nop
					}
					break;
				}
			}
			else
			{
				file.fSkip();
				if (file.getCurrentChar() == ':')
				{
					if (onSub)
					{
						file.movePos(1);
						if ((vLabl.find(temp) == vLabl.end()))
							vLablContainer[subName].localLab.insert({temp, pos});
						else
							SCLCOMP_ERROR("Local label \"" << temp << "\" already exists.\n")
					}
					else
						SCLCOMP_ERROR("Can't create local label outsibe subroutine.\n")
				}
				else
				{
					SCLCOMP_ERROR("Error in line " << line << " Not valid keyword.\n")
				}
			}
		}
		//std::cout << "\n";
	}
	file.setPos(0);
}
