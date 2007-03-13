#include <stdio.h>
#include <math.h>

double x, y, result, A;


int main(void)
{
	x = -1;
	y = -1;
	
//	result = y/x;
	
	A = atan2(y,x);
	
	printf("\n%f\n", A);
	
//	return 0;
}
