#include <stdio.h>
int main()
{
int y = 0;
int x = 0;
int s = 0;
int j = 0;
int i = 0;

x = 3;
y = 4 + x;
x = 2 + 1;
printf("%d \n", (-x) + y);
i = 5;
int i_ = 0;
for(i_ = 0; i_ < x + y; i_++)
{
	i = i_;
	printf("%d %d \n", i + x, i);
}
s = 0;
int j_ = 0;
for(j_ = 0; j_ < 10; j_++)
{
	j = j_;
	s = s + j;
}
printf("%d %d %d \n", s, j, y);

return 0;
}
