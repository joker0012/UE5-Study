// StudyProjectEditor.Target.cs
// ============================================================
// Editor Target —— 你在开发时实际运行的版本（带编辑器）
// 与 Game Target 的唯一区别：Type = TargetType.Editor
// ============================================================

using UnrealBuildTool;
using System.Collections.Generic;

public class StudyProjectEditorTarget : TargetRules
{
	public StudyProjectEditorTarget(TargetInfo Target) : base(Target)
	{
		// Editor 类型 —— 编译出带 UE 编辑器的可执行文件
		// 这是你日常开发 F5 / Rider 启动的版本
		Type = TargetType.Editor;

		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;

		// 和 Game Target 注册相同的游戏模块
		ExtraModuleNames.Add("StudyProject");
	}
}
