// simple_math.c
// ============================================================
// 知识点 0.4 — 第三方 C 库实现
// ============================================================

#include "simple_math.h"

int sm_add(int a, int b)
{
    return a + b;
}

int sm_subtract(int a, int b)
{
    return a - b;
}

int sm_multiply(int a, int b)
{
    return a * b;
}

float sm_divide(int a, int b)
{
    if (b == 0) return 0.0f;
    return (float)a / (float)b;
}

int sm_factorial(int n)
{
    if (n <= 1) return 1;
    return n * sm_factorial(n - 1);
}

int sm_gcd(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
