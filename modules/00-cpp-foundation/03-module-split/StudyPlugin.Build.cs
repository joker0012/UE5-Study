// StudyPlugin.Build.cs
// ============================================================
// 知识点 0.3 — Runtime 模块
// Runtime 模块在编辑器和打包版本都会编译。
// 放运行时逻辑：Actor、Component、游戏玩法等。
// 不依赖任何编辑器专属 API（如 UnrealEd、Slate 编辑器等）。
// ============================================================

using UnrealBuildTool;

public class StudyPlugin : ModuleRules
{
	public StudyPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		// 公开依赖 —— 头文件中用到的模块
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
		});

		// 私有依赖 —— 只在 .cpp 中用到的模块
		PrivateDependencyModuleNames.AddRange(new string[] {
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
		});

		// ⚠️ Runtime 模块不能依赖 Editor 模块（StudyPluginEditor）
		// 否则打包时会编译失败 —— 打包时不编译 Editor 模块
	}
}
