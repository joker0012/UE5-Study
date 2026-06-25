// LiveCodingDemoActor.cpp
// ============================================================
// 知识点 0.5 — Live Coding / Hot Reload 演示
// ============================================================

#include "LiveCodingDemoActor.h"

DEFINE_LOG_CATEGORY_STATIC(LogLiveCoding, Log, All);

ALiveCodingDemoActor::ALiveCodingDemoActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALiveCodingDemoActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogLiveCoding, Warning, TEXT("===== Live Coding Demo BeginPlay ====="));
	PrintCurrentState();
}

void ALiveCodingDemoActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 持续旋转 —— 改 RotationSpeed 后 Live Coding 立即生效
	AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));

	// 每 60 帧打印一次计数器 —— 验证 Live Coding 不重置运行时状态
	TickCounter++;
	if (TickCounter % 60 == 0)
	{
		UE_LOG(LogLiveCoding, Log, TEXT("Tick 计数: %d（Live Coding 不会重置这个值）"), TickCounter);
	}
}

void ALiveCodingDemoActor::PrintCurrentState()
{
	UE_LOG(LogLiveCoding, Log, TEXT("消息: %s"), *Message);
	UE_LOG(LogLiveCoding, Log, TEXT("旋转速度: %.1f"), RotationSpeed);
	UE_LOG(LogLiveCoding, Log, TEXT("颜色: %s"), *DisplayColor.ToString());
	UE_LOG(LogLiveCoding, Log, TEXT("CalculateSomething(42) = %d"), CalculateSomething(42));
}

int32 ALiveCodingDemoActor::CalculateSomething(int32 Input) const
{
	// 故意写成简单逻辑 —— 让你改成别的算法后 Live Coding 验证
	// 例如：把 return Input * 2; 改成 return Input * Input;
	return Input * 2;
}
