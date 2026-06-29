// DebugMacrosDemoActor.cpp
// ============================================================
// 知识点 1.2 — UE_LOG / check / ensure 调试宏体系
// ============================================================

#include "DebugMacrosDemoActor.h"
// LoggingLogCategories.h 提供 DECLARE_LOG_CATEGORY_EXTERN
#include "Logging/LogMacros.h"
#include "Logging/StructuredLog.h"

// ━━━━━━━━━━━━━━━━ 自定义 Log Category ━━━━━━━━━━━━━━━━
// 用法：DECLARE 在 .h 或 .cpp 顶部声明，DEFINE 在 .cpp 中定义
// 好处：可以在 Output Log 的 Category 列表里单独过滤/开关
DECLARE_LOG_CATEGORY_CLASS(LogStudyDebug, Log, All);
// 参数：Category名, 默认Verbosity, 编译时最大Verbosity

DEFINE_LOG_CATEGORY(LogStudyDebug);
// 注意：DECLARE_LOG_CATEGORY_CLASS + DEFINE_LOG_CATEGORY 配对使用（类内声明）
//       DECLARE_LOG_CATEGORY_EXTERN + DEFINE_LOG_CATEGORY  配对使用（全局声明）


// ━━━━━━━━━━━━━━━━ 构造 & 生命周期 ━━━━━━━━━━━━━━━━

ADebugMacrosDemoActor::ADebugMacrosDemoActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADebugMacrosDemoActor::BeginPlay()
{
	Super::BeginPlay();

	RunAllDemos();
}

// ━━━━━━━━━━━━━━━━ 主演示函数 ━━━━━━━━━━━━━━━━

void ADebugMacrosDemoActor::RunAllDemos()
{
	UE_LOG(LogStudyDebug, Warning, TEXT("===== 知识点 1.2 — UE_LOG / check / ensure 调试宏 ====="));

	Demo_UELog();
	Demo_LogCategory();
	Demo_Check();
	Demo_Ensure();
	Demo_Verify();
	Demo_OtherMacros();

	UE_LOG(LogStudyDebug, Warning, TEXT("===== 调试宏演示完成 ====="));
}

// ━━━━━━━━━━━━━━━━ 1. UE_LOG 基础 ━━━━━━━━━━━━━━━━

void ADebugMacrosDemoActor::Demo_UELog()
{
	UE_LOG(LogStudyDebug, Warning, TEXT("--- 1. UE_LOG 基础用法 ---"));

	// ━━ 日志级别（Verbosity）━━
	// Fatal   —— 严重错误，会终止程序（Shipping 也会执行）
	// Error   —— 错误，红色显示，不会被 Suppress
	// Warning —— 警告，黄色显示
	// Log     —— 普通信息，灰色
	// Display —— 显示信息，会在屏幕上短暂显示（~4秒）
	// Verbose —— 详细信息，默认不显示，需手动开启
	// VeryVerbose —— 极其详细，通常只给开发者看

	UE_LOG(LogStudyDebug, Fatal, TEXT("")) // 不演示 Fatal，会崩
	// ↑ 这行注释掉了，实际不会执行
	// 真实代码中 Fatal 会弹窗 + 写 crash dump + 终止

	UE_LOG(LogStudyDebug, Error, TEXT("这是 Error —— 红色，表示错误"));
	UE_LOG(LogStudyDebug, Warning, TEXT("这是 Warning —— 黄色，表示警告"));
	UE_LOG(LogStudyDebug, Log, TEXT("这是 Log —— 灰色，普通信息"));
	UE_LOG(LogStudyDebug, Display, TEXT("这是 Display —— 会在屏幕上短暂显示"));

	// ━━ 格式化字符串 ━━
	// UE_LOG 使用类似 printf 的格式化
	FString Name = TEXT("勇者");
	int32 Level = 25;
	float Hp = 850.5f;

	// %s —— 字符串（FString 需要 * 解引用）
	UE_LOG(LogStudyDebug, Log, TEXT("角色: %s"), *Name);

	// %d —— 整数
	UE_LOG(LogStudyDebug, Log, TEXT("等级: %d"), Level);

	// %.2f —— 浮点数（保留2位小数）
	UE_LOG(LogStudyDebug, Log, TEXT("血量: %.2f"), Hp);

	// 多参数混合
	UE_LOG(LogStudyDebug, Warning, TEXT("%s (Lv.%d) HP=%.1f"), *Name, Level, Hp);

	// ━━ 常见坑：%s 的类型 ━━
	// %s 对应 const TCHAR*
	// FString → 需要加 *（如 *Name）
	// FName   → 需要 *Name.ToString()
	// FText   → 需要 *Text.ToString()
	// int32/float → 绝不能用 %s，用 %d/%f

	// ━━ LogTemp —— 不需要声明的临时 Category ━━
	// LogTemp 是引擎内置的，适合快速调试，不用声明 Category
	UE_LOG(LogTemp, Warning, TEXT("LogTemp 不需要声明，适合临时调试"));

	// ━━ UE5 结构化日志（Structured Log）━━
	// UE5.0+ 新增，支持键值对格式，便于机器解析和 Insights 筛选
	// 格式：UE_LOGFMT(Category, Verbosity, "文本 {key} {key2}", arg1, arg2)
	// 注意：UE_LOGFMT 的参数不需要 * 解引用
	UE_LOGFMT(LogStudyDebug, Log, "结构化日志: 角色 {CharName} 等级 {Lvl}", Name, Level);

	// ━━ 屏幕消息 —— 不进日志文件，只在游戏画面上显示 ━━
	// GEngine->AddOnScreenDebugMessage 适合临时可视化调试
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,                    // Key: -1 表示总是添加新消息
			3.0f,                  // 显示时长（秒）
			FColor::Green,         // 颜色
			TEXT("屏幕消息：3秒后消失")  // 内容
		);
	}
}

// ━━━━━━━━━━━━━━━━ 2. 自定义 Log Category ━━━━━━━━━━━━━━━━

void ADebugMacrosDemoActor::Demo_LogCategory()
{
	UE_LOG(LogStudyDebug, Warning, TEXT("--- 2. 自定义 Log Category ---"));

	// 为什么用自定义 Category 而不是 LogTemp？
	// 1. 可以在 Output Log 面板的 Category 列表中单独过滤
	// 2. 可以通过 ini 文件控制每个 Category 的日志级别
	// 3. 发布时可以一键关闭某个 Category 的所有日志

	// 声明方式（本文件顶部）：
	//   DECLARE_LOG_CATEGORY_CLASS(LogStudyDebug, Log, All);
	//   DEFINE_LOG_CATEGORY(LogStudyDebug);

	// 也可以用全局声明（放在 .h 中）：
	//   DECLARE_LOG_CATEGORY_EXTERN(LogMyGame, Log, All);
	// 然后在 .cpp 中：
	//   DEFINE_LOG_CATEGORY(LogMyGame);

	// 在 ini 中控制日志级别：
	//   [Core.Log]
	//   LogStudyDebug=Warning  ← 只显示 Warning 及以上

	UE_LOG(LogStudyDebug, Log, TEXT("这条日志属于 LogStudyDebug Category"));
	UE_LOG(LogStudyDebug, Warning, TEXT("可以在 Output Log 中按 Category 过滤这条日志"));

	// 引擎内置常用 Category
	UE_LOG(LogTemp, Log, TEXT("LogTemp —— 引擎内置临时 Category"));
	UE_LOG(LogCore, Log, TEXT("LogCore —— 引擎核心 Category"));
}

// ━━━━━━━━━━━━━━━━ 3. check 宏家族 ━━━━━━━━━━━━━━━━

void ADebugMacrosDemoActor::Demo_Check()
{
	UE_LOG(LogStudyDebug, Warning, TEXT("--- 3. check 宏家族 ---"));

	// ━━ check(expr) —— 断言，失败就崩 ━━
	// 编译目标：Development / Debug / Editor 生效
	//          Shipping 自动移除（零开销）
	// 用途：验证「不可能为假」的条件 —— 程序逻辑错误才触发

	// check(PlayerLevel > 0);  // 如果 PlayerLevel <= 0 就崩溃
	// 我们用条件包裹来安全演示：
	if (PlayerLevel > 0)
	{
		check(PlayerLevel > 0);  // 通过
		UE_LOG(LogStudyDebug, Log, TEXT("check(PlayerLevel > 0) —— 通过"));
	}

	// ━━ checkf(expr, fmt, ...) —— 带格式化消息的 check ━━
	// 比 check 多一个自定义错误消息，崩溃时能看到具体信息
	checkf(PlayerLevel >= 1, TEXT("PlayerLevel 不应该 < 1，当前值: %d"), PlayerLevel);
	UE_LOG(LogStudyDebug, Log, TEXT("checkf(PlayerLevel >= 1) —— 通过"));

	// ━━ checkSlow(expr) —— 慢速检查，只在 Debug 生效 ━━
	// Development 也会移除（比 check 更早被优化掉）
	// 用途：放在性能敏感的代码路径中，只在 Debug 模式检查
	// checkSlow(TestObject != nullptr);

	// ━━ checkCode(expr) —— 编译期代码检查 ━━
	// 在编译时执行一次，通常用于验证编译期常量
	// checkCode(static_assert(sizeof(int32) == 4, "int32 should be 4 bytes"));

	// ━━ checkEntry / checkReturn —— 函数入口/出口检查 ━━
	// checkEntry() —— 放在函数开头，如果被跳过说明流程有问题
	// checkReturn() —— 放在函数 return 前

	// ━━ 触发 check 失败（可选演示）━━
	if (bTriggerCheckFail)
	{
		UE_LOG(LogStudyDebug, Error, TEXT("即将触发 check 失败 —— 程序会崩溃！"));
		// check(0);  // ← 取消注释会崩溃
		// 上面这行不执行，避免演示时意外崩溃
		// 如果你想体验：在编辑器 Details 面板勾选 bTriggerCheckFail，
		// 然后把下面一行的注释去掉：
		// checkf(TestObject != nullptr, TEXT("TestObject 不能为空！请在编辑器中设置"));
		UE_LOG(LogStudyDebug, Warning, TEXT("check 触发已跳过（取消注释可体验崩溃）"));
	}
}

// ━━━━━━━━━━━━━━━━ 4. ensure 宏家族 ━━━━━━━━━━━━━━━━

void ADebugMacrosDemoActor::Demo_Ensure()
{
	UE_LOG(LogStudyDebug, Warning, TEXT("--- 4. ensure 宏家族 ---"));

	// ━━ ensure(expr) —— 软断言 ━━
	// 和 check 的区别：
	//   check  —— 失败就崩（确保不可恢复的状态被立即发现）
	//   ensure —— 失败不崩，但触发断点（如果有调试器）+ 记录 callstack
	//             返回 bool，可以用来做分支处理
	//
	// 编译目标：Development / Debug / Editor 生效
	//          Shipping 自动移除（表达式本身也不执行）
	//
	// 用途：验证「可能失败但可恢复」的条件

	// ensure 返回 expr 的值，可以用来分支
	bool bIsValid = (PlayerLevel > 0);
	if (ensure(bIsValid))
	{
		UE_LOG(LogStudyDebug, Log, TEXT("ensure(PlayerLevel > 0) —— 通过，继续执行"));
	}
	else
	{
		UE_LOG(LogStudyDebug, Error, TEXT("ensure 失败但没崩溃 —— 可以走容错路径"));
	}

	// ━━ ensureMsgf(expr, fmt, ...) —— 带消息的 ensure ━━
	if (!bTriggerEnsureFail)
	{
		// 正常路径
		UE_LOG(LogStudyDebug, Log, TEXT("ensureMsgf —— 条件通过"));
	}
	else
	{
		// 故意触发 ensure 失败（不会崩溃，但会在 Output Log 中看到 ensure failed）
		bool bBadCondition = false;
		ensureMsgf(bBadCondition,
			TEXT("ensure 失败！PlayerLevel=%d, TestObject=%s"),
			PlayerLevel,
			TestObject ? *TestObject->GetName() : TEXT("nullptr"));

		// ensure 失败后代码继续执行 —— 这是它和 check 最大的区别
		UE_LOG(LogStudyDebug, Warning, TEXT("ensure 失败后继续执行 —— 没有崩溃"));

		// ━━ ensureAlways(expr) —— 每次都报告的 ensure ━━
		// 普通 ensure 同一个调用点只报告一次（去重）
		// ensureAlways 每次失败都报告 —— 适合循环中的检查
		ensureAlwaysMsgf(bBadCondition,
			TEXT("ensureAlways —— 每次失败都报告，不去重"));
		UE_LOG(LogStudyDebug, Warning, TEXT("ensureAlways 也不会崩溃"));
	}

	// ━━ ensure 的返回值用法 ━━
	// 典型模式：ensure + 提前返回
	// if (!ensure(TestObject)) return;  // 空指针时记录 callstack 但不崩，安全退出

	// ━━ check vs ensure 选择指南 ━━
	// ┌─────────────┬──────────────┬──────────────┐
	// │             │   check      │   ensure     │
	// ├─────────────┼──────────────┼──────────────┤
	// │ 失败行为    │ 崩溃终止     │ 断点+日志，继续运行 │
	// │ Shipping    │ 移除         │ 移除          │
	// │ 返回值      │ 无           │ 返回 bool     │
	// │ 适用场景    │ 不可恢复的bug│ 可能恢复的异常  │
	// │ 例子        │ 空指针不该出现│ 资源加载失败    │
	// └─────────────┴──────────────┴──────────────┘
}

// ━━━━━━━━━━━━━━━━ 5. verify 宏 ━━━━━━━━━━━━━━━━

void ADebugMacrosDemoActor::Demo_Verify()
{
	UE_LOG(LogStudyDebug, Warning, TEXT("--- 5. verify 宏 ---"));

	// ━━ verify(expr) —— 和 check 类似，但 Shipping 也执行 ━━
	// check 在 Shipping 被完全移除（表达式不执行）
	// verify 在 Shipping 中：断言被移除，但【表达式仍然执行】
	//
	// 用途：当断言的表达式本身有副作用时，用 verify 而非 check
	// 典型场景：验证函数返回值，但函数本身需要执行

	// 示例：验证一个有返回值的函数
	int32 Result = FMath::RandRange(1, 100);
	// verify(Result > 0);  // Development: 断言+执行；Shipping: 只执行（但 Result > 0 一定为真）
	UE_LOG(LogStudyDebug, Log, TEXT("verify 示例: Result = %d (>0 为真)"), Result);

	// verifyf —— 带消息的 verify
	verifyf(Result >= 1 && Result <= 100, TEXT("Result 越界: %d"), Result);
	UE_LOG(LogStudyDebug, Log, TEXT("verifyf(Result 在 1-100 之间) —— 通过"));

	// ━━ check vs verify 核心区别 ━━
	// check(false)  → Shipping 中：不崩，表达式被移除
	// verify(false) → Shipping 中：不崩，但表达式已执行
	//
	// 常见误用：
	//   check(SomeFunc());  // ← Shipping 中 SomeFunc() 不执行！
	//   verify(SomeFunc()); // ← Shipping 中 SomeFunc() 仍执行
}

// ━━━━━━━━━━━━━━━━ 6. 其他实用调试宏 ━━━━━━━━━━━━━━━━

void ADebugMacrosDemoActor::Demo_OtherMacros()
{
	UE_LOG(LogStudyDebug, Warning, TEXT("--- 6. 其他实用调试宏 ---"));

	// ━━ UE_CLOG —— 条件日志 ━━
	// UE_CLOG(条件, Category, Verbosity, Format, ...)
	// 只有条件为 true 时才输出日志，比 if + UE_LOG 更简洁
	bool bIsError = false;
	UE_CLOG(bIsError, LogStudyDebug, Error, TEXT("出错了！"));
	UE_CLOG(!bIsError, LogStudyDebug, Log, TEXT("没问题（条件日志：bIsError=false 时的分支）"));

	// ━━ TEXT() 宏 —— 创建 TCHAR 字符串 ━━
	// 所有 UE_LOG 的格式化字符串都应该用 TEXT() 包裹
	// 确保 Unicode 兼容（Windows 上 TCHAR = wchar_t）
	UE_LOG(LogStudyDebug, Log, TEXT("TEXT() 确保字符串是 TCHAR* 类型"));

	// ━━ FString::Printf —— 不输出日志，只格式化字符串 ━━
	FString Formatted = FString::Printf(TEXT("格式化: Level=%d, HP=%.1f"), PlayerLevel, 850.5f);
	UE_LOG(LogStudyDebug, Log, TEXT("%s"), *Formatted);

	// ━━ check vs ensure vs verify 总结表 ━━
	UE_LOG(LogStudyDebug, Warning, TEXT(""));
	UE_LOG(LogStudyDebug, Warning, TEXT("╔══════════════════════════════════════════════════╗"));
	UE_LOG(LogStudyDebug, Warning, TEXT("║          调试宏总结                               ║"));
	UE_LOG(LogStudyDebug, Warning, TEXT("║  check   — 失败崩溃, Shipping 移除, 无返回值      ║"));
	UE_LOG(LogStudyDebug, Warning, TEXT("║  ensure  — 失败断点, Shipping 移除, 返回 bool     ║"));
	UE_LOG(LogStudyDebug, Warning, TEXT("║  verify  — 失败崩溃, Shipping 保留表达式, 无返回值 ║"));
	UE_LOG(LogStudyDebug, Warning, TEXT("║  UE_LOG  — 日志输出, 所有配置都执行               ║"));
	UE_LOG(LogStudyDebug, Warning, TEXT("║  UE_CLOG — 条件日志, 满足条件才输出               ║"));
	UE_LOG(LogStudyDebug, Warning, TEXT("╚══════════════════════════════════════════════════╝"));
}
