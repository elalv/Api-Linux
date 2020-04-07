/* sv_lib_v2.c

*/

#include <stdio.h>

__asm__(".symver xyz_new,xyz@@VER_2");
__asm__(".symver xyz_old,xyz@VER_1");

void xyz_old(void) { printf("v1 xyz\n"); }

void xyz_new(void) { printf("v2 xyz\n"); }

void pqr(void) { printf("v2 pqr\n"); }
