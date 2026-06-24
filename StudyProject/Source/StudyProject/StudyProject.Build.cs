// StudyProject.Build.cs
// ============================================================
// 知识点 0.1 — Build.cs 详解
// Build.cs 定义「模块规则」：这个模块依赖谁、暴露什么 API、
// 用什么 PCH 策略、是否只在编辑器编译等。
//
// 核心：PublicDependency（公开依赖） vs PrivateDependency（私有依赖）
//   Public  → 头文件里 #include 了依赖模块的头文件 → 模块使用者也会自动继承这个依赖
//   Private → 只在 .cpp 里用 → 模块使用者看不到这个依赖
// ============================================================

using UnrealBuildTool;

// 类名 = 模块名，必须与目录名一致
public class StudyProject : ModuleRules
{
	public StudyProject(ReadOnlyTargetRules Target) : base(Target)
	{
		// ━━━━━━━━━━━━━━━━ PCH 策略 ━━━━━━━━━━━━━━━━

		// UseExplicitOrSharedPCHs —— 最常用，推荐
		// 模块自己的头文件用 explicit PCH，引擎模块用 shared PCH
		// 另一个选项 UseSharedPCHs 会强制所有模块共享同一个 PCH
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// ━━━━━━━━━━━━━━━━ 公开依赖 ━━━━━━━━━━━━━━━━

		// 公开依赖 = 本模块的 .h 文件中 #include 了这些模块的头文件
		// 其他依赖本模块的模块会自动继承这些依赖（传递性）
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",           // 基础容器、智能指针、日志等（FString, TArray, TSharedPtr...）
			"CoreUObject",    // 反射系统（UCLASS, UFUNCTION, UPROPERTY...）
			"Engine",         // Actor, Component, World 等核心游戏类
			"InputCore",      // 输入相关的基础类型
			"EnhancedInput",  // Enhanced Input 系统（3.x 模块会深入学）
			"GameplayTags",   // FGameplayTag —— 测试 Actor 会用到
		});

		// ━━━━━━━━━━━━━━━━ 私有依赖 ━━━━━━━━━━━━━━━━

		// 私有依赖 = 只在 .cpp 文件中 #include，不影响头文件
		// 其他模块依赖本模块时不会继承这些依赖
		PrivateDependencyModuleNames.AddRange(new string[] {
			// 本示例暂无私有依赖，后续知识点会逐步添加
			// 常见的有：
			// "Slate", "SlateCore",  // 用到 Slate UI 时
			// "RenderCore", "RHI",   // 用到渲染相关时
			// "NavigationSystem",    // 用到导航时
			"StudyPlugin",            // 0.2 — 主项目依赖插件模块，调用插件提供的类
		});

		// ━━━━━━━━━━━━━━━━ 演示：条件依赖 ━━━━━━━━━━━━━━━━

		// 可以根据编译目标（Editor / Game / Shipping）动态添加依赖
		// 这是实际项目中的常见做法
		if (Target.bBuildEditor == true)
		{
			// 编辑器专属模块 —— 打包时不会编译
			// PrivateDependencyModuleNames.Add("UnrealEd");
			// PrivateDependencyModuleNames.Add("EditorScriptingUtilities");
		}

		// ━━━━━━━━━━━━━━━━ 演示：宏定义 ━━━━━━━━━━━━━━━━

		// PublicDefinitions 对所有依赖本模块的模块可见
		// PrivateDefinitions 只对本模块可见
		// 用法：#if WITH_STUDY_FEATURE ... #endif
		PublicDefinitions.Add("STUDY_PROJECT_VERSION=1");

		// ━━━━━━━━━━━━━━━━ 可选配置（了解即可）━━━━━━━━━━━━━━━━

		// bUsePrecompiled —— 是否使用预编译库（仅打包发布插件时用）
		// bUsePrecompiled = false;

		// DynamicallyLoadedModuleNames —— 运行时动态加载的模块
		// 很少用，一般只在需要运行时按需加载时才用
		// DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
	}
}
