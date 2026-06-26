// StudyPluginEditorModule.h
// ============================================================
// 知识点 0.3 (扩展) — Editor 模块接口
// 演示在编辑器多个位置注册菜单/按钮：
//   1. 主工具栏（Play 工具栏）
//   2. 顶部菜单栏（Window 菜单下）
//   3. 视口右键菜单（Actor 上下文菜单）
// ============================================================

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FStudyPluginEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	// ━━━━━━━━━━━━━━━━ 菜单注册方法 ━━━━━━━━━━━━━━━━

	// 1. 主工具栏按钮
	void RegisterToolbarButton();

	// 2. 顶部菜单栏项
	void RegisterMenuBarEntry();

	// 3. 视口右键菜单项
	void RegisterContextMenuEntry();

	// ━━━━━━━━━━━━━━━━ 回调方法 ━━━━━━━━━━━━━━━━

	void OnToolbarButtonClicked();
	void OnMenuBarClicked();
	void OnContextMenuClicked();
};
