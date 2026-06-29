// DebugMacrosDemoActor.h
// ============================================================
// 知识点 1.2 — UE_LOG / check / ensure 调试宏体系
// 这个 Actor 演示 UE5 的三大调试宏家族：
//   1. UE_LOG —— 日志输出（LogTemp / 自定义 Category / 不同级别）
//   2. check  —— 断言（失败就崩，Development 编译生效，Shipping 自动移除）
//   3. ensure —— 软断言（失败不崩但断点，返回 bool 可分支处理）
//
// 额外覆盖：UE_LOG 格式化、DEFINE_LOG_CATEGORY、verify、checkSlow
// ============================================================

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebugMacrosDemoActor.generated.h"

UCLASS(BlueprintType, Category = "StudyProject|Debug")
class STUDYPROJECT_API ADebugMacrosDemoActor : public AActor
{
	GENERATED_BODY()

public:
	ADebugMacrosDemoActor();

protected:
	virtual void BeginPlay() override;

public:
	// ━━━━━━━━━━━━━━━━ UPROPERTY 配置 ━━━━━━━━━━━━━━━━

	// 测试用的可空指针 —— 演示 check/ensure 对空指针的处理
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug|Test")
	UObject* TestObject = nullptr;

	// 测试用的数值 —— 演示 check 对数值条件的验证
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug|Test",
		meta = (ClampMin = "0", ClampMax = "100"))
	int32 PlayerLevel = 1;

	// 控制是否触发 ensure 失败的开关
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug|Test")
	bool bTriggerEnsureFail = true;

	// 控制是否触发 check 失败的开关
	// ⚠️ 开启后 Play 会直接崩溃（这是 check 的设计行为）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug|Test",
		meta = (DisplayName = "触发 check 崩溃 (慎用)"))
	bool bTriggerCheckFail = false;

	// ━━━━━━━━━━━━━━━━ UFUNCTION 演示入口 ━━━━━━━━━━━━━━━━

	// 运行全部演示 —— 在 BeginPlay 中自动调用，也可手动调用
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void RunAllDemos();

private:
	// ━━━━━━━━━━━━━━━━ 各阶段演示 ━━━━━━━━━━━━━━━━

	// 1. UE_LOG 基础用法 + 格式化
	void Demo_UELog();

	// 2. 自定义 Log Category
	void Demo_LogCategory();

	// 3. check 宏家族（check / checkf / checkSlow / checkCode）
	void Demo_Check();

	// 4. ensure 宏家族（ensure / ensureMsgf / ensureAlways）
	void Demo_Ensure();

	// 5. verify 宏 —— 和 check 类似但 Shipping 也会执行
	void Demo_Verify();

	// 6. 其他实用调试宏
	void Demo_OtherMacros();
};
