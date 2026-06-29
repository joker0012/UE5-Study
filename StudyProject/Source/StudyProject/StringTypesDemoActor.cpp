// StringTypesDemoActor.cpp
// ============================================================
// 知识点 1.3 — FName / FString / FText 区别与转换
// ============================================================

#include "StringTypesDemoActor.h"
// Text.h 提供 FText 的各种构造和转换方法
#include "Internationalization/Text.h"
// Namespace.h 提供 NSLOCTEXT 宏
#include "Internationalization/Internationalization.h"

DEFINE_LOG_CATEGORY_STATIC(LogStringTypes, Log, All);

// ━━━━━━━━━━━━━━━━ 命名空间定义（用于 NSLOCTEXT）━━━━━━━━━━━━━━━━
// NSLOCTEXT(Namespace, Key, SourceString)
//   Namespace —— 文本命名空间，用于组织本地化条目
//   Key       —— 唯一键，本地化系统用这个查找翻译
//   SourceString —— 源语言（通常是英文）的原文
#define LOCTEXT_NAMESPACE "StringTypesDemo"

AStringTypesDemoActor::AStringTypesDemoActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AStringTypesDemoActor::BeginPlay()
{
	Super::BeginPlay();

	RunAllDemos();
}

void AStringTypesDemoActor::RunAllDemos()
{
	UE_LOG(LogStringTypes, Warning, TEXT("===== 知识点 1.3 — FName / FString / FText ====="));

	Demo_FName();
	Demo_FString();
	Demo_FText();
	Demo_Conversions();
	Demo_Performance();
	Demo_Pitfalls();

	UE_LOG(LogStringTypes, Warning, TEXT("===== 字符串类型演示完成 ====="));
}

// ━━━━━━━━━━━━━━━━ 1. FName —— 轻量标识符 ━━━━━━━━━━━━━━━━

void AStringTypesDemoActor::Demo_FName()
{
	UE_LOG(LogStringTypes, Warning, TEXT("--- 1. FName —— 轻量标识符 ---"));

	// ━━ 本质：全局名字表（Name Table）中的索引 ━━
	// FName 内部只存一个 ID（整数），不存字符串内容
	// 相同字符串的 FName 只在表中存一份 → 内存高效

	// 创建方式
	FName Name1 = TEXT("Player");
	FName Name2 = TEXT("Player");
	FName Name3 = FName(TEXT("Different"));

	// ━━ 比较 O(1) —— 比较的是 ID，不是字符串内容 ━━
	// 这是 FName 最大的优势
	bool bSame = (Name1 == Name2);  // true —— 同一字符串 → 同一 ID
	UE_LOG(LogStringTypes, Log, TEXT("Name1 == Name2: %s"), bSame ? TEXT("true") : TEXT("false"));

	// ━━ 不可变 —— 不能修改 FName 的内容 ━━
	// 只能重新赋值一个新的 FName
	Name1 = TEXT("Changed");  // 这不是"修改"，是重新指向新的表条目

	// ━━ 常用方法 ━━
	FName MyName = TEXT("TestActor");
	UE_LOG(LogStringTypes, Log, TEXT("ToString: %s"), *MyName.ToString());     // → "TestActor"
	UE_LOG(LogStringTypes, Log, TEXT("IsNone: %s"), MyName.IsNone() ? TEXT("true") : TEXT("false"));  // → false

	// ━━ 比较：== 运算符 vs Compare ━━
	// == 运算符 —— 比较 ID，O(1)
	// Compare() —— 按字典序比较字符串内容，用于排序
	FName A = TEXT("Apple");
	FName B = TEXT("Banana");
	int32 CmpResult = A.Compare(B);  // < 0 表示 A < B
	UE_LOG(LogStringTypes, Log, TEXT("Apple.Compare(Banana) = %d (负数=前者小)"), CmpResult);

	// ━━ 适用场景 ━━
	UE_LOG(LogStringTypes, Log, TEXT("  适用：Bone名、Socket名、Tag名、属性名、资源路径标识"));
	UE_LOG(LogStringTypes, Log, TEXT("  不适用：显示给玩家的文本、需要拼接修改的字符串"));

	// ━━ 注意：FName 大小写不敏感 ━━
	FName Upper = TEXT("Test");
	FName Lower = TEXT("test");
	UE_LOG(LogStringTypes, Log, TEXT("'Test' == 'test': %s (FName 大小写不敏感)"),
		(Upper == Lower) ? TEXT("true") : TEXT("false"));
}

// ━━━━━━━━━━━━━━━━ 2. FString —— 通用字符串 ━━━━━━━━━━━━━━━━

void AStringTypesDemoActor::Demo_FString()
{
	UE_LOG(LogStringTypes, Warning, TEXT("--- 2. FString —— 通用字符串 ---"));

	// ━━ 本质：可变的 TCHAR 动态数组 ━━
	// 类似 std::string，可以拼接、查找、替换、格式化

	// 创建
	FString Str1 = TEXT("Hello");
	FString Str2 = TEXT(" World");

	// ━━ 拼接 ━━
	FString Combined = Str1 + Str2;
	UE_LOG(LogStringTypes, Log, TEXT("拼接: %s + %s = \"%s\""), *Str1, *Str2, *Combined);

	// += 运算符
	FString Greeting = TEXT("Welcome");
	Greeting += TEXT(", ");
	Greeting += TEXT("Hero!");
	UE_LOG(LogStringTypes, Log, TEXT("+= 拼接: \"%s\""), *Greeting);

	// ━━ 格式化 —— Printf / Format ━━
	FString Formatted = FString::Printf(TEXT("等级: %d, 血量: %.1f, 名字: %s"), 25, 850.5f, TEXT("勇者"));
	UE_LOG(LogStringTypes, Log, TEXT("Printf: %s"), *Formatted);

	// FString::Format —— 参数化格式（更安全）
	TArray<FStringFormatArg> Args = {
		FStringFormatArg(25),                    // int32
		FStringFormatArg(850.5f),                // float
		FStringFormatArg(TEXT("勇者"))            // FString
	};
	FString Formatted2 = FString::Format(TEXT("等级: {0}, 血量: {1:.1f}, 名字: {2}"), Args);
	UE_LOG(LogStringTypes, Log, TEXT("Format: %s"), *Formatted2);

	// ━━ 常用方法 ━━
	FString Sentence = TEXT("The Quick Brown Fox");

	// 长度
	UE_LOG(LogStringTypes, Log, TEXT("Len(\"%s\") = %d"), *Sentence, Sentence.Len());

	// 大小写转换
	UE_LOG(LogStringTypes, Log, TEXT("ToUpper: %s"), *Sentence.ToUpper());
	UE_LOG(LogStringTypes, Log, TEXT("ToLower: %s"), *Sentence.ToLower());

	// 包含查找
	bool bContains = Sentence.Contains(TEXT("Fox"));
	UE_LOG(LogStringTypes, Log, TEXT("Contains(\"Fox\"): %s"), bContains ? TEXT("true") : TEXT("false"));

	// 查找位置
	int32 Pos = Sentence.Find(TEXT("Brown"));
	UE_LOG(LogStringTypes, Log, TEXT("Find(\"Brown\"): 位置 %d"), Pos);

	// 子串
	FString Sub = Sentence.Mid(4, 5);  // 从位置4取5个字符 → "Quick"
	UE_LOG(LogStringTypes, Log, TEXT("Mid(4,5): \"%s\""), *Sub);

	// 替换
	FString Replaced = Sentence.Replace(TEXT("Fox"), TEXT("Dog"));
	UE_LOG(LogStringTypes, Log, TEXT("Replace(Fox→Dog): %s"), *Replaced);

	// ━━ 字符串 ↔ 数值转换 ━━
	FString NumStr = TEXT("42");
	int32 Num = FCString::Atoi(*NumStr);       // 字符串 → int32
	float Flt = FCString::Atof(*NumStr);       // 字符串 → float
	UE_LOG(LogStringTypes, Log, TEXT("Atoi(\"42\") = %d, Atof(\"42\") = %.2f"), Num, Flt);

	FString BackToStr = FString::FromInt(123);  // int32 → 字符串
	UE_LOG(LogStringTypes, Log, TEXT("FromInt(123) = \"%s\""), *BackToStr);

	// ━━ 分割 ━━
	FString CSV = TEXT("Apple,Banana,Cherry");
	TArray<FString> Parts;
	CSV.ParseIntoArray(Parts, TEXT(","), true);  // true = 去除空字符串
	UE_LOG(LogStringTypes, Log, TEXT("ParseIntoArray(\",\"): %d 个部分"), Parts.Num());
	for (int32 i = 0; i < Parts.Num(); i++)
	{
		UE_LOG(LogStringTypes, Log, TEXT("  [%d] %s"), i, *Parts[i]);
	}

	// ━━ 适用场景 ━━
	UE_LOG(LogStringTypes, Log, TEXT("  适用：日志输出、字符串拼接/解析、文件路径操作、序列化"));
	UE_LOG(LogStringTypes, Log, TEXT("  不适用：对象标识符（用 FName）、显示给玩家的文本（用 FText）"));
}

// ━━━━━━━━━━━━━━━━ 3. FText —— 本地化文本 ━━━━━━━━━━━━━━━━

void AStringTypesDemoActor::Demo_FText()
{
	UE_LOG(LogStringTypes, Warning, TEXT("--- 3. FText —— 本地化文本 ---"));

	// ━━ 本质：不可变的、支持本地化的文本 ━━
	// FText 内部存的是：Namespace + Key + SourceString（+ 可选的翻译）
	// 设计目标：让同一段代码在不同语言环境下显示不同文字

	// ━━ 创建方式 ━━

	// 方式 1: INVTEXT —— 不变文本（不参与本地化）
	// 适合：调试文本、不面向玩家的内部文本
	FText InvText = INVTEXT("这是不变文本");
	UE_LOG(LogStringTypes, Log, TEXT("INVTEXT: %s"), *InvText.ToString());

	// 方式 2: NSLOCTEXT —— 带命名空间的本地化文本
	// 格式: NSLOCTEXT(Namespace, Key, SourceString)
	FText LocalizedText = NSLOCTEXT("StringTypesDemo", "Greeting", "Welcome, Hero!");
	UE_LOG(LogStringTypes, Log, TEXT("NSLOCTEXT: %s"), *LocalizedText.ToString());

	// 方式 3: LOCTEXT —— 使用当前文件定义的 LOCTEXT_NAMESPACE
	// 需要在文件顶部 #define LOCTEXT_NAMESPACE "XXX"
	// 用完 #undef LOCTEXT_NAMESPACE
	FText LocText = LOCTEXT("Farewell", "Goodbye, Hero!");
	UE_LOG(LogStringTypes, Log, TEXT("LOCTEXT: %s"), *LocText.ToString());

	// 方式 4: FText::FromString —— 从 FString 创建（不参与本地化）
	// 注意：这样创建的 FText 没有本地化信息，只适合临时使用
	FText FromStr = FText::FromString(TEXT("从FString创建"));
	UE_LOG(LogStringTypes, Log, TEXT("FromString: %s"), *FromStr.ToString());

	// 方式 5: FText::AsNumber / AsPercent 等 —— 格式化数字
	// 会根据当前语言环境（Culture）自动格式化
	FText NumText = FText::AsNumber(1234567);
	UE_LOG(LogStringTypes, Log, TEXT("AsNumber(1234567): %s"), *NumText.ToString());

	FText PercentText = FText::AsPercent(0.875f);
	UE_LOG(LogStringTypes, Log, TEXT("AsPercent(0.875): %s"), *PercentText.ToString());

	// ━━ FText 不可变 —— 不能直接修改 ━━
	// 没有拼接运算符（+ 不能用）
	// 但可以用 FText::Format 模板替换

	// FText::Format —— 用占位符 {0} {1} 替换
	FText Template = LOCTEXT("LevelUp", "{0} 升到了 {1} 级！");
	FText PlayerName = INVTEXT("勇者");
	FText Result = FText::Format(Template, PlayerName, FText::AsNumber(25));
	UE_LOG(LogStringTypes, Log, TEXT("Format: %s"), *Result.ToString());

	// ━━ 比较 FText ━━
	// 注意：FText 的 == 比较的是本地化身份（Namespace + Key），不是字符串内容！
	// 两个不同 Key 的 FText 即使显示文字相同，== 也返回 false
	FText A = NSLOCTEXT("Test", "Key1", "Hello");
	FText B = NSLOCTEXT("Test", "Key2", "Hello");
	UE_LOG(LogStringTypes, Log, TEXT("不同Key相同文字 ==: %s (FText比较的是身份不是内容)"),
		(A == B) ? TEXT("true") : TEXT("false"));

	// 如果要比较显示内容，用 ToString() 后比较
	UE_LOG(LogStringTypes, Log, TEXT("ToString() 相同文字 ==: %s"),
		(A.ToString() == B.ToString()) ? TEXT("true") : TEXT("false"));

	// FText::Identical —— 精确比较（含 Culture 信息）
	UE_LOG(LogStringTypes, Log, TEXT("Identical: %s"),
		A.IdenticalTo(B) ? TEXT("true") : TEXT("false"));

	// ━━ 当前语言环境 ━━
	FString Culture = FInternationalization::Get().GetCurrentCulture()->GetName();
	UE_LOG(LogStringTypes, Log, TEXT("当前语言环境: %s"), *Culture);

	// ━━ 适用场景 ━━
	UE_LOG(LogStringTypes, Log, TEXT("  适用：UI 显示文字、对话台词、提示信息、面向玩家的所有文本"));
	UE_LOG(LogStringTypes, Log, TEXT("  不适用：标识符（用 FName）、内部字符串操作（用 FString）"));
}

// ━━━━━━━━━━━━━━━━ 4. 三者互转 ━━━━━━━━━━━━━━━━

void AStringTypesDemoActor::Demo_Conversions()
{
	UE_LOG(LogStringTypes, Warning, TEXT("--- 4. FName / FString / FText 互转 ---"));

	// ━━ 转换关系图 ━━
	//
	//   FName  ──→ FString ──→ FText
	//     ↑          ↑          │
	//     │          │          │
	//     └──────────┴──────────┘ (有损/需指定)
	//
	// 箭头方向 = 安全/无损转换
	// 逆向 = 需要注意丢失信息

	// ━━ FName → FString (安全) ━━
	FName MyName = TEXT("Hero");
	FString NameToStr = MyName.ToString();
	UE_LOG(LogStringTypes, Log, TEXT("FName→FString: %s → \"%s\""), *MyName.ToString(), *NameToStr);

	// ━━ FString → FName (安全，但有去重开销) ━━
	FString Str = TEXT("Weapon_Sword");
	FName StrToName = FName(*Str);
	UE_LOG(LogStringTypes, Log, TEXT("FString→FName: \"%s\" → %s"), *Str, *StrToName.ToString());

	// ━━ FString → FText (安全，但不参与本地化) ━━
	FString DescStr = TEXT("一把锋利的剑");
	FText StrToText = FText::FromString(DescStr);
	UE_LOG(LogStringTypes, Log, TEXT("FString→FText: \"%s\" → %s"), *DescStr, *StrToText.ToString());

	// ━━ FName → FText (安全，但不参与本地化) ━━
	FName TagName = TEXT("Enemy_Boss");
	FText NameToText = FText::FromName(TagName);
	UE_LOG(LogStringTypes, Log, TEXT("FName→FText: %s → %s"), *TagName.ToString(), *NameToText.ToString());

	// ━━ FText → FString (有损！丢失本地化信息) ━━
	FText MyText = NSLOCTEXT("Demo", "Key1", "Hello World");
	FString TextToStr = MyText.ToString();
	UE_LOG(LogStringTypes, Log, TEXT("FText→FString (有损): %s → \"%s\""), *MyText.ToString(), *TextToStr);

	// ━━ FText → FName (有损！经过 FString 中转) ━━
	FName TextToName = FName(*MyText.ToString());
	UE_LOG(LogStringTypes, Log, TEXT("FText→FName (有损): %s → %s"), *MyText.ToString(), *TextToName.ToString());

	// ━━ 转换总结表 ━━
	UE_LOG(LogStringTypes, Warning, TEXT(""));
	UE_LOG(LogStringTypes, Warning, TEXT("┌──────────────────┬───────────────────────────────┐"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ 转换             │ 说明                          │"));
	UE_LOG(LogStringTypes, Warning, TEXT("├──────────────────┼───────────────────────────────┤"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ FName → FString  │ 安全无损                       │"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ FName → FText    │ 安全，但不参与本地化            │"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ FString → FName  │ 安全，但有 Name Table 查找开销  │"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ FString → FText  │ 安全，但不参与本地化            │"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ FText → FString  │ 有损！丢失本地化 Namespace/Key  │"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ FText → FName    │ 有损！经 FString 中转           │"));
	UE_LOG(LogStringTypes, Warning, TEXT("└──────────────────┴───────────────────────────────┘"));
}

// ━━━━━━━━━━━━━━━━ 5. 性能对比 ━━━━━━━━━━━━━━━━

void AStringTypesDemoActor::Demo_Performance()
{
	UE_LOG(LogStringTypes, Warning, TEXT("--- 5. 性能对比 ---"));

	// ━━ 比较 10 万次，对比 FName vs FString 的比较速度 ━━
	const int32 Iterations = 100000;

	// FName 比较 —— O(1)，比整数比较还快
	FName NameA = TEXT("TestName_Performance_Check");
	FName NameB = TEXT("TestName_Performance_Check");

	// FString 比较 —— O(n)，逐字符比较
	FString StrA = TEXT("TestName_Performance_Check");
	FString StrB = TEXT("TestName_Performance_Check");

	// FName 计时
	uint32 StartCycles = FPlatformTime::Cycles();
	for (int32 i = 0; i < Iterations; i++)
	{
		volatile bool bResult = (NameA == NameB);  // volatile 防止优化掉
		(void)bResult;
	}
	uint32 FNameCycles = FPlatformTime::Cycles() - StartCycles;

	// FString 计时
	StartCycles = FPlatformTime::Cycles();
	for (int32 i = 0; i < Iterations; i++)
	{
		volatile bool bResult = (StrA == StrB);
		(void)bResult;
	}
	uint32 FStringCycles = FPlatformTime::Cycles() - StartCycles;

	UE_LOG(LogStringTypes, Log, TEXT("%d 次比较:", Iterations));
	UE_LOG(LogStringTypes, Log, TEXT("  FName:   %u cycles", FNameCycles));
	UE_LOG(LogStringTypes, Log, TEXT("  FString: %u cycles", FStringCycles));
	UE_LOG(LogStringTypes, Log, TEXT("  FName 快 %.1fx", FStringCycles));

	// ━━ 内存占用对比 ━━
	UE_LOG(LogStringTypes, Log, TEXT(""));
	UE_LOG(LogStringTypes, Log, TEXT("内存占用 (sizeof):"));
	UE_LOG(LogStringTypes, Log, TEXT("  sizeof(FName)   = %d 字节 (只有 ID + Number)", sizeof(FName)));
	UE_LOG(LogStringTypes, Log, TEXT("  sizeof(FString) = %d 字节 (实际堆内存 + 数据)", sizeof(FString)));
	UE_LOG(LogStringTypes, Log, TEXT("  sizeof(FText)   = %d 字节 (本地化信息 + 文本)", sizeof(FText)));

	// ━━ 结论 ━━
	UE_LOG(LogStringTypes, Log, TEXT(""));
	UE_LOG(LogStringTypes, Log, TEXT("性能结论:"));
	UE_LOG(LogStringTypes, Log, TEXT("  · 大量比较/查找 → 用 FName (O(1) ID 比较)"));
	UE_LOG(LogStringTypes, Log, TEXT("  · 拼接/格式化   → 用 FString (可变字符串)"));
	UE_LOG(LogStringTypes, Log, TEXT("  · 面向玩家显示  → 用 FText (本地化)"));
}

// ━━━━━━━━━━━━━━━━ 6. 常见坑 & 最佳实践 ━━━━━━━━━━━━━━━━

void AStringTypesDemoActor::Demo_Pitfalls()
{
	UE_LOG(LogStringTypes, Warning, TEXT("--- 6. 常见坑 & 最佳实践 ---"));

	// ━━ 坑 1: UE_LOG 中 %s 的类型 ━━
	// %s 需要 const TCHAR*
	// FName → *Name.ToString()
	// FString → *Str
	// FText → *Text.ToString()
	FName Name = TEXT("Test");
	FString Str = TEXT("Hello");
	FText Text = INVTEXT("World");

	// ✅ 正确
	UE_LOG(LogStringTypes, Log, TEXT("正确: %s, %s, %s"),
		*Name.ToString(), *Str, *Text.ToString());

	// ❌ 错误（会编译报错或崩溃）
	// UE_LOG(LogStringTypes, Log, TEXT("错误: %s"), *Name);      // FName 不能直接 *
	// UE_LOG(LogStringTypes, Log, TEXT("错误: %s"), *Text);      // FText 不能直接 *

	// ━━ 坑 2: FText 不能用 + 拼接 ━━
	// ❌ FText A + FText B  —— 编译错误
	// ✅ 用 FText::Format
	FText Part1 = INVTEXT("你好");
	FText Part2 = INVTEXT("世界");
	FText Combined = FText::Format(INVTEXT("{0} {1}"), Part1, Part2);
	UE_LOG(LogStringTypes, Log, TEXT("FText 拼接用 Format: %s"), *Combined.ToString());

	// ━━ 坑 3: FName 大小写不敏感 ━━
	FName CaseA = TEXT("Hello");
	FName CaseB = TEXT("HELLO");
	UE_LOG(LogStringTypes, Log, TEXT("FName 'Hello' == 'HELLO': %s (大小写不敏感!)"),
		(CaseA == CaseB) ? TEXT("true") : TEXT("false"));

	// 如果需要大小写敏感的比较，转成 FString 再比
	UE_LOG(LogStringTypes, Log, TEXT("FString 'Hello' == 'HELLO': %s (大小写敏感)"),
		(CaseA.ToString() == CaseB.ToString()) ? TEXT("true") : TEXT("false"));

	// ━━ 坑 4: FText::FromString 创建的 FText 无法本地化 ━━
	// 用 FromString 创建的 FText 不会出现在本地化收集工具中
	// 正式项目应该用 NSLOCTEXT / LOCTEXT
	UE_LOG(LogStringTypes, Log, TEXT("坑4: FText::FromString 不参与本地化收集，正式项目用 NSLOCTEXT"));

	// ━━ 坑 5: FName 在不同模块中的 Name Table 不共享 ━━
	// FName 的 ID 在同一个进程中是全局的，但跨 DLL 边界时需注意

	// ━━ 最佳实践总结 ━━
	UE_LOG(LogStringTypes, Warning, TEXT(""));
	UE_LOG(LogStringTypes, Warning, TEXT("┌──────────────┬────────────────────────────────────────┐"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ 场景         │ 用哪个类型                              │"));
	UE_LOG(LogStringTypes, Warning, TEXT("├──────────────┼────────────────────────────────────────┤"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ 骨骼/Socket名│ FName (编辑器选择，高效比较)            │"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ GameplayTag  │ FName (内部基于 FName)                  │"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ 文件路径     │ FString (需要拼接、查找、替换)          │"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ 日志输出     │ FString (格式化方便)                    │"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ UI 显示文字  │ FText (支持本地化)                      │"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ 对话台词     │ FText (必须本地化)                      │"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ 编辑器标签   │ FText (Detail 面板显示)                 │"));
	UE_LOG(LogStringTypes, Warning, TEXT("│ 数值显示     │ FText::AsNumber (自动按语言格式化)      │"));
	UE_LOG(LogStringTypes, Warning, TEXT("└──────────────┴────────────────────────────────────────┘"));
}

#undef LOCTEXT_NAMESPACE
