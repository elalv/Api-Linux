/* foo1.c

*/
#include <stdlib.h>
#include <stdio.h>

void xyz(void)
{
    printf("        func1-xyz\n");
}

void abc(void)
{
    printf("        func1-abc\n");
}

void func1(int x)
{
    printf("Called func1\n");
    xyz();
    abc();
}
