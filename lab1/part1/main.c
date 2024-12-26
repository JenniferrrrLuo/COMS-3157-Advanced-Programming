#include <stdio.h>
#include "gcd.h"
#include "prime.h"

int main()
{
    int x, y; 
    scanf("%d", &x); 
    scanf("%d", &y); 
    printf("You typed in %d and %d\n", x, y); 

    float avg = (x + y)/2.0; 
    printf("The average is: %f\n", avg); 

    if (prime(x)){
        printf("%d is prime\n", x); 
    } else {
        printf("%d is not prime\n", x); 
    }

    if (prime(y)){   
        printf("%d is prime\n", y); 
    } else {   
        printf("%d is not prime\n", y); 
    }   

    int gcd_number = gcd(x, y); 
    if (gcd_number == 1){
        printf("%d and %d are relatively prime\n", x, y); 
    } else {
        printf("%d and %d are not relatively prime\n", x, y);
    }

    return 0; 
} 
