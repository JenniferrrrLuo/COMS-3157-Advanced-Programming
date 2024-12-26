#include "gcd.h"

int gcd(int x, int y)
{
    while (y!=0)
    {
        int remainder = x % y; 
        x = y; 
        y = remainder; 
    }
    return x; 
}
