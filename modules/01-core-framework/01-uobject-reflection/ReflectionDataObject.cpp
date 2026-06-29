// ReflectionDataObject.cpp
// ============================================================
// 知识点 1.1 — UObject & UFUNCTION / UPROPERTY 宏
// ============================================================

#include "ReflectionDataObject.h"
// UnrealType.h 提供 TFieldIterator / FProperty / UFunction 等反射 API
#include "UObject/UnrealType.h"

DEFINE_LOG_CATEGORY_STATIC(LogReflection, Log, All);

UReflectionDataObject::UReflectionDataObject()
{
	// FText 用 INVTEXT 宏创建非本地化的不变文本
	// （正式项目应该用 NSLOCTEXT 做本地化）
	Description = INVTEXT("通过反射创建的数据对象 —— 演示 UPROPERTY/UFUNCTION");
}

// ━━━━━━━━━━━━━━━━ UFUNCTION 实现 ━━━━━━━━━━━━━━━━

void UReflectionDataObject::TakeDamage(int32 DamageAmount)
{
	if (bIsInvincible)
	{
		UE_LOG(LogReflection, Warning, TEXT("无敌状态！伤害 %d 被免疫"), DamageAmount);
		return;
	}

	int32 OldHealth = Health;
	Health = FMath::Max(0, Health - DamageAmount);
	HealthPercent = MaxHealth > 0 ? static_cast<float>(Health) / MaxHealth : 0.0f;

	// 记录伤害历史
	DamageHistory.Add(DamageAmount);

	UE_LOG(LogReflection, Log, TEXT("受到 %d 伤害 → 血量: %d/%d (%.0f%%)"),
		DamageAmount, Health, MaxHealth, HealthPercent * 100.0f);

	// 调用 BlueprintImplementableEvent
	// 如果蓝图实现了 OnHealthChanged → 执行蓝图逻辑
	// 如果蓝图没实现 → 空操作（no-op），不会崩溃
	OnHealthChanged(Health, OldHealth);
}

bool UReflectionDataObject::IsAlive() const
{
	return Health > 0;
}

void UReflectionDataObject::GetHealthInfo(int32& OutCurrentHealth, int32& OutMaxHealth, float& OutPercent) const
{
	OutCurrentHealth = Health;
	OutMaxHealth = MaxHealth;
	OutPercent = HealthPercent;
}

void UReflectionDataObject::InitializeDefaults_Implementation()
{
	// BlueprintNativeEvent 的 C++ 默认实现
	// 注意函数名后缀 _Implementation —— 这是 UE 的命名约定
	// 如果蓝图没有重写 InitializeDefaults，就执行这里的代码
	Health = MaxHealth;
	HealthPercent = 1.0f;
	DamageHistory.Empty();

	UE_LOG(LogReflection, Log, TEXT("InitializeDefaults (C++ 默认实现) → 血量已重置为 %d"), Health);
}

void UReflectionDataObject::ResetHealth()
{
	Health = MaxHealth;
	HealthPercent = 1.0f;

	UE_LOG(LogReflection, Log, TEXT("血量已重置 → %d/%d"), Health, MaxHealth);
}

// ━━━━━━━━━━━━━━━━ 反射内省 ━━━━━━━━━━━━━━━━

void UReflectionDataObject::PrintClassInfo() const
{
	UE_LOG(LogReflection, Warning, TEXT("===== 类反射信息 ====="));

	// GetClass() 返回 UClass* —— 这是这个对象的反射元数据
	// UClass 里记录了：类名、父类、所有属性、所有函数、接口……
	UClass* Class = GetClass();

	// ━━ 类名 —— 最常用的反射查询 ━━
	// GetName() 返回不带前缀的类名（"ReflectionDataObject"）
	UE_LOG(LogReflection, Log, TEXT("类名 (GetName):         %s"), *Class->GetName());

	// GetPathName() 返回完整路径（包含包路径）
	UE_LOG(LogReflection, Log, TEXT("路径 (GetPathName):      %s"), *Class->GetPathName());

	// GetPrefixCPP() 返回 C++ 前缀（"U" for UObject, "A" for Actor）
	// 注意：返回值是临时 FString，先存到局部变量再取 *
	FString PrefixCPP = Class->GetPrefixCPP();
	UE_LOG(LogReflection, Log, TEXT("C++ 前缀 (GetPrefixCPP): %s"), *PrefixCPP);

	// IsChildOf —— 继承关系检查（反射版 instanceof）
	UE_LOG(LogReflection, Log, TEXT("是 UObject 子类: %s"),
		Class->IsChildOf<UObject>() ? TEXT("是") : TEXT("否"));

	// ━━ 统计属性数量 ━━
	// TFieldIterator<FProperty> 遍历类（包括父类）的所有反射属性
	int32 PropCount = 0;
	for (TFieldIterator<FProperty> It(Class); It; ++It)
	{
		PropCount++;
	}
	UE_LOG(LogReflection, Log, TEXT("属性数量: %d"), PropCount);

	// ━━ 统计函数数量 ━━
	// TFieldIterator<UFunction> 遍历类（包括父类）的所有反射函数
	int32 FuncCount = 0;
	for (TFieldIterator<UFunction> It(Class); It; ++It)
	{
		FuncCount++;
	}
	UE_LOG(LogReflection, Log, TEXT("函数数量: %d"), FuncCount);

	UE_LOG(LogReflection, Warning, TEXT("===== 反射信息结束 ====="));
}
