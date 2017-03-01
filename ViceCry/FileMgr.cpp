#include "StdAfx.h"
#include "FileMgr.h"

const char* CFileLoader::LoadLine(FILE* hFile)
{
	static char		cLineBuffer[512];

	if ( !CFileMgr::ReadLine(hFile, cLineBuffer, sizeof(cLineBuffer)) )
		return nullptr;

	for ( int i = 0; cLineBuffer[i]; ++i )
	{
		if ( cLineBuffer[i] == '\n' )
			cLineBuffer[i] = '\0';
		else if ( cLineBuffer[i] < ' ' || cLineBuffer[i] == ',' )
			cLineBuffer[i] = ' ';
	}

	const char* p = cLineBuffer;
	while ( *p <= ' ' )
	{
		if ( !*p++ )
			break;
	}
	return p;
}
