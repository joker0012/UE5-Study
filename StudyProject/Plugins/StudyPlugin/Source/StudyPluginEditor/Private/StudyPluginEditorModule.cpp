// StudyPluginEditorModule.cpp
// ============================================================
// 知识点 0.3 (扩展) — Editor 模块实现
// 演示 ToolMenus 在编辑器三个位置注册 UI 入口。
// ============================================================

#include "StudyPluginEditorModule.h"
#include "StudyPluginExample.h"       // Runtime 模块的类

// ━━━━━━━━━━━━━━━━ 头文件 ━━━━━━━━━━━━━━━━

#include "ToolMenus.h"
#include "ToolMenuEntry.h"
#include "ToolMenuSection.h"
#include "Styling/AppStyle.h"        // FAppStyle —— UE5 统一样式系统
#include "Engine/Selection.h"        // GEditor->GetSelectedActors() 需要

DEFINE_LOG_CATEGORY_STATIC(LogStudyPluginEditor, Log, All);

#define LOCTEXT_NAMESPACE "FStudyPluginEditorModule"

// ━━━━━━━━━━━━━━━━ 生命周期 ━━━━━━━━━━━━━━━━

void FStudyPluginEditorModule::StartupModule()
{
	UE_LOG(LogStudyPluginEditor, Warning, TEXT("===== StudyPluginEditor 已加载 ====="));

	// 在三个不同位置注册菜单/按钮
	RegisterToolbarButton();    // 主工具栏
	RegisterMenuBarEntry();     // 顶部菜单栏
	RegisterContextMenuEntry(); // 视口右键菜单
}

void FStudyPluginEditorModule::ShutdownModule()
{
	UE_LOG(LogStudyPluginEditor, Warning, TEXT("===== StudyPluginEditor 已卸载 ====="));

	// 注销所有注册的菜单项 —— UnregisterOwner 会一次性清除该 Owner 注册的所有条目
	if (UToolMenus::IsToolMenuUIEnabled())
	{
		UToolMenus::UnregisterOwner(this);
	}
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1. 主工具栏按钮
//    路径: "LevelEditor.LevelEditorToolBar.PlayToolBar"
//    位置: 编辑器顶部 Play 按钮所在的工具栏
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void FStudyPluginEditorModule::RegisterToolbarButton()
{
	if (!UToolMenus::IsToolMenuUIEnabled()) return;

	// ExtendMenu = 在已有菜单上追加内容（不覆盖原有项）
	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu(
		"LevelEditor.LevelEditorToolBar.PlayToolBar"
	);

	if (ToolbarMenu)
	{
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("StudyPluginSection");

		// InitToolBarButton 参数：名称、点击动作、标签、tooltip、图标
		Section.AddEntry(FToolMenuEntry::InitToolBarButton(
			"StudyPluginToolbarButton",
			FExecuteAction::CreateRaw(this, &FStudyPluginEditorModule::OnToolbarButtonClicked),
			LOCTEXT("ToolbarButtonLabel", "Study Plugin"),
			LOCTEXT("ToolbarButtonTooltip", "工具栏按钮 —— 点击调用 Runtime 模块方法"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Settings")
		));

		UE_LOG(LogStudyPluginEditor, Log, TEXT("[1] 工具栏按钮已注册 → PlayToolBar"));
	}
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 2. 顶部菜单栏项
//    路径: "LevelEditor.MainMenu.Window"
//    位置: 编辑器顶部菜单栏的 Window 菜单
//    用 InitMenuEntry（不是 InitToolBarButton）—— 菜单项和工具栏按钮用不同的初始化方法
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void FStudyPluginEditorModule::RegisterMenuBarEntry()
{
	if (!UToolMenus::IsToolMenuUIEnabled()) return;

	UToolMenu* WindowMenu = UToolMenus::Get()->ExtendMenu(
		"LevelEditor.MainMenu.Window"
	);

	if (WindowMenu)
	{
		FToolMenuSection& Section = WindowMenu->FindOrAddSection("StudyPluginMenuSection");

		// InitMenuEntry 参数：名称、标签、tooltip、图标、点击动作
		// 注意：菜单项用 InitMenuEntry，工具栏按钮用 InitToolBarButton
		// 注意：参数顺序是 Name, Label, ToolTip, Icon, Action（Icon 在 Action 前面！）
		Section.AddEntry(FToolMenuEntry::InitMenuEntry(
			"StudyPluginMenuBarEntry",
			LOCTEXT("MenuBarLabel", "Study Plugin 测试"),
			LOCTEXT("MenuBarTooltip", "菜单栏项 —— 在 Window 菜单下"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Search"),
			FExecuteAction::CreateRaw(this, &FStudyPluginEditorModule::OnMenuBarClicked)
		));

		UE_LOG(LogStudyPluginEditor, Log, TEXT("[2] 菜单栏项已注册 → Window 菜单"));
	}
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 3. 视口右键菜单项
//    路径: "LevelEditor.ActorContextMenu"
//    位置: 视口中选中 Actor 后右键弹出的上下文菜单
//    这是编辑器扩展最常用的位置 —— 给特定 Actor 添加操作
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void FStudyPluginEditorModule::RegisterContextMenuEntry()
{
	if (!UToolMenus::IsToolMenuUIEnabled()) return;

	UToolMenu* ContextMenu = UToolMenus::Get()->ExtendMenu(
		"LevelEditor.ActorContextMenu"
	);

	if (ContextMenu)
	{
		FToolMenuSection& Section = ContextMenu->FindOrAddSection("StudyPluginContextSection");

		Section.AddEntry(FToolMenuEntry::InitMenuEntry(
			"StudyPluginContextEntry",
			LOCTEXT("ContextMenuLabel", "Study Plugin: 打印 Actor 信息"),
			LOCTEXT("ContextMenuTooltip", "右键菜单项 —— 打印选中 Actor 的信息"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Info"),
			FExecuteAction::CreateRaw(this, &FStudyPluginEditorModule::OnContextMenuClicked)
		));

		UE_LOG(LogStudyPluginEditor, Log, TEXT("[3] 右键菜单项已注册 → ActorContextMenu"));
	}
}

// ━━━━━━━━━━━━━━━━ 回调方法 ━━━━━━━━━━━━━━━━

void FStudyPluginEditorModule::OnToolbarButtonClicked()
{
	UE_LOG(LogStudyPluginEditor, Warning, TEXT("===== [工具栏按钮] 被点击 ====="));

	const FString Greeting = FStudyPluginExample::GetGreeting();
	const int32 Result = FStudyPluginExample::Add(10, 20);

	UE_LOG(LogStudyPluginEditor, Log, TEXT("  GetGreeting() = %s"), *Greeting);
	UE_LOG(LogStudyPluginEditor, Log, TEXT("  Add(10, 20) = %d"), Result);
}

void FStudyPluginEditorModule::OnMenuBarClicked()
{
	UE_LOG(LogStudyPluginEditor, Warning, TEXT("===== [菜单栏项] 被点击 ====="));
	UE_LOG(LogStudyPluginEditor, Log, TEXT("  这是从 Window 菜单栏点击的"));
}

void FStudyPluginEditorModule::OnContextMenuClicked()
{
	UE_LOG(LogStudyPluginEditor, Warning, TEXT("===== [右键菜单项] 被点击 ====="));

	// 获取当前选中的 Actor —— 右键菜单最常见的用法
	if (GEditor)
	{
		TArray<AActor*> SelectedActors;
		GEditor->GetSelectedActors()->GetSelectedObjects(SelectedActors);

		if (SelectedActors.Num() > 0)
		{
			AActor* Selected = SelectedActors[0];
			UE_LOG(LogStudyPluginEditor, Log, TEXT("  选中 Actor: %s"), *Selected->GetName());
			UE_LOG(LogStudyPluginEditor, Log, TEXT("  位置: %s"), *Selected->GetActorLocation().ToString());
			UE_LOG(LogStudyPluginEditor, Log, TEXT("  类: %s"), *Selected->GetClass()->GetName());
		}
		else
		{
			UE_LOG(LogStudyPluginEditor, Log, TEXT("  没有选中 Actor"));
		}
	}
}

#undef LOCTEXT_NAMESPACE

// ━━━━━━━━━━━━━━━━ IMPLEMENT_MODULE ━━━━━━━━━━━━━━━━
// 这个宏必须放在 .cpp 文件末尾（全局作用域，不能在命名空间内）
// 作用：向引擎注册模块入口函数，引擎通过这个函数创建模块实例
// 参数1：模块实现类名
// 参数2：模块名字符串（必须与 .uplugin / Build.cs 一致）
IMPLEMENT_MODULE(FStudyPluginEditorModule, StudyPluginEditor)
