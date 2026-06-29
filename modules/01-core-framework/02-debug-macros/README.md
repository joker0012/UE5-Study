# 1.2 UE_LOG / check / ensure 调试宏体系

## 学习目标

掌握 UE5 三大调试宏家族的用法、区别和适用场景：
1. **UE_LOG** — 日志输出（格式化、自定义 Category、结构化日志）
2. **check** — 断言（失败崩溃，Shipping 自动移除）
3. **ensure** — 软断言（失败不崩，断点+日志，返回 bool 可容错）

额外覆盖：verify、UE_CLOG、屏幕消息、checkSlow。

## 核心概念

### UE_LOG 日志级别

| Verbosity | 颜色 | 行为 | Shipping |
|-----------|------|------|----------|
| Fatal | — | 弹窗 + crash dump + **终止程序** | 执行 |
| Error | 红色 | 输出到日志 | 执行 |
| Warning | 黄色 | 输出到日志 | 执行 |
| Log | 灰色 | 输出到日志 | 执行 |
| Display | — | 输出到日志 + 屏幕短暂显示 | 执行 |
| Verbose | — | 默认不显示，需手动开启 | 不执行 |
| VeryVerbose | — | 极详细，通常只给开发者 | 不执行 |

### check vs ensure vs verify

| 宏 | 失败行为 | Shipping | 返回值 | 适用场景 |
|----|----------|----------|--------|----------|
| `check(expr)` | 崩溃终止 | 移除 | 无 | 不可恢复的 bug（空指针不该出现） |
| `checkf(expr, fmt, ...)` | 崩溃 + 自定义消息 | 移除 | 无 | 同上，需要更详细错误信息 |
| `checkSlow(expr)` | 崩溃终止 | 移除（Debug 才生效） | 无 | 性能敏感路径中的检查 |
| `ensure(expr)` | 断点 + 日志，**继续运行** | 移除 | `bool`（expr 的值） | 可恢复的异常（资源加载失败） |
| `ensureMsgf(expr, fmt, ...)` | 断点 + 日志 + 自定义消息 | 移除 | `bool` | 同上，需要详细信息 |
| `ensureAlways(expr)` | 每次都报告（不去重） | 移除 | `bool` | 循环中的检查 |
| `verify(expr)` | 崩溃终止 | 移除断言，**保留表达式** | 无 | 表达式有副作用时使用 |
| `verifyf(expr, fmt, ...)` | 崩溃 + 自定义消息 | 保留表达式 | 无 | 同上，带消息 |

### 自定义 Log Category

```cpp
// 方式 1：类内声明（DECLARE_LOG_CATEGORY_CLASS）
// .cpp 顶部
DECLARE_LOG_CATEGORY_CLASS(LogMyDebug, Log, All);
DEFINE_LOG_CATEGORY(LogMyDebug);

// 方式 2：全局声明（DECLARE_LOG_CATEGORY_EXTERN）
// .h 中
DECLARE_LOG_CATEGORY_EXTERN(LogMyGame, Log, All);
// .cpp 中
DEFINE_LOG_CATEGORY(LogMyGame);
```

好处：
- Output Log 面板可按 Category 单独过滤
- ini 文件可控制每个 Category 的级别：`[Core.Log] LogMyDebug=Warning`

### UE_LOG 格式化速查

| 格式符 | C++ 类型 | UE 类型 | 注意 |
|--------|----------|---------|------|
| `%s` | `const char*` | `FString` 需 `*` | FName 用 `*Name.ToString()` |
| `%d` | `int32` | `int32` | — |
| `%f` | `float/double` | `float/double` | `%.2f` 保留 2 位小数 |
| `%u` | `uint32` | `uint32` | — |
| `%lld` | `int64` | `int64` | — |
| `%x` | hex | — | 十六进制输出 |

### UE5 结构化日志 (UE_LOGFMT)

```cpp
// UE5.0+ 新增，参数不需要 * 解引用
UE_LOGFMT(LogMyDebug, Log, "角色 {Name} 等级 {Level}", CharName, Level);
// 便于 Unreal Insights 筛选和机器解析
```

## 文件说明

| 文件 | 说明 |
|------|------|
| `DebugMacrosDemoActor.h/.cpp` | 演示 UE_LOG / check / ensure / verify / UE_CLOG 等全部调试宏 |

## 验证步骤

1. 关闭编辑器后用 UBT 编译（或编辑器内 Ctrl+Alt+F11 热重载）
2. 在编辑器中将 `DebugMacrosDemoActor` 拖入关卡
3. 按 Play，查看 Output Log 中 `LogStudyDebug` 的输出
4. 在 Details 面板修改参数（如 `bTriggerEnsureFail`）观察不同行为

## 预期 Log 输出（节选）

```
LogStudyDebug: ===== 知识点 1.2 — UE_LOG / check / ensure 调试宏 =====
LogStudyDebug: --- 1. UE_LOG 基础用法 ---
LogStudyDebug: 这是 Error —— 红色，表示错误
LogStudyDebug: 这是 Warning —— 黄色，表示警告
LogStudyDebug: 这是 Log —— 灰色，普通信息
LogStudyDebug: 角色: 勇者
LogStudyDebug: 等级: 25
LogStudyDebug: 血量: 850.50
LogStudyDebug: --- 4. ensure 宏家族 ---
LogStudyDebug: ensure 失败后继续执行 —— 没有崩溃
LogStudyDebug: ensureAlways 也不会崩溃
LogStudyDebug: ╔══════════════════════════════════════════════════╗
LogStudyDebug: ║  check   — 失败崩溃, Shipping 移除, 无返回值      ║
LogStudyDebug: ║  ensure  — 失败断点, Shipping 移除, 返回 bool     ║
LogStudyDebug: ║  verify  — 失败崩溃, Shipping 保留表达式, 无返回值 ║
LogStudyDebug: ║  UE_LOG  — 日志输出, 所有配置都执行               ║
LogStudyDebug: ╚══════════════════════════════════════════════════╝
```

## 实验建议

### 实验 1：观察 ensure 行为
- 确保 `bTriggerEnsureFail = true`
- Play 后观察 Output Log —— 会有 `ensure failed` 消息但程序继续运行
- 如果用 Rider/VS 调试器附加，会触发断点

### 实验 2：体验 check 崩溃
- 将代码中 `checkf(TestObject != nullptr, ...)` 那行的注释去掉
- 确保 `bTriggerCheckFail = true` 且 `TestObject` 为空
- Play → 程序会崩溃，弹窗显示 callstack

### 实验 3：Log Category 过滤
- 在 Output Log 面板点击 Category 列
- 取消勾选 `LogStudyDebug` → 该 Category 的日志全部隐藏
- 在 `DefaultEngine.ini` 中添加 `[Core.Log] LogStudyDebug=Error` → 只显示 Error 级别
