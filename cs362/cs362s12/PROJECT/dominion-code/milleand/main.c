#include <stdio.h>
#include <stdlib.h>
#include "suite.h"

int main(int argc, char** argv)
{
	RunAllTests();
#ifdef WIN32 && _DEBUG
	// Pause after execution if running in debug mode on Windows
	system("pause");
#endif
}
