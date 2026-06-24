// BuildConfigTestActor.h
// ============================================================
// 知识点 0.1 — 测试 Actor
// 这个 Actor 验证 Build.cs 中配置的模块依赖是否生效。
// 它使用了 CoreUObject（反射宏）、Engine（AActor）、
// GameplayTags（FGameplayTag）三个模块的 API。
// 如果 Build.cs 没有正确添加依赖，编译会报 "unresolved external"。
// ============================================================

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"   // 需要 GameplayTags 模块依赖
#include "BuildConfigTestActor.generated.h"

UCLASS(BlueprintType, Category = "StudyProject|BuildConfig")
class STUDYPROJECT_API ABuildConfigTestActor : public AActor
{
	GENERATED_BODY()

public:
	ABuildConfigTestActor();

	// ━━━━━━━━━━━━━━━━ 验证用属性 ━━━━━━━━━━━━━━━━

	// 这个宏需要 CoreUObject 模块
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildConfig")
	int32 TestValue = 42;

	// FGameplayTag 需要 GameplayTags 模块
	// → 如果 Build.cs 没加 "GameplayTags" 依赖，这里编译会失败
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildConfig")
	FGameplayTag TestTag;

protected:
	virtual void BeginPlay() override;

	// ━━━━━━━━━━━━━━━━ 验证用函数 ━━━━━━━━━━━━━━━━

	// BlueprintCallable 需要 Engine 模块
	UFUNCTION(BlueprintCallable, Category = "BuildConfig")
	void PrintModuleDependencyInfo();

	// BlueprintPure 测试 —— 无副作用的纯查询函数
	UFUNCTION(BlueprintPure, Category = "BuildConfig")
	int32 GetTestValueSquared() const;
};
