// LiveCodingDemoActor.h
// ============================================================
// 知识点 0.5 — Live Coding / Hot Reload 演示
// 这个 Actor 用于演示：不关闭编辑器，改 C++ 代码后热重载生效。
// ============================================================

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LiveCodingDemoActor.generated.h"

UCLASS(BlueprintType, Category = "StudyProject|LiveCoding")
class STUDYPROJECT_API ALiveCodingDemoActor : public AActor
{
	GENERATED_BODY()

public:
	ALiveCodingDemoActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	// ━━━━━━━━━━━━━━━━ 可调参数（改完用 Live Coding 热重载）━━━━━━━━━━━━━━━━

	// 旋转速度 —— 试着改成不同值，Live Coding 后立即生效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LiveCoding", meta = (ClampMin = "0", ClampMax = "500"))
	float RotationSpeed = 90.0f;

	// 消息文本 —— 试着改这行文字，Live Coding 后 BeginPlay 重新输出
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LiveCoding")
	FString Message = TEXT("Live Coding 默认消息 —— 改我试试！");

	// 颜色 —— 试着改 RGB 值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LiveCoding")
	FColor DisplayColor = FColor::Cyan;

	// ━━━━━━━━━━━━━━━━ 验证函数 ━━━━━━━━━━━━━━━━

	// 调用后打印当前状态 —— 验证 Live Coding 后函数体变更是否生效
	UFUNCTION(BlueprintCallable, Category = "LiveCoding")
	void PrintCurrentState();

	// 返回一个计算结果 —— 改算法后用 Live Coding 验证
	UFUNCTION(BlueprintCallable, Category = "LiveCoding")
	int32 CalculateSomething(int32 Input) const;

private:
	// 计数器 —— 演示 Tick 中持续运行的状态不会被 Live Coding 重置
	int32 TickCounter = 0;
};
