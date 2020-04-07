/* foo3.c

*/
#include <stdlib.h>
#include <stdio.h>

void abc(void);

void xyz(void)
{
    printf("        func3-xyz\n");
}

void func3(int x)
{
    printf("Called func3\n");
    xyz();
    abc();
}
