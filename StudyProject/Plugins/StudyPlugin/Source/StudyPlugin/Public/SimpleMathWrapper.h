// SimpleMathWrapper.h
// ============================================================
// 知识点 0.4 — 第三方库包装类
// UE 代码不能直接调 C 库函数（命名空间、编译差异等），
// 标准做法是写一个 Wrapper 类封装第三方 API。
// ============================================================

#pragma once

#include "CoreMinimal.h"

class STUDYPLUGIN_API FSimpleMathWrapper
{
public:
	// 加法
	static int32 Add(int32 A, int32 B);

	// 减法
	static int32 Subtract(int32 A, int32 B);

	// 乘法
	static int32 Multiply(int32 A, int32 B);

	// 除法（返回 float）
	static float Divide(int32 A, int32 B);

	// 阶乘
	static int32 Factorial(int32 N);

	// 最大公约数
	static int32 GCD(int32 A, int32 B);

	// 一次性测试所有方法，返回格式化的结果字符串
	static FString RunAllTests();
};
