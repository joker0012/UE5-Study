// SimpleMathWrapper.cpp
// ============================================================
// 知识点 0.4 — 第三方库包装类实现
// 这里 #include 第三方 C 库头文件，调用其函数。
// ============================================================

#include "SimpleMathWrapper.h"
#include "simple_math.h"   // 第三方 C 库头文件（Build.cs 中配置了 include 路径）

DEFINE_LOG_CATEGORY_STATIC(LogSimpleMath, Log, All);

int32 FSimpleMathWrapper::Add(int32 A, int32 B)
{
	return sm_add(A, B);
}

int32 FSimpleMathWrapper::Subtract(int32 A, int32 B)
{
	return sm_subtract(A, B);
}

int32 FSimpleMathWrapper::Multiply(int32 A, int32 B)
{
	return sm_multiply(A, B);
}

float FSimpleMathWrapper::Divide(int32 A, int32 B)
{
	return sm_divide(A, B);
}

int32 FSimpleMathWrapper::Factorial(int32 N)
{
	return sm_factorial(N);
}

int32 FSimpleMathWrapper::GCD(int32 A, int32 B)
{
	return sm_gcd(A, B);
}

FString FSimpleMathWrapper::RunAllTests()
{
	// 调用所有第三方库函数，验证集成成功
	int32 r_add = sm_add(100, 200);
	int32 r_sub = sm_subtract(50, 18);
	int32 r_mul = sm_multiply(7, 8);
	float r_div = sm_divide(100, 4);
	int32 r_fac = sm_factorial(5);
	int32 r_gcd = sm_gcd(48, 36);

	FString Result = FString::Printf(
		TEXT("SimpleMathLib 测试结果:\n")
		TEXT("  add(100, 200) = %d\n")
		TEXT("  subtract(50, 18) = %d\n")
		TEXT("  multiply(7, 8) = %d\n")
		TEXT("  divide(100, 4) = %.1f\n")
		TEXT("  factorial(5) = %d\n")
		TEXT("  gcd(48, 36) = %d\n")
		TEXT("  => 第三方 C 库集成成功！"),
		r_add, r_sub, r_mul, r_div, r_fac, r_gcd
	);

	UE_LOG(LogSimpleMath, Warning, TEXT("%s"), *Result);
	return Result;
}
