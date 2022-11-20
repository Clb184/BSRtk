#include "bsr_ECL.h"


void ECLdump(const char* fName, const char* mapfile)
{
	DefSCLIns = initDefMap();
	if(mapfile != NULL)
		SCLIns = initMap(mapfile);
	Clb::file file(false);
	file.loadFile(fName);
	file.setFSkip(&strSkip);
	getAddress(file);
	readIns(file);
}

void readIns(Clb::file& file)
{
	BYTE cmd;
	std::string temp; uint32_t tempd;
	BYTE bpar0;
	char bpar1;
	WORD wpar0;
	short wpar1;
	uint32_t dpar0;
	int dpar1;
	float fpar0;
	float fpar1;
	bool useMap;

	file.getff(&cmd, 1);
	while(cmd)
	{
		file.setPos(file.getPos() - 1);
		temp = file.getString();
		std::cout << "#ptnconst \"" << temp << "\"\n";
		file.getff(&cmd, 1);
	}
	std::cout << "\n";
	file.getff(&cmd, 1);
	while (cmd)
	{
		file.setPos(file.getPos() - 1);
		temp = file.getString();
		file.getff(&tempd, 4);
		file.getff(&cmd, 1);
		if (Subr.find(tempd + offset) != Subr.end())
		{
			Subr[tempd + offset] = temp;
		}
		if (ConstSubr.find(tempd + offset) != ConstSubr.end())
		{
			ConstSubr[tempd + offset] = temp;
		}
	}

#define B 	file.getff(&bpar0, 1); std::cout << (WORD)bpar0;
#define W 	file.getff(&wpar0, 2); std::cout << wpar0;
#define DW 	file.getff(&dpar0, 4); std::cout << dpar0;

#define CH 	file.getff(&bpar1, 1); std::cout << (short)bpar1;
#define SH 	file.getff(&wpar1, 2); std::cout << wpar1;
#define IN 	file.getff(&dpar1, 4); std::cout << dpar1;

#define F file.getff(&fpar0, 4); std::cout << fpar0 << "f";
#define H file.getff(&dpar0, 4); std::cout << std::hex << "0x" << dpar0 << std::dec;

#define STR std::cout << "\"" << file.getString() << "\"";
#define SUB(x) file.getff(&dpar0, 4); std::cout << Subr[file.getPos() + dpar0 - x]; 
#define LAB(x) file.getff(&dpar0, 4); std::cout << labl[file.getPos() + dpar0 - x]; 
#define C std::cout << ", ";
	while (!file.isEof())
	{
		if (ConstSubr.find(file.getPos()) != ConstSubr.end())
		{
			if (file.getPos() != offset)
			{
				std::cout << "}\n\n";
			}
			std::cout << "static sub " << ConstSubr[file.getPos()] << " {\n";
		}
		else if (Subr.find(file.getPos()) != Subr.end())
		{
			if (file.getPos() != offset)
			{
				std::cout << "}\n\n";
			}
			std::cout << "sub " << Subr[file.getPos()] << " {\n";
		}
		if (labl.find(file.getPos()) != labl.end())
		{
			std::cout << labl[file.getPos()] << ":\n";
		}
		file.getff(&cmd, 1);
		if (SCLIns.find(cmd) != SCLIns.end())
			std::cout << '	' << SCLIns[cmd].name << " ";
		else
			std::cout << "	CMD_" << (WORD)cmd << " ";
		switch (cmd)
		{
		case 0x00://
			SUB(4)
			break;
		case 0x01:
			B
				break;
		case 0x02: //Relative call?
			SUB(4)
			break;
		case 0x03: //Relative call?
			SUB(4) C W
				break;
		case 0x04: //Relative call?
			SUB(4)
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
			DW
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
			LAB(4)
			break;
		case 0x18:
			LAB(4)
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
		case 0x4b:
			break;
		case 0x4c://Another relative jump?
			LAB(4)
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
			B C STR
			break;
		case 0x53://Load Stage Background?
			B C STR
			break;
		case 0x54: //Load music?
			B C STR C STR
			break;
		case 0x55:
			W C LAB(4)
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
			W C B
			break;
		case 0x87:
			W C H
			break;
		case 0x88:
			STR
			break;
		case 0x89:
			SUB(4) C W
			break;
		case 0x8a:
			SUB(4) C W C W
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
			DW
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
		case 0xa7://Push Register?
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
			LAB(4)
			//file.getff(&dpar0, 4);
			//std::cout << dpar0;
			break;
		case 0xbc:
			W
				break;
		case 0xc9:
			LAB(4)
			break;
		default:
			std::cout << "Not documented in pos 0x"; printf("%x", file.getPos() - 1);
			_asm
			{
				nop
			}
			break;
		}

		std::cout << "\n";
	}
	std::cout << "}\n";
}

void getAddress(Clb::file& file)
{

	BYTE cmd;
	std::string temp; uint32_t tempd, tempd2;
	file.getff(&cmd, 1);
	while (cmd)
	{
		file.setPos(file.getPos() - 1);
		temp = file.getString();
		//std::cout << "\"" << temp << "\"\n";
		file.getff(&cmd, 1);
	}
	tempd2 = file.getPos();
	file.getff(&cmd, 1);
	while (cmd)
	{
		file.setPos(file.getPos() - 1);
		temp = file.getString();
		file.getff(&tempd, 4);
		file.getff(&cmd, 1);
	}
	offset = file.getPos();
	file.setPos(tempd2);
	file.getff(&cmd, 1);
	while (cmd)
	{
		file.setPos(file.getPos() - 1);
		temp = file.getString();
		file.getff(&tempd, 4);
		Subr.insert({ tempd + offset, temp });
		ConstSubr.insert({ tempd + offset, temp });
		//std::cout << "@" << temp << ", add: " << tempd << "\n";
		file.getff(&cmd, 1);
	}
	BYTE bpar0;
	char bpar1;
	WORD wpar0;
	short wpar1;
	uint32_t dpar0;
	int dpar1;
	float fpar0;
	float fpar1;


#define B 	file.setPos(file.getPos() + 1);
#define W 	file.setPos(file.getPos() + 2);
#define DW 	file.setPos(file.getPos() + 4);

#define CH 	B
#define SH 	W
#define IN 	DW

#define F DW

#define STR file.getString();
#define SUB(x) file.getff(&dpar0, 4);\
	temp = "Sub" + std::to_string(Subr.size() + 1);\
	Subr.insert( {file.getPos() + dpar0 - x, temp}); 
#define LAB(x) file.getff(&dpar0, 4);\
	temp = "Lab" + std::to_string(labl.size() + 1);\
	labl.insert( {file.getPos() + dpar0 - x, temp}); 
#define C 
	temp = "Sub0";
	Subr.insert({file.getPos(), temp});
	while (!file.isEof())
	{
#ifdef CLB_DEBUG
		std::cout << "\n" << std::hex << file.getPos() << std::dec;
#endif // CLB_DEBUG

		file.getff(&cmd, 1);

#ifdef CLB_DEBUG
		std::cout << " CMD_" << (WORD)cmd << std::hex << " - 0x" << (WORD)cmd << std::dec;
#endif // CLB_DEBUG

		switch (cmd)
		{
		case 0x00://size 5 b
			SUB(4)
			break;
		case 0x01:
			B
				break;
		case 0x02: //Relative call?
			SUB(4)
			break;
		case 0x03: //Relative call?
			SUB(4) C W
				break;
		case 0x04: //Relative call?
			SUB(4)
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
			DW
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
			LAB(4)
				break;
		case 0x18:
			LAB(4)
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
		case 0x3a://GreatEq?
			break;
		case 0x3b:
			break;
		case 0x3c:
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
		case 0x4b:
			break;
		case 0x4c://Another relative jump?
			LAB(4)
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
			B C STR
				break;
		case 0x53://Load Stage Background?
			B C STR
				break;
		case 0x54: //Load music?
			B C STR C STR
				break;
		case 0x55:
			W C LAB(4)
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
			W C DW
			break;
		case 0x84:

			W C DW
				break;
		case 0x85:
			W C B
				break;
		case 0x87:
			W C DW
				break;
		case 0x88:

			STR
				break;
		case 0x89://Anm task
			SUB(4) C W
				break;
		case 0x8a:
			SUB(4) C W C W
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
			DW
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
			W C W
				//if (dpar0 > 0xffffff00)
					//std::cout << (float)dpar0 << "f";
				//else
					//std::cout << wpar0 << ", " << wpar1;
				break;
		case 0xa6:
			W
				break;
		case 0xa7://Push register
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
		case 0xb2:
			B
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
			LAB(4)
				//file.getff(&dpar0, 4);
				//std::cout << dpar0;
			break;
		case 0xbc:
			W
			break;
		case 0xc9:
			LAB(4)
			break;
		default:
			std::cout << " 0x" << std::hex << (WORD)cmd << std::dec << " Not documented in pos 0x"; printf("%x", file.getPos() - 1);
			_asm
			{
				nop
			}
			break;
		}

		//std::cout << "\n";
	}
	int id = 0;
	for (Address::iterator i = Subr.begin(); i != Subr.end(); ++i )
	{
		std::string tname = "Sub";
		tname += std::to_string(id);
		i->second = tname;
		id++;
	}
	for (Address::iterator i = labl.begin(); i != labl.end(); ++i)
	{
		std::string tname = "Lab_";
		tname += std::to_string(i->first);
		i->second = tname;
	}
	file.setPos(0);
}