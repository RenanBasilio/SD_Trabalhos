#include <math.h>

// Método que verifica se um número é primo. Retorna true se o número for primo, ou false caso contrário.
bool isPrime(int num)
{
    if(num <= 1) return false;
    //else if(num == 2) return true;
    else{
        for (int i = 2; i < num; i++)
        {
            if (num%i == 0)
                return false;
        }
        return true;
    }
}