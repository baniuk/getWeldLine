// CalcFeatures_TEST.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	int ret = 0;
	HANDLE hLogFile;
	time_t seconds;

	time ( &seconds );
	hLogFile = CreateFile("C_WeldLineDetect.mylog", FILE_APPEND_DATA, 
		FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG|_CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, hLogFile);
	_RPT1(_CRT_WARN,  "The current local time is: %s\n", ctime(&seconds) );

	::testing::InitGoogleTest(&argc, argv);
	ret = RUN_ALL_TESTS();
	return ret;
}

