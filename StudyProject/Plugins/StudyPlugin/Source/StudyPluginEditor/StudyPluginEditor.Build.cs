// StudyPluginEditor.Build.cs
// ============================================================
// 知识点 0.3 — Editor 模块
// Editor 模块仅在编辑器下编译，打包时自动排除。
// 放编辑器专属逻辑：编辑器工具栏按钮、Detail 面板自定义、
// Asset Action、Editor Utility 等。
//
// 关键依赖：
//   - 依赖 StudyPlugin（Runtime 模块）→ Editor 可以调用 Runtime
//   - 依赖 UnrealEd / SlateCore / Slate → 编辑器 UI 专属
//   - 依赖 EditorStyle / ToolMenus → 工具栏/菜单注册
// ============================================================

using UnrealBuildTool;

public class StudyPluginEditor : ModuleRules
{
	public StudyPluginEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		// 公开依赖
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"StudyPlugin",          // ← Editor 模块依赖 Runtime 模块（单向）
		});

		// 私有依赖 —— 编辑器专属模块
		PrivateDependencyModuleNames.AddRange(new string[] {
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"UnrealEd",             // 编辑器核心（GEditor 等）
			"EditorStyle",          // 编辑器样式（FEditorStyle，已废弃但部分 API 仍需）
			"ToolMenus",            // 菜单/工具栏注册
			"LevelEditor",          // 关卡编辑器（ActorContextMenu 等路径需要）
		});
	}
}
