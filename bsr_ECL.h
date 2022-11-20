#include "main_libs.h"
#pragma once

struct Subroutine;
struct Ins;

typedef std::map<uint32_t, std::string> Address;
typedef std::map<std::string, uint32_t> vAddress;
typedef std::map<BYTE, Ins> InsMap;
typedef std::map<std::string, std::string> Keyword;

static InsMap DefSCLIns;
static InsMap SCLIns;

static uint32_t offset;
static Address ConstSubr;
static Address Subr;
static Address labl;

static vAddress vSubr;
static vAddress vConstSubr;
static std::map<std::string, Subroutine> vLablContainer;

static std::vector<std::string> PTNConst;


struct Subroutine
{
	uint32_t address;
	vAddress localLab;
};

struct Ins
{
	std::string name;
};

static Keyword SCLKeyWord
{
	{"static", "static"},
	{"sub", "sub"}
};

static Keyword CompilerDirect
{
	{"include", "include"},
	{"ptnconst", "ptnconst"},
	{"define", "define"},
	{"mapfile", "mapfile"}
};

//Instruction Mapping
InsMap initDefMap();
InsMap initMap(const char*);

void ECLdump(const char*, const char*);
void readIns(Clb::file&);
void getAddress(Clb::file&);

void ECLComp(const char*, const char*, const char*);
void getIFileIns(const char*, Clb::file&);
void getIFileAdd(const char*);
void CReadIns(Clb::file&, Clb::file&);
void CGetAddress(Clb::file&);