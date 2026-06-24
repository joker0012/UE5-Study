// StudyPluginExample.cpp
// ============================================================
// 知识点 0.2 — 插件示例类实现
// ============================================================

#include "StudyPluginExample.h"

FString FStudyPluginExample::GetGreeting()
{
	return TEXT("你好，这是来自 StudyPlugin 插件的问候！插件代码成功被调用。");
}

int32 FStudyPluginExample::Add(int32 A, int32 B)
{
	return A + B;
}
