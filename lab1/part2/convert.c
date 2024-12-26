#include <stdio.h>
#include <math.h>

int main()
{
    int x; 
    scanf("%d", &x);

    printf("signed dec:   %d\n", x); 
    printf("unsigned dec: %u\n", x); 
    printf("hex:          %x\n", x); 
    printf("binary:       "); 

    int y = (unsigned int) x; 
    for (int i = 31; i >= 0; i--){
        int num = pow(2, i); 
        if (y - num >= 0){
            printf("1"); 
            y = y - num;
        } else {
            printf("0"); 
        }
        if (i % 4 == 0){
            printf(" "); 
        }
    }
    printf("\n"); 

    return 0; 
}
