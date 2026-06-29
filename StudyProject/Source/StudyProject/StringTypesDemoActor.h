// StringTypesDemoActor.h
// ============================================================
// 知识点 1.3 — FName / FString / FText 区别与转换
// 这个 Actor 演示 UE5 三大字符串类型的本质区别、适用场景和互转方法。
//
// 核心一句话：
//   FName  → 身份标识（"这个 Actor 叫 Player_Start_01"）—— 比较 O(1)
//   FString → 通用字符串（"Hello World"）—— 可变、可拼接、可格式化
//   FText  → 显示文本（"欢迎来到游戏"）—— 支持本地化，只读不可拼
// ============================================================

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StringTypesDemoActor.generated.h"

UCLASS(BlueprintType, Category = "StudyProject|Strings")
class STUDYPROJECT_API AStringTypesDemoActor : public AActor
{
	GENERATED_BODY()

public:
	AStringTypesDemoActor();

protected:
	virtual void BeginPlay() override;

public:
	// ━━━━━━━━━━━━━━━━ UPROPERTY 演示属性 ━━━━━━━━━━━━━━━━

	// FName —— 编辑器中显示为下拉框（历史记录），适合标识符
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strings|FName")
	FName CharacterName = TEXT("Hero");

	// FString —— 编辑器中显示为文本框，适合任意文本输入
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strings|FString")
	FString CustomNickname = TEXT("勇者123");

	// FText —— 编辑器中显示为本地化文本编辑器，适合显示给玩家
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strings|FText")
	FText DisplayName = INVTEXT("无名英雄");

	// ━━━━━━━━━━━━━━━━ UFUNCTION 演示入口 ━━━━━━━━━━━━━━━━

	UFUNCTION(BlueprintCallable, Category = "Strings")
	void RunAllDemos();

private:
	// 1. FName —— 轻量标识符
	void Demo_FName();

	// 2. FString —— 通用字符串
	void Demo_FString();

	// 3. FText —— 本地化文本
	void Demo_FText();

	// 4. 三者互转
	void Demo_Conversions();

	// 5. 性能对比： FName 比较 vs FString 比较
	void Demo_Performance();

	// 6. 常见坑 & 最佳实践
	void Demo_Pitfalls();
};
