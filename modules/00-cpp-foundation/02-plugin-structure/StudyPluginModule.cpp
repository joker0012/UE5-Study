// StudyPluginModule.cpp
// ============================================================
// 知识点 0.2 — 插件模块实现
// ============================================================

#include "StudyPluginModule.h"
#include "StudyPluginExample.h"

// 自定义日志分类 —— 方便在 Output Log 中过滤插件自己的日志
DEFINE_LOG_CATEGORY_STATIC(LogStudyPlugin, Log, All);

// ━━━━━━━━━━━━━━━━ 模块生命周期实现 ━━━━━━━━━━━━━━━━

void FStudyPluginModule::StartupModule()
{
	// 模块加载时触发 —— 你启动编辑器时就能在 Output Log 看到这条
	UE_LOG(LogStudyPlugin, Warning, TEXT("===== StudyPlugin 已加载（StartupModule）====="));

	// 演示：调用插件内的一个示例类
	const FString Greeting = FStudyPluginExample::GetGreeting();
	UE_LOG(LogStudyPlugin, Log, TEXT("%s"), *Greeting);
}

void FStudyPluginModule::ShutdownModule()
{
	// 模块卸载时触发 —— 关闭编辑器时输出
	UE_LOG(LogStudyPlugin, Warning, TEXT("===== StudyPlugin 已卸载（ShutdownModule）====="));
}

// ━━━━━━━━━━━━━━━━ 模块注册宏 ━━━━━━━━━━━━━━━━

// 这个宏把 FStudyPluginModule 注册为名为 "StudyPlugin" 的模块
// 第一个参数是模块实现类，第二个参数是模块名（必须与 .Build.cs / .uplugin 一致）
// 没有这个宏，模块无法被引擎识别和加载
IMPLEMENT_MODULE(FStudyPluginModule, StudyPlugin)
