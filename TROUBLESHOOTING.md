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
| 4 | C2338 %s expects character pointer but got character | 1.1 | 2026-06-26 |
| 5 | C2440 const void* 转 void* 丢失限定符 | 1.1 | 2026-06-26 |

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

## #4 C2338: '%s' expects character pointer but got character

- **日期**：2026-06-26
- **模块**：1.1 UObject & UFUNCTION / UPROPERTY 宏
- **报错原文**：
  ```
  ReflectionDataObject.cpp(95,2): error C2338: static_assert failed:
  ''%s' expects character pointer but got character. (Is there an extra '*' somewhere?)'
  ```

### 原因分析

`GetClass()->GetPrefixCPP()` 返回的是 `FString`（临时值），直接在 `UE_LOG` 中写 `*Class->GetPrefixCPP()` 时，编译器在格式化参数求值顺序上可能将返回值视为 `char` 而非字符串指针。UE5.7 的格式化字符串安全检查（`FormatStringSan`）会做编译期类型校验，检测到 `%s` 期望指针但收到字符。

### 解决方式

将返回值先存到局部变量，再取 `*`：
```cpp
// ❌ 错误 —— 临时值直接取 *，触发格式化安全检查
UE_LOG(LogTemp, Log, TEXT("前缀: %s"), *Class->GetPrefixCPP());

// ✅ 正确 —— 先存局部变量再取 *
FString PrefixCPP = Class->GetPrefixCPP();
UE_LOG(LogTemp, Log, TEXT("前缀: %s"), *PrefixCPP);
```

### 踩坑要点

- UE5.7 的 `FormatStringSan` 是编译期检查，比旧版更严格
- 任何返回临时 `FString` 的函数（如 `GetPrefixCPP()`、`GetPathName()` 等），在 `UE_LOG` 中都应先存局部变量
- `*FString` 解引用得到 `const TCHAR*`，但如果 `FString` 是临时值，生命周期可能在参数求值前结束

---

## #5 C2440: const void* 转 void* 丢失限定符

- **日期**：2026-06-26
- **模块**：1.1 UObject & UFUNCTION / UPROPERTY 宏
- **报错原文**：
  ```
  ReflectionDemoActor.cpp(147,17): error C2440: "初始化": 无法从"const ValueType *"
  转换为"void *"
  note: 转换丢失限定符
  ```

### 原因分析

`PrintPropertyValue` 函数参数声明为 `const void* ContainerPtr`，而 `FProperty::ContainerPtrToValuePtr<void>()` 的 `const` 版本返回 `const void*`，但 `GetPropertyValue()` 等函数需要 `void*`（非 const）。C++ 不允许 `const void*` 隐式转换为 `void*`。

### 解决方式

将函数参数从 `const void*` 改为 `void*`：
```cpp
// ❌ 错误 —— const void* 导致后续所有反射 API 都收到 const 指针
void PrintPropertyValue(FProperty* Property, const void* ContainerPtr);

// ✅ 正确 —— 反射 API 需要 void* 来读写值
void PrintPropertyValue(FProperty* Property, void* ContainerPtr);
```

### 踩坑要点

- UE5 反射 API（`ContainerPtrToValuePtr`、`GetPropertyValue`、`SetPropertyValue`）的参数都是 `void*`（非 const）
- 如果外部调用者有 `const UObject*`，需要先 `const_cast` 或在设计上避免 const
- `PrintAllProperties(UObject* Object)` 传入 `Object` 时，`UObject*` → `void*` 隐式转换是合法的

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
