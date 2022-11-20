#include "main_libs.h"

bool strSkip(char c)
{
	return c == '"';
}

bool numSkip(char c)
{
	if (c == '\n')
		line++;
	return c == ',' || c == ' ' || c == '	' || c == '\n' || c == 0x0d || c == ';';
}

bool skipSpace(char c)
{
	return c == ' ' || c == '	';
}

bool isBlankSpace(char c)
{
	if (c == '\n')
		line++;
	return skipSpace(c) || c == '\n' || c == 0x0d;
}

bool isValidChar(char c)
{
	return (c >= 'a' && c <= 'z') || 
		(c >= 'A' && c <= 'Z') || 
		(c >= '0' && c <= '9') || 
		c == '-' || c == '_';
}
namespace Clb
{
	
	bool getInt(const std::string& str, void* dest, size_t s)
	{
		bool sign = false;
		int size = str.length();
		long ret = 0;
		const char* temp = str.c_str();
		if (size != 0)
		{
			if (size > 2 && temp[0] == '0' && temp[1] == 'x')
			{
				for (int i = 2; i < size; i++)
				{
					switch (temp[i])
					{
					case '0': case '1': case '2': case '3': case '4':
					case '5': case '6': case '7': case '8': case '9':
						ret = (ret << 4) + (temp[i] - '0');
						break;
					case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
						ret = (ret << 4) + (temp[i] - 55);
						break;
					case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
						ret = (ret << 4) + (temp[i] - 87);
						break;
					default:
						return false;
					}
				}
			}
			else if (size > 2 && temp[0] == '0' && temp[1] == 'b')
			{
				for (int i = 2; i < size; i++)
				{
					switch (temp[i])
					{
					case '0':
						ret <<= 1;
						break;
					case '1':
						ret = (ret << 1) | 0b1;
						break;
					default:
						return false;
					}
				}
			}
			else
			{
				for (int i = 0; i < size; i++)
				{
					switch (temp[i])
					{
					case '-':
						if (!sign)
							sign = true;
						else
							return false;
						break;
					case '0': case '1': case '2': case '3': case '4':
					case '5': case '6': case '7': case '8': case '9':
						ret = (ret * 10) + (temp[i] - '0');
						break;
					default:
						return false;
					}
				}
			}

			if (sign)
				ret = -ret;
			memcpy(dest, &ret, s);
			return true;
		}
		return false;
	}

	bool getFloat(const std::string& str, float& dest)
	{
		double ret = 0.0, decimal = 0.0;
		int size = str.length(), div = 10;
		uint8_t pos = 1;
		bool dec = false, sign = false, great = true;
		const char* temp = str.c_str();

		for (int i = 0; i < size; i++)
		{
			if (!dec)
			{
			switch (temp[i])
			{
			case '-':
				if (!sign)
					sign = true;
				else
					return false;
				break;
			case '.':
				if (!dec)
				{
					great = ret <= 0;
					dec = true;
				}
				else
					return false;



				break;
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				ret = (ret * 10.0) + (temp[i] - '0');
				if (!great)
					pos++;
				break;
			case 'f':
				if (i == size - 1 && size > 1)
				{
					if (sign)
						dest = -ret;
					else
						dest = ret;
					return true;
				}
				else
					return false;
			default:
				return false;
			}
			}
			else
			{
				switch (temp[i])
				{
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9':
					decimal = (decimal * 10.0) + (temp[i] - '0');
					div *= 10.0;
					break;
				case 'f':
					if (i == size - 1)
					{
						if (sign)
							dest = -(ret + (decimal / div));
						else
							dest = (ret + (decimal / div));
						return true;
					}
					else
						return false;
				default:
						return false;
				}
			}
		}
		if (sign)
			dest = -(ret + (decimal / div));
		else
			dest = (ret + (decimal / div));
		return true;
	}

	/////////////////////////////////////////

	bool file::loadFile(const char* fName)
	{

		FILE* fp = fopen(fName, "rb");
		if (!fp)
		{
			printf("File not found.\n");
			exit(-1);
		}
		fseek(fp, 0, SEEK_END);
		fsize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		fsize -= ftell(fp);
		buffer = (char*)malloc(fsize);
		fread(buffer, fsize, 1, fp);
		fclose(fp);
		return buffer;
	}

	bool file::isEof()
	{
		return pos >= fsize;
	}

	char file::getCurrentChar() const
	{
		return buffer[pos];
	}

	char file::getCurrentChar(size_t offset) const
	{
		return buffer[pos + offset];
	}

	void file::getff(void* dest, size_t size)
	{
		if (!isEof())
		{
			memcpy(dest, buffer + pos, size);
			pos += size;
		}
	}

	void file::setFSkip(bool(*func)(char c))
	{
		sfunc = func;
	}
	void file::setRFormat(bool(*func)(char))
	{
		rfunc = func;
	}

	//
	void file::createFile(const char* nm)
	{
		this->fName = nm;
		this->fsize = 0;
		this->buffer = (char*)malloc(1);
	}

	void file::appendFile(void * src, size_t size)
	{
		size_t pSize = fsize;
		this->fsize += size;
		char* nBuffer = (char*)malloc(this->fsize);
		if ( nBuffer)
		{
			memcpy(nBuffer, this->buffer, pSize);
			memcpy(nBuffer + pSize, src, size);
			free(this->buffer);
			this->buffer = nBuffer;
		}
		else
		{
			printf("An error ocurred while re-allocating file.\n");
			exit(-1);
		}
	}

	void file::putc(char c)
	{
		char temp = c;
		appendFile(&temp, 1);
	}

	void file::writeFile()
	{
		FILE* fp = fopen(fName, "wb");
		fwrite(buffer, fsize, 1, fp);
		fclose(fp);
	}

	//Getting something...

	std::string file::getString()
	{
		std::string ret = "";
		for (; buffer[pos]; pos++)
		{
			if (isFormat && sfunc(buffer[pos]))
				break;
			if (!isEof())
			{
				if (isFormat)
				{
					if (buffer[pos] == '\\')
					{
						if(buffer[pos + 1] == '\\' || sfunc(buffer[pos + 1]))
							pos++;
						else
						{
							std::cout << "Unrecognized token. \n";
							exit(-1);
						}
					}
				}
				else 
				{
					if(sfunc(buffer[pos]) || buffer[pos] == '\\')
						ret.push_back('\\');
				}
				ret.push_back(buffer[pos]);
			}
		}
		if(!isFormat)
			pos++;
		return ret;
	}


	std::string file::getString(bool (*func)(char))
	{
		std::string ret = "";
		for (; func(buffer[pos]); pos++)
		{
			if (!isEof())
			{
				ret.push_back(buffer[pos]);
			}
		}
		return ret;
	}

	//Pos Manipulation
	size_t file::getSize() const { return fsize; }
	uint32_t file::getPos() const { return pos; }

	void file::setPos(size_t n) { pos = n; }

	bool file::movePos(size_t n) { pos += n; return pos + 1; }

	void file::fSkip()
	{
		while (sfunc(buffer[pos]) && !isEof() && isFormat)
			pos++;
	}


	//Constructor and Destructor
	file::file(bool form)
	{
		isFormat = form;
	}

	file::~file()
	{
		if(NULL != buffer)
			free(buffer);
	}
}