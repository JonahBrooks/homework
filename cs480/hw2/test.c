#include <stdio.h>
#include <stdbool.h>
int main()
{
bool False = false;
bool True = true;

False = True;
True = False;
if (True)
{
	printf("%s\n", ((False)?"True":"False"));
}
if (False)
{
	printf("%s\n", ((False)?"True":"False"));
}
return 0;
}
