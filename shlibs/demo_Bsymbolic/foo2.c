/* foo2.c

*/
#include <stdlib.h>
#include <stdio.h>

void xyz(void)
{
    printf("        func2-xyz\n");
}

void abc(void)
{
    printf("        func1-abc\n");
}

void func2(int x)
{
    printf("Called func2\n");
    xyz();
    abc();
}
