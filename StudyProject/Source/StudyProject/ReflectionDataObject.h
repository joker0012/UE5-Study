// ReflectionDataObject.h
// ============================================================
// 知识点 1.1 — UObject & UFUNCTION / UPROPERTY 宏
// 这个类演示 UE5 反射系统的核心宏：
//   UCLASS    —— 声明一个可被反射的 UObject 派生类
//   UPROPERTY —— 声明一个可被反射的属性（编辑器可见/蓝图可读写/序列化）
//   UFUNCTION —— 声明一个可被反射的函数（蓝图可调用/可重写）
//
// 反射是 UE5 的基石：序列化、蓝图交互、编辑器编辑、网络复制、
// GC 垃圾回收……全都依赖反射元数据。
// ============================================================

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReflectionDataObject.generated.h"

/**
 * 一个纯数据对象（非 Actor），演示反射宏的各种用法。
 *
 * 继承 UObject    —— 获得 GC、反射、序列化能力
 * BlueprintType   —— 可在蓝图中作为变量类型使用
 * Blueprintable   —— 可在内容浏览器中创建蓝图子类
 */
UCLASS(BlueprintType, Blueprintable, Category = "StudyProject|Reflection")
class STUDYPROJECT_API UReflectionDataObject : public UObject
{
	GENERATED_BODY()

public:
	UReflectionDataObject();

	// ━━━━━━━━━━━━━━━━ UPROPERTY 演示 ━━━━━━━━━━━━━━━━

	// --- 访问权限组合 ---

	// EditAnywhere + BlueprintReadWrite
	//   编辑器：任意实例都可编辑（Details 面板）
	//   蓝图：  可读取、可修改
	//   meta(ClampMin/ClampMax)：编辑器中限制输入范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reflection|Basic",
		meta = (ClampMin = "0", ClampMax = "999"))
	int32 Health = 100;

	// EditDefaultsOnly + BlueprintReadWrite
	//   编辑器：只有 CDO（Class Default Object）可编辑 → 子实例继承默认值
	//   蓝图：  可读写
	//   适用场景：所有实例共享的基础属性（如最大血量）
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Reflection|Basic",
		meta = (ClampMin = "1"))
	int32 MaxHealth = 100;

	// VisibleAnywhere + BlueprintReadOnly
	//   编辑器：任意实例都可见但只读（灰色显示）
	//   蓝图：  只读
	//   适用场景：运行时计算的只读状态（如当前血量百分比）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reflection|Basic")
	float HealthPercent = 1.0f;

	// --- 数据类型演示 ---

	// FString —— 可变字符串，最常用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reflection|Strings")
	FString DisplayName = TEXT("反射数据对象");

	// FText —— 本地化文本，支持多语言
	//   显示给玩家的文字应该用 FText，而不是 FString
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reflection|Strings")
	FText Description;

	// FName —— 不可变名称，高效比较（内部用 ID 表，比较是整数比较）
	//   适合做标识符/Tag，不适合做显示文本
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reflection|Strings")
	FName ObjectTag = TEXT("Default");

	// --- 高级 meta ---

	// EditCondition：条件编辑
	//   当 bIsInvincible 为 true 时，InvincibleDuration 才可编辑
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reflection|Advanced")
	bool bIsInvincible = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reflection|Advanced",
		meta = (EditCondition = "bIsInvincible", ClampMin = "0.0"))
	float InvincibleDuration = 5.0f;

	// 结构体属性 —— FVector 是 UE 内置的反射结构体
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reflection|Advanced")
	FVector SpawnLocation = FVector::ZeroVector;

	// 数组属性 —— TArray 是 UE 内置的反射容器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reflection|Advanced")
	TArray<int32> DamageHistory;

	// --- private 属性 + AllowPrivateAccess ---
	// private 成员默认不暴露给蓝图/编辑器
	// meta(AllowPrivateAccess = "true") 允许突破访问限制
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reflection|Private",
		meta = (AllowPrivateAccess = "true"))
	int32 SecretCode = 42;

	// ━━━━━━━━━━━━━━━━ UFUNCTION 演示 ━━━━━━━━━━━━━━━━

	// BlueprintCallable —— 蓝图可调用（有白色执行引脚）
	//   适合有副作用的操作（修改状态、触发事件）
	UFUNCTION(BlueprintCallable, Category = "Reflection|Actions")
	void TakeDamage(int32 DamageAmount);

	// BlueprintPure —— 蓝图纯函数（无执行引脚）
	//   没有 exec 引脚，适合无副作用的查询
	//   蓝图里表现为一个节点，只有数据输出引脚
	UFUNCTION(BlueprintPure, Category = "Reflection|Query")
	bool IsAlive() const;

	// BlueprintPure 返回多个值 —— 蓝图中每个 out 参数都是一个输出引脚
	UFUNCTION(BlueprintPure, Category = "Reflection|Query")
	void GetHealthInfo(int32& OutCurrentHealth, int32& OutMaxHealth, float& OutPercent) const;

	// BlueprintNativeEvent —— C++ 提供默认实现，蓝图可以重写
	//   .cpp 中实现 InitializeDefaults_Implementation
	//   如果蓝图没重写，执行 C++ 版本；重写了则执行蓝图版本
	UFUNCTION(BlueprintNativeEvent, Category = "Reflection|Actions")
	void InitializeDefaults();

	// BlueprintImplementableEvent —— C++ 不提供实现，蓝图必须实现
	//   如果蓝图没实现，调用时是空操作（no-op）
	//   适合做回调/事件通知：C++ 触发，蓝图响应
	UFUNCTION(BlueprintImplementableEvent, Category = "Reflection|Events")
	void OnHealthChanged(int32 NewHealth, int32 OldHealth);

	// meta(DisplayName) —— 在蓝图中显示自定义名称（中文友好）
	UFUNCTION(BlueprintCallable, Category = "Reflection|Actions",
		meta = (DisplayName = "重置血量"))
	void ResetHealth();

	// ━━━━━━━━━━━━━━━━ 反射内省函数 ━━━━━━━━━━━━━━━━

	// 打印类信息 —— 演示 GetClass() 反射查询
	//   GetClass() 返回 UClass*，这是这个对象的反射元数据入口
	UFUNCTION(BlueprintCallable, Category = "Reflection|Introspection")
	void PrintClassInfo() const;
};
