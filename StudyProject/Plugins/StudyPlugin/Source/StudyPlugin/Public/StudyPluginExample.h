// StudyPluginExample.h
// ============================================================
// 知识点 0.2 — 插件内的示例类
// 演示：插件可以被主项目调用。
// 这个类提供一个静态方法，主项目的代码可以 #include 后直接调用，
// 验证「插件代码能被主项目使用」。
// ============================================================

#pragma once

#include "CoreMinimal.h"

// STUDYPLUGIN_API 宏 —— 让这个类的符号导出，主项目才能链接使用
// 这个宏由 UBT 自动生成（基于模块名 StudyPlugin）
class STUDYPLUGIN_API FStudyPluginExample
{
public:
	// 返回一句问候语 —— 验证插件方法可被外部调用
	static FString GetGreeting();

	// 一个简单的计算方法 —— 演示插件提供工具函数
	static int32 Add(int32 A, int32 B);
};
