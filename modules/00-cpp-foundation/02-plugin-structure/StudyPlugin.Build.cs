// StudyPlugin.Build.cs
// ============================================================
// 知识点 0.2 — 插件模块的 Build.cs
// 插件模块的 Build.cs 与项目模块写法完全一致，
// 唯一区别：它位于 Plugins/插件名/Source/模块名/ 下。
// ============================================================

using UnrealBuildTool;

public class StudyPlugin : ModuleRules
{
	public StudyPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		// 插件公开依赖 —— 头文件中用到的模块
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
		});

		// 插件私有依赖 —— 只在 .cpp 中用到的模块
		PrivateDependencyModuleNames.AddRange(new string[] {
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
		});
	}
}
