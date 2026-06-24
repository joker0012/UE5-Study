// StudyPlugin.Build.cs
// ============================================================
// 知识点 0.3/0.4 — Runtime 模块 + 第三方库集成
// Runtime 模块在编辑器和打包版本都会编译。
// 放运行时逻辑：Actor、Component、游戏玩法等。
// 不依赖任何编辑器专属 API（如 UnrealEd、Slate 编辑器等）。
// ============================================================

using UnrealBuildTool;
using System.IO;   // Path.Combine 需要

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

		// ━━━━━━━━━━━━━━━━ 0.4 第三方库集成 ━━━━━━━━━━━━━━━━
		// 集成 SimpleMathLib —— 一个自编译的 C 静态库
		// 三步走：1.加头文件路径  2.加库文件路径  3.声明要链接的 .lib
		SetupSimpleMathLib();
	}

	// ━━━━━━━━━━━━━━━━ 第三方库集成方法 ━━━━━━━━━━━━━━━━
	// 这是 UE5 集成第三方库的标准写法：
	// 1. PublicIncludePaths / PrivateIncludePaths → 头文件搜索路径
	// 2. RuntimeDependencies / PublicAdditionalLibraries → 链接库文件
	private void SetupSimpleMathLib()
	{
		// 第三方库根目录（相对于本 Build.cs 所在位置）
		string LibDir = Path.Combine(ModuleDirectory, "..", "ThirdParty", "SimpleMathLib");

		// 1. 头文件路径 —— 让 #include "simple_math.h" 能找到
		PublicIncludePaths.Add(Path.Combine(LibDir, "include"));

		// 2. 库文件路径 —— 根据平台选择对应的 .lib
		string LibPath = Path.Combine(LibDir, "lib", "SimpleMathLib.lib");

		// 3. 添加到链接列表
		if (File.Exists(LibPath))
		{
			PublicAdditionalLibraries.Add(LibPath);
		}
		else
		{
			System.Console.WriteLine("[StudyPlugin] 警告: SimpleMathLib.lib 不存在，请先编译 ThirdParty 库");
		}
	}
}
