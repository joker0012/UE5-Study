// StudyPluginEditorModule.h
// ============================================================
// 知识点 0.3 — Editor 模块接口
// Editor 模块在 StartupModule 中注册工具栏按钮，
// 点击按钮后调用 Runtime 模块的方法 —— 验证 Editor → Runtime 单向依赖。
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
	// 注册工具栏按钮
	void RegisterToolbarButton();

	// 工具栏按钮点击回调
	void OnToolbarButtonClicked();

	// 标记是否已注册，用于 ShutdownModule 时判断是否需要注销
	bool bHasRegistered = false;
};
