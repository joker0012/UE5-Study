// ReflectionDemoActor.cpp
// ============================================================
// 知识点 1.1 — UObject & UFUNCTION / UPROPERTY 宏
// ============================================================

#include "ReflectionDemoActor.h"
#include "ReflectionDataObject.h"
// 反射 API 头文件：TFieldIterator / FProperty / 各种 F*Property
#include "UObject/UnrealType.h"

DEFINE_LOG_CATEGORY_STATIC(LogReflectionDemo, Log, All);

AReflectionDemoActor::AReflectionDemoActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AReflectionDemoActor::BeginPlay()
{
	Super::BeginPlay();

	RunReflectionDemo();
}

// ━━━━━━━━━━━━━━━━ 主演示函数 ━━━━━━━━━━━━━━━━

void AReflectionDemoActor::RunReflectionDemo()
{
	UE_LOG(LogReflectionDemo, Warning, TEXT("===== 知识点 1.1 — UObject & UFUNCTION / UPROPERTY ====="));

	// ━━━━ 1. GetClass()->GetName() —— 最基础的反射查询 ━━━━
	UE_LOG(LogReflectionDemo, Warning, TEXT("--- 1. GetClass()->GetName() ---"));
	UE_LOG(LogReflectionDemo, Log, TEXT("Actor 类名: %s"), *GetClass()->GetName());

	// ━━━━ 2. NewObject<T>() —— 运行时创建 UObject 实例 ━━━━
	UE_LOG(LogReflectionDemo, Warning, TEXT("--- 2. NewObject<T>() 创建实例 ---"));
	// this 作为 Outer → Actor 被 GC 时数据对象也会被回收
	DataObject = NewObject<UReflectionDataObject>(this);
	UE_LOG(LogReflectionDemo, Log, TEXT("创建对象: %s"), *DataObject->GetName());
	UE_LOG(LogReflectionDemo, Log, TEXT("对象类名: %s"), *DataObject->GetClass()->GetName());

	// ━━━━ 3. 数据对象自带的反射信息 ━━━━
	UE_LOG(LogReflectionDemo, Warning, TEXT("--- 3. 数据对象反射信息 ---"));
	DataObject->PrintClassInfo();

	// ━━━━ 4. TFieldIterator 遍历所有属性 ━━━━
	UE_LOG(LogReflectionDemo, Warning, TEXT("--- 4. TFieldIterator 遍历属性 ---"));
	PrintAllProperties(DataObject);

	// ━━━━ 5. 调用各种 UFUNCTION ━━━━
	UE_LOG(LogReflectionDemo, Warning, TEXT("--- 5. 调用 UFUNCTION ---"));

	// BlueprintNativeEvent 调用（走 C++ 默认实现）
	DataObject->InitializeDefaults();

	// BlueprintCallable 调用（有副作用）
	DataObject->TakeDamage(30);
	DataObject->TakeDamage(25);

	// BlueprintPure 调用（无副作用查询）
	UE_LOG(LogReflectionDemo, Log, TEXT("IsAlive() = %s"),
		DataObject->IsAlive() ? TEXT("true") : TEXT("false"));

	// BlueprintPure 多返回值
	int32 CurHP = 0, MaxHP = 0;
	float Percent = 0.0f;
	DataObject->GetHealthInfo(CurHP, MaxHP, Percent);
	UE_LOG(LogReflectionDemo, Log, TEXT("GetHealthInfo → HP: %d/%d (%.0f%%)"),
		CurHP, MaxHP, Percent * 100.0f);

	// ━━━━ 6. 属性变更后重新遍历（对比变化）━━━━━━
	UE_LOG(LogReflectionDemo, Warning, TEXT("--- 6. 属性变更后的反射遍历 ---"));
	PrintAllProperties(DataObject);

	// ━━━━ 7. FindPropertyByName —— 按名称查找并修改属性 ━━━━
	UE_LOG(LogReflectionDemo, Warning, TEXT("--- 7. FindPropertyByName 反射读写 ---"));
	FProperty* FoundProp = DataObject->GetClass()->FindPropertyByName(TEXT("Health"));
	if (FoundProp)
	{
		// ContainerPtrToValuePtr —— 从对象指针拿到属性的内存地址
		void* ValuePtr = FoundProp->ContainerPtrToValuePtr<void>(DataObject);

		// CastField<FIntProperty> —— 把 FProperty 转型为具体的属性类型
		if (FIntProperty* IntProp = CastField<FIntProperty>(FoundProp))
		{
			// 反射读取
			int32 Value = IntProp->GetPropertyValue(ValuePtr);
			UE_LOG(LogReflectionDemo, Log, TEXT("找到属性 'Health' → 类型: %s, 当前值: %d"),
				*FoundProp->GetCPPType(), Value);

			// 反射写入 —— 不通过 C++ setter，直接通过反射改值
			IntProp->SetPropertyValue(ValuePtr, 999);
			UE_LOG(LogReflectionDemo, Log, TEXT("通过反射修改 → Health = 999"));

			// 验证修改是否生效
			UE_LOG(LogReflectionDemo, Log, TEXT("验证: DataObject->Health = %d"), DataObject->Health);
		}
	}
	else
	{
		UE_LOG(LogReflectionDemo, Warning, TEXT("未找到属性 'Health'"));
	}

	UE_LOG(LogReflectionDemo, Warning, TEXT("===== 反射演示完成 ====="));
}

// ━━━━━━━━━━━━━━━━ 反射辅助函数 ━━━━━━━━━━━━━━━━

void AReflectionDemoActor::PrintAllProperties(UObject* Object)
{
	if (!Object) return;

	UClass* Class = Object->GetClass();
	UE_LOG(LogReflectionDemo, Log, TEXT("对象: %s, 类: %s"), *Object->GetName(), *Class->GetName());

	int32 Index = 0;
	for (TFieldIterator<FProperty> It(Class); It; ++It)
	{
		FProperty* Property = *It;

		// ━━ 属性基本信息 ━━
		UE_LOG(LogReflectionDemo, Log, TEXT("  [%d] %s (%s)"),
			Index,
			*Property->GetName(),        // 属性名
			*Property->GetCPPType());    // C++ 类型字符串

		// ━━ 属性标志（EPropertyFlags）━━
		// 这些标志决定了属性在编辑器/蓝图中的行为
		FString Flags;
		if (Property->HasAnyPropertyFlags(CPF_Edit))             Flags += TEXT("Edit ");
		if (Property->HasAnyPropertyFlags(CPF_BlueprintVisible))  Flags += TEXT("BPVisible ");
		if (Property->HasAnyPropertyFlags(CPF_BlueprintReadOnly)) Flags += TEXT("BPReadOnly ");
		if (Property->HasAnyPropertyFlags(CPF_Transient))         Flags += TEXT("Transient ");
		UE_LOG(LogReflectionDemo, Log, TEXT("      标志: %s"),
			Flags.Len() > 0 ? *Flags : TEXT("(无)"));

		// ━━ 打印属性当前值 ━━
		PrintPropertyValue(Property, Object);

		Index++;
	}
}

void AReflectionDemoActor::PrintPropertyValue(FProperty* Property, void* ContainerPtr)
{
	// ContainerPtrToValuePtr —— 从对象内存基地址算出属性的偏移地址
	void* ValuePtr = Property->ContainerPtrToValuePtr<void>(ContainerPtr);

	// ━━ 按属性类型分别取值 ━━
	// CastField<T> 是 FProperty 专用的类型转换（类似 UObject 的 Cast<T>）
	// 转换成功说明属性是 T 类型，然后调用 T 特有的 GetPropertyValue

	if (FIntProperty* IntProp = CastField<FIntProperty>(Property))
	{
		UE_LOG(LogReflectionDemo, Log, TEXT("      值: %d"), IntProp->GetPropertyValue(ValuePtr));
	}
	else if (FFloatProperty* FloatProp = CastField<FFloatProperty>(Property))
	{
		UE_LOG(LogReflectionDemo, Log, TEXT("      值: %.2f"), FloatProp->GetPropertyValue(ValuePtr));
	}
	else if (FBoolProperty* BoolProp = CastField<FBoolProperty>(Property))
	{
		UE_LOG(LogReflectionDemo, Log, TEXT("      值: %s"),
			BoolProp->GetPropertyValue(ValuePtr) ? TEXT("true") : TEXT("false"));
	}
	else if (FStrProperty* StrProp = CastField<FStrProperty>(Property))
	{
		UE_LOG(LogReflectionDemo, Log, TEXT("      值: \"%s\""), *StrProp->GetPropertyValue(ValuePtr));
	}
	else if (FNameProperty* NameProp = CastField<FNameProperty>(Property))
	{
		UE_LOG(LogReflectionDemo, Log, TEXT("      值: %s"),
			*NameProp->GetPropertyValue(ValuePtr).ToString());
	}
	else if (FTextProperty* TextProp = CastField<FTextProperty>(Property))
	{
		UE_LOG(LogReflectionDemo, Log, TEXT("      值: %s"),
			*TextProp->GetPropertyValue(ValuePtr).ToString());
	}
	else if (FObjectProperty* ObjProp = CastField<FObjectProperty>(Property))
	{
		UObject* Obj = ObjProp->GetObjectPropertyValue(ValuePtr);
		UE_LOG(LogReflectionDemo, Log, TEXT("      值: %s"),
			Obj ? *Obj->GetName() : TEXT("nullptr"));
	}
	else if (FStructProperty* StructProp = CastField<FStructProperty>(Property))
	{
		// 结构体属性 —— 检查是否是 FVector
		if (StructProp->Struct == TBaseStructure<FVector>::Get())
		{
			FVector* Vec = reinterpret_cast<FVector*>(ValuePtr);
			UE_LOG(LogReflectionDemo, Log, TEXT("      值: %s"), *Vec->ToString());
		}
		else
		{
			UE_LOG(LogReflectionDemo, Log, TEXT("      值: (struct: %s)"),
				*StructProp->Struct->GetName());
		}
	}
	else if (FArrayProperty* ArrayProp = CastField<FArrayProperty>(Property))
	{
		// 数组属性 —— 用 FScriptArrayHelper 访问
		FScriptArrayHelper ArrayHelper(ArrayProp, ValuePtr);
		UE_LOG(LogReflectionDemo, Log, TEXT("      值: Array[%d]"), ArrayHelper.Num());
	}
	else
	{
		// 其他类型（枚举、Map、Set 等）—— 只显示类型名
		UE_LOG(LogReflectionDemo, Log, TEXT("      值: (%s)"),
			*Property->GetClass()->GetName());
	}
}
