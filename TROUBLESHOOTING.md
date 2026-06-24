# TROUBLESHOOTING.md — 编译报错记录

> 记录学习过程中遇到的编译报错、原因分析和最终解决方式。
> AI 助手在遇到新报错时，应先查本文档确认是否已有解决方案。
> 解决新报错后，必须追加记录到本文档。

---

## 目录

| # | 报错关键词 | 模块 | 日期 |
|---|-----------|------|------|
| 1 | C4668 未定义预处理器宏 | 0.1 | 2026-06-23 |
| 2 | LNK2019 无法解析的外部符号 FGameplayTag | 0.1 | 2026-06-23 |
| 3 | Build.cs 加了依赖但链接器没识别 | 0.1 | 2026-06-23 |

---

## #1 C4668: 未定义预处理器宏

- **日期**：2026-06-23
- **模块**：0.1 Build.cs / Target.cs
- **报错原文**：
  ```
  BuildConfigTestActor.cpp(49,5): Error C4668 : 没有将"STUDY_PROJECT_VERSION"定义为预处理器宏，用"0"替换"#if/#elif"
  #if STUDY_PROJECT_VERSION
      ^
  ```

### 原因分析

UE5 默认开启 `/we4668`（C4668 警告提升为错误）。`#if MACRO` 在宏未定义时，编译器会先用 0 替换再求值，触发 C4668。

### 解决方式

将 `#if` 改为 `#ifdef`：
```cpp
// ❌ 错误写法 —— 宏未定义时触发 C4668
#if STUDY_PROJECT_VERSION
    ...
#endif

// ✅ 正确写法 —— 只检查是否定义，不会触发警告
#ifdef STUDY_PROJECT_VERSION
    ...
#endif
```

### 踩坑要点

- `#if` 适合宏一定被定义的场景（值为 0 或 1）
- `#ifdef` 适合宏可能未定义的场景
- UE5 的 `/we4668` 编译选项会把所有 C4668 警告当错误，必须用 `#ifdef` 或 `#if defined()`

---

## #2 LNK2019: 无法解析的外部符号 FGameplayTag

- **日期**：2026-06-23
- **模块**：0.1 Build.cs / Target.cs
- **报错原文**：
  ```
  Module.StudyProject.gen.cpp.obj : error LNK2019: 无法解析的外部符号
  "__declspec(dllimport) class UScriptStruct * __cdecl Z_Construct_UScriptStruct_FGameplayTag(void)"
  ```

### 原因分析

`BuildConfigTestActor.h` 中使用了 `FGameplayTag` 类型（`UPROPERTY` 反射），但项目编译时找不到 `GameplayTags` 模块的导入库。

检查链路：
1. Build.cs 中已有 `"GameplayTags"` 公开依赖 ✅
2. 引擎 `UnrealEditor-GameplayTags.dll` 和 `.lib` 都存在 ✅
3. 但 UBT 生成的 `Definitions.StudyProject.h` 中缺少 `GAMEPLAYTAGS_API` 宏 ❌
4. 链接器响应文件 `.dll.rsp` 中没有 `GameplayTags.lib` ❌

**根因**：修改 Build.cs 添加新依赖后，UBT 增量编译缓存未失效，仍然使用旧的依赖列表进行链接。

### 解决方式

清理 `Intermediate\Build` 目录后全量重编译：

```powershell
# 1. 删除编译中间文件
Remove-Item -Path "H:\UE_Project\Study\StudyProject\Intermediate\Build" -Recurse -Force

# 2. 重新生成项目文件
& "H:\UE_Engine\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" `
    -projectfiles -project="H:\UE_Project\Study\StudyProject\StudyProject.uproject" -game -engine

# 3. 全量编译
& "H:\UE_Engine\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" `
    StudyProjectEditor Win64 Development `
    -project="H:\UE_Project\Study\StudyProject\StudyProject.uproject" -waitmutex
```

### 踩坑要点

- **修改 Build.cs 后，增量编译可能不生效**——UBT 的依赖缓存不会自动失效
- 如果改了 Build.cs 加了新模块依赖后遇到 LNK2019，直接清 `Intermediate\Build` 全量重编
- 在 Rider 中对应操作：Build → Clean Solution → Rebuild Solution
- UBT 命令行加 `-force` 参数也可强制全量编译，但清理 Build 目录最彻底

---

## #3 Build.cs 加了依赖但链接器没识别

- **日期**：2026-06-23
- **模块**：0.1 Build.cs / Target.cs
- **现象**：Build.cs 中写了 `PublicDependencyModuleNames.Add("GameplayTags")`，但 UBT 生成的链接器响应文件中没有 `GameplayTags.lib`

### 排查方法

当怀疑依赖没生效时，按以下步骤排查：

1. **检查 Definitions 文件**（确认 UBT 是否识别了依赖）：
   ```
   StudyProject\Intermediate\Build\Win64\x64\UnrealEditor\Development\StudyProject\Definitions.StudyProject.h
   ```
   如果依赖生效，文件中应出现 `XXXPAYLOADTAGS_API DLLIMPORT` 这样的宏定义

2. **检查链接器响应文件**（确认是否传入链接器）：
   ```
   StudyProject\Intermediate\Build\Win64\x64\UnrealEditor\Development\StudyProject\UnrealEditor-StudyProject.dll.rsp
   ```
   搜索依赖模块的 `.lib` 文件名

3. **如果以上文件缺少依赖**，说明 UBT 缓存了旧的 Build.cs 解析结果 → 清理 `Intermediate\Build` 全量重编

### 解决方式

同 #2 —— 清理 `Intermediate\Build` 目录后重新生成项目文件 + 全量编译。

### 踩坑要点

- 修改 Build.cs 是高风险操作，UBT 的增量机制对 Build.cs 变更的检测不可靠
- **经验法则**：凡是改了 Build.cs（加/删依赖、改 PCH 策略、加宏定义），一律清理中间文件全量重编

---

## 报错记录模板

> 后续遇到新报错时，按此模板追加到上方。

```markdown
## #N 报错关键词

- **日期**：YYYY-MM-DD
- **模块**：X.Y 知识点名
- **报错原文**：
  ```
  （粘贴编译器/链接器原始输出）
  ```

### 原因分析

（分析报错根因，包括检查了哪些文件、排除了哪些可能）

### 解决方式

（最终生效的解决步骤，含命令/代码）

### 踩坑要点

（预防建议、相关知识点、容易混淆的地方）
```
