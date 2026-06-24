// StudyPluginModule.h
// ============================================================
// 知识点 0.2 — 插件模块接口
// 每个 UE 模块都有一个实现 IModuleInterface 的类，
// 负责模块的启动（StartupModule）和关闭（ShutdownModule）。
// 这是模块的「生命周期入口」，可用于注册菜单、加载资源、初始化子系统等。
// ============================================================

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"   // IModuleInterface, FModuleManager

class FStudyPluginModule : public IModuleInterface
{
public:
	// ━━━━━━━━━━━━━━━━ 模块生命周期 ━━━━━━━━━━━━━━━━

	// 模块加载时调用（引擎启动或插件启用时）
	// 适合做：注册编辑器菜单、绑定委托、初始化全局数据
	virtual void StartupModule() override;

	// 模块卸载时调用（引擎关闭或插件禁用时）
	// 适合做：注销之前注册的内容，清理资源
	virtual void ShutdownModule() override;

	// ━━━━━━━━━━━━━━━━ 便捷访问 ━━━━━━━━━━━━━━━━

	// 获取本模块单例 —— 其他代码可通过这个静态方法拿到模块实例
	static inline FStudyPluginModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FStudyPluginModule>("StudyPlugin");
	}

	// 检查模块是否已加载
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("StudyPlugin");
	}
};
