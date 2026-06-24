// StudyPluginEditorModule.cpp
// ============================================================
// 知识点 0.3 — Editor 模块实现
// ============================================================

#include "StudyPluginEditorModule.h"
#include "StudyPluginExample.h"       // Runtime 模块的类 —— 验证 Editor 能调用 Runtime

// 编辑器 UI 相关头文件
#include "ToolMenus.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"
#include "EditorStyleSet.h"
#include "Styling/AppStyle.h"        // FAppStyle —— UE5.7 推荐替代 FEditorStyle

DEFINE_LOG_CATEGORY_STATIC(LogStudyPluginEditor, Log, All);

#define LOCTEXT_NAMESPACE "FStudyPluginEditorModule"

void FStudyPluginEditorModule::StartupModule()
{
	UE_LOG(LogStudyPluginEditor, Warning, TEXT("===== StudyPluginEditor 已加载 ====="));

	RegisterToolbarButton();
}

void FStudyPluginEditorModule::ShutdownModule()
{
	UE_LOG(LogStudyPluginEditor, Warning, TEXT("===== StudyPluginEditor 已卸载 ====="));

	// 注销工具栏按钮
	if (bHasRegistered && UToolMenus::IsToolMenuUIEnabled())
	{
		UToolMenus::UnregisterOwner(this);
	}
}

// ━━━━━━━━━━━━━━━━ 工具栏按钮注册 ━━━━━━━━━━━━━━━━

void FStudyPluginEditorModule::RegisterToolbarButton()
{
	if (!UToolMenus::IsToolMenuUIEnabled())
	{
		UE_LOG(LogStudyPluginEditor, Warning, TEXT("ToolMenu UI 未启用，跳过工具栏注册"));
		return;
	}

	// 在主工具栏（LevelEditor.LevelEditorToolbar.Play）中注册一个扩展
	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu(
		"LevelEditor.LevelEditorToolbar.Play"
	);

	if (ToolbarMenu)
	{
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("StudyPluginSection");

		// 添加一个按钮
		Section.AddEntry(FToolMenuEntry::InitToolBarButton(
			"StudyPluginToolbarButton",
			FExecuteAction::CreateRaw(this, &FStudyPluginEditorModule::OnToolbarButtonClicked),
			LOCTEXT("ToolbarButtonLabel", "Study Plugin"),
			LOCTEXT("ToolbarButtonTooltip", "点击调用 Runtime 模块方法"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Settings")
		));

		bHasRegistered = true;
		UE_LOG(LogStudyPluginEditor, Log, TEXT("工具栏按钮已注册"));
	}
}

// ━━━━━━━━━━━━━━━━ 按钮点击回调 ━━━━━━━━━━━━━━━━

void FStudyPluginEditorModule::OnToolbarButtonClicked()
{
	UE_LOG(LogStudyPluginEditor, Warning, TEXT("===== 工具栏按钮被点击 ====="));

	// 调用 Runtime 模块的方法 —— 验证 Editor 模块能调用 Runtime 模块
	const FString Greeting = FStudyPluginExample::GetGreeting();
	const int32 Result = FStudyPluginExample::Add(10, 20);

	UE_LOG(LogStudyPluginEditor, Log, TEXT("调用 Runtime 模块结果："));
	UE_LOG(LogStudyPluginEditor, Log, TEXT("  GetGreeting() = %s"), *Greeting);
	UE_LOG(LogStudyPluginEditor, Log, TEXT("  Add(10, 20) = %d"), Result);
	UE_LOG(LogStudyPluginEditor, Warning, TEXT("===== 验证完成：Editor 模块成功调用了 Runtime 模块 ====="));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStudyPluginEditorModule, StudyPluginEditor)
