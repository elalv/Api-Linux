/* prog.c

*/
#include <stdlib.h>
#include <stdio.h>

void xyz(void)
{
    printf("        main-xyz\n");
}

int main(int argc, char *argv[])
{
    void func1(void), func2(void), func3(void);

    func1();
    func2();

    exit(EXIT_SUCCESS);
}
