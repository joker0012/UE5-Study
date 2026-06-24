// BuildConfigTestActor.cpp
// ============================================================
// 知识点 0.1 — 测试 Actor 实现
// ============================================================

#include "BuildConfigTestActor.h"
#include "StudyPluginExample.h"   // 0.2 — 引用插件的示例类

ABuildConfigTestActor::ABuildConfigTestActor()
{
	// 设为可放置在关卡中的 Actor
	PrimaryActorTick.bCanEverTick = false;
}

void ABuildConfigTestActor::BeginPlay()
{
	Super::BeginPlay();

	PrintModuleDependencyInfo();
}

void ABuildConfigTestActor::PrintModuleDependencyInfo()
{
	// ━━━━━━━━━━━━━━━━ 验证 Core 模块 ━━━━━━━━━━━━━━━━
	// UE_LOG 来自 Core 模块
	UE_LOG(LogTemp, Warning, TEXT("===== BuildConfig 验证 ====="));

	// ━━━━━━━━━━━━━━━━ 验证 CoreUObject 模块 ━━━━━━━━━━━━━━━━
	// GetClass() / GetName() 来自反射系统
	UE_LOG(LogTemp, Log, TEXT("类名: %s"), *GetClass()->GetName());

	// ━━━━━━━━━━━━━━━━ 验证 Engine 模块 ━━━━━━━━━━━━━━━━
	// GetActorLocation() 来自 AActor
	UE_LOG(LogTemp, Log, TEXT("位置: %s"), *GetActorLocation().ToString());

	// ━━━━━━━━━━━━━━━━ 验证 GameplayTags 模块 ━━━━━━━━━━━━━━━━
	// FGameplayTag::RequestGameplayTag 来自 GameplayTags 模块
	// 如果 Build.cs 没加这个依赖，编译时会报链接错误
	if (TestTag.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("TestTag: %s"), *TestTag.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("TestTag: 未设置（在编辑器 Detail 面板设置试试）"));
	}

	// ━━━━━━━━━━━━━━━━ 验证宏定义 ━━━━━━━━━━━━━━━━
	// Build.cs 中 PublicDefinitions.Add("STUDY_PROJECT_VERSION=1")
#ifdef STUDY_PROJECT_VERSION
	UE_LOG(LogTemp, Log, TEXT("STUDY_PROJECT_VERSION = %d"), STUDY_PROJECT_VERSION);
#else
	UE_LOG(LogTemp, Log, TEXT("STUDY_PROJECT_VERSION 未定义"));
#endif

	UE_LOG(LogTemp, Warning, TEXT("===== 验证完成，如果上面都有输出说明依赖配置正确 ====="));

	// ━━━━━━━━━━━━━━━━ 0.2 — 验证插件调用 ━━━━━━━━━━━━━━━━
	// 调用 StudyPlugin 插件提供的方法，验证「主项目能调用插件代码」
	UE_LOG(LogTemp, Warning, TEXT("===== 插件调用验证 ====="));
	UE_LOG(LogTemp, Log, TEXT("插件问候: %s"), *FStudyPluginExample::GetGreeting());
	UE_LOG(LogTemp, Log, TEXT("插件计算 3 + 5 = %d"), FStudyPluginExample::Add(3, 5));
}

int32 ABuildConfigTestActor::GetTestValueSquared() const
{
	return TestValue * TestValue;
}
