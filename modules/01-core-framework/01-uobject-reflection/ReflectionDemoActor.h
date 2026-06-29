// ReflectionDemoActor.h
// ============================================================
// 知识点 1.1 — UObject & UFUNCTION / UPROPERTY 宏
// 这个 Actor 演示：
//   1. 用 NewObject<T>() 创建 UObject 实例
//   2. 用 GetClass()->GetName() 查询类名
//   3. 用 TFieldIterator 遍历所有反射属性
//   4. 用 FindPropertyByName 按名称查找并修改属性
//   5. 调用各种 UFUNCTION（Callable / Pure / NativeEvent）
// ============================================================

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReflectionDemoActor.generated.h"

class UReflectionDataObject;

UCLASS(BlueprintType, Category = "StudyProject|Reflection")
class STUDYPROJECT_API AReflectionDemoActor : public AActor
{
	GENERATED_BODY()

public:
	AReflectionDemoActor();

protected:
	virtual void BeginPlay() override;

public:
	// ━━━━━━━━━━━━━━━━ UPROPERTY 演示 ━━━━━━━━━━━━━━━━

	// 持有一个数据对象指针
	// VisibleAnywhere —— 编辑器可见（只读）
	// BlueprintReadOnly —— 蓝图可读
	// 注意：UObject* 指针必须加 UPROPERTY()，否则 GC 可能回收它
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reflection")
	UReflectionDataObject* DataObject;

	// ━━━━━━━━━━━━━━━━ UFUNCTION 演示 ━━━━━━━━━━━━━━━━

	// 手动触发反射演示（也可以在蓝图中调用）
	UFUNCTION(BlueprintCallable, Category = "Reflection")
	void RunReflectionDemo();

private:
	// 遍历对象的所有属性并打印 —— 演示 TFieldIterator 反射内省
	void PrintAllProperties(UObject* Object);

	// 按类型打印属性值 —— 演示 FProperty 类型系统
	void PrintPropertyValue(FProperty* Property, void* ContainerPtr);
};
