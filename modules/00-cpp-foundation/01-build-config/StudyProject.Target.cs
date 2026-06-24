// StudyProject.Target.cs
// ============================================================
// 知识点 0.1 — Target.cs 详解
// Target.cs 定义「编译目标」：编译出什么可执行文件、用什么配置。
// 一个 UE 项目通常有两个 Target：Game（打包用）和 Editor（开发用）。
// ============================================================

using UnrealBuildTool;
using System.Collections.Generic;

// 类名必须与文件名一致（去掉 .Target.cs 后缀）
// 继承 TargetRules —— UE5.7 的基类
public class StudyProjectTarget : TargetRules
{
	// 构造函数接收 TargetInfo，传给基类
	public StudyProjectTarget(TargetInfo Target) : base(Target)
	{
		// ━━━━━━━━━━━━━━━━ 核心字段 ━━━━━━━━━━━━━━━━

		// TargetType 决定编译产物：
		//   Game   → 打包给玩家的独立可执行文件（不含编辑器）
		//   Editor → 带编辑器的开发版（你在 Rider/VS 里 F5 启动的就是这个）
		//   Client → 仅客户端（不需要服务端代码的版本）
		//   Server → 仅服务端（专用服务器）
		Type = TargetType.Game;

		// DefaultBuildSettings 控制引擎头文件包含顺序的版本
		// V6 是 UE5.3+ 的最新版本，确保与现代引擎 API 兼容
		DefaultBuildSettings = BuildSettingsVersion.V6;

		// IncludeOrderVersion 指定头文件包含顺序的引擎版本
		// Unreal5_7 对应 UE 5.7，不同版本的宏定义和头文件顺序可能不同
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;

		// ━━━━━━━━━━━━━━━━ 模块注册 ━━━━━━━━━━━━━━━━

		// ExtraModuleNames 列出这个 Target 要编译哪些游戏模块
		// 每个 Build.cs 定义的 ModuleRules 类名对应一个模块名
		// Editor Target 通常和 Game Target 注册相同的模块
		ExtraModuleNames.Add("StudyProject");

		// ━━━━━━━━━━━━━━━━ 可选配置（了解即可）━━━━━━━━━━━━━━━━

		// 是否编译开发工具（DevelopmentTools）
		// bBuildDeveloperTools = true;  // Editor Target 默认 true

		// 是否启用 CrashReporter
		// bUseChecksInShipping = false;  // Shipping 版本是否保留 check/ensure

		// 全局宏定义（所有模块可见）
		// GlobalDefinitions.Add("MY_FEATURE=1");
	}
}
