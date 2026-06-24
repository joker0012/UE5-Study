// simple_math.h
// ============================================================
// 知识点 0.4 — 第三方 C 库头文件
// 一个极简的数学运算库，纯 C 接口。
// ============================================================

#ifndef SIMPLE_MATH_H
#define SIMPLE_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

// 基本运算
int sm_add(int a, int b);
int sm_subtract(int a, int b);
int sm_multiply(int a, int b);
float sm_divide(int a, int b);

// 阶乘（递归）
int sm_factorial(int n);

// 最大公约数
int sm_gcd(int a, int b);

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_MATH_H
