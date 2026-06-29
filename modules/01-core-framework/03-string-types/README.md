# 1.3 FName / FString / FText 区别与转换

## 学习目标

理解 UE5 三大字符串类型的本质区别、适用场景和互转方法。

| 类型 | 本质 | 比较 | 可变 | 本地化 | 适用场景 |
|------|------|------|------|--------|----------|
| **FName** | 全局名字表中的 ID | O(1) ID 比较 | 不可变 | 不支持 | 标识符：骨骼名、Socket名、Tag名 |
| **FString** | 可变 TCHAR 数组 | O(n) 逐字符 | 可变 | 不支持 | 通用字符串：拼接、格式化、路径操作 |
| **FText** | 本地化文本（Namespace + Key） | 按身份比较 | 不可变 | 支持 | 显示文本：UI、对话、提示 |

## 核心概念

### FName —— 轻量标识符

- 内部只存一个整数 ID，相同字符串在全局 Name Table 中只存一份
- 比较 O(1)：比的是 ID，不是字符串内容
- **大小写不敏感**：`FName("Hello") == FName("HELLO")` 为 true
- 不可变：只能重新赋值，不能修改内容

### FString —— 通用字符串

- 类似 `std::string`，支持拼接、查找、替换、格式化
- `FString::Printf` — printf 风格格式化
- `FString::Format` — 参数化格式（`{0} {1}` 占位符）
- `FCString::Atoi / Atof` — 字符串转数值
- `FString::FromInt` — 数值转字符串
- `ParseIntoArray` — 按分隔符分割成数组

### FText —— 本地化文本

- 不可变，不能用 `+` 拼接，只能用 `FText::Format`
- 创建方式：
  - `INVTEXT("...")` — 不变文本，不参与本地化
  - `NSLOCTEXT(Namespace, Key, Source)` — 带命名空间的本地化文本
  - `LOCTEXT(Key, Source)` — 使用文件级 `#define LOCTEXT_NAMESPACE`
  - `FText::FromString(Str)` — 从 FString 创建（不参与本地化）
  - `FText::AsNumber(123)` / `AsPercent(0.5f)` — 数字格式化（按语言环境）
- `==` 比较的是本地化身份（Namespace + Key），**不是**字符串内容

### 转换关系

```
FName  ──→ FString ──→ FText     (安全/无损)
FText  ──→ FString ──→ FName     (有损！丢失本地化信息)
```

## 性能对比

| 指标 | FName | FString | FText |
|------|-------|---------|-------|
| 比较速度 | O(1) | O(n) | O(1)（身份比较） |
| sizeof | ~8 字节 | ~16 字节 + 堆 | ~48 字节 |
| 创建开销 | Name Table 查找 | 堆分配 | 构造本地化信息 |

## 常见坑

1. **UE_LOG 中 `%s`**：FName 要 `*Name.ToString()`，FString 直接 `*Str`，FText 要 `*Text.ToString()`
2. **FText 不能用 `+`**：只能用 `FText::Format(INVTEXT("{0}{1}"), A, B)`
3. **FName 大小写不敏感**：需要大小写敏感比较时转 FString
4. **FText::FromString 不参与本地化**：正式项目用 NSLOCTEXT/LOCTEXT
5. **FText `==` 比较身份不是内容**：两个不同 Key 的 FText 即使文字相同也不相等

## 文件说明

| 文件 | 说明 |
|------|------|
| `StringTypesDemoActor.h/.cpp` | 演示三种字符串类型的创建、操作、互转、性能对比 |

## 验证步骤

1. 关闭编辑器后用 UBT 编译（或编辑器内 Ctrl+Alt+F11 热重载）
2. 拖 `StringTypesDemoActor` 到关卡
3. Play → 查看 Output Log 中 `LogStringTypes` 的输出
4. 在 Details 面板观察三种类型的编辑器表现差异

## 预期 Log 输出（节选）

```
LogStringTypes: ===== 知识点 1.3 — FName / FString / FText =====
LogStringTypes: --- 1. FName —— 轻量标识符 ---
LogStringTypes: Name1 == Name2: true
LogStringTypes: 'Test' == 'test': true (FName 大小写不敏感)
LogStringTypes: --- 2. FString —— 通用字符串 ---
LogStringTypes: 拼接: Hello + World = "Hello World"
LogStringTypes: ParseIntoArray(","): 3 个部分
LogStringTypes: --- 3. FText —— 本地化文本 ---
LogStringTypes: Format: 勇者 升到了 25 级！
LogStringTypes: 不同Key相同文字 ==: false (FText比较的是身份不是内容)
LogStringTypes: --- 5. 性能对比 ---
LogStringTypes: 100000 次比较:
LogStringTypes:   FName 快 5.2x
LogStringTypes: --- 6. 常见坑 & 最佳实践 ---
LogStringTypes: FText 拼接用 Format: 你好 世界
```

## 实验建议

### 实验 1：编辑器表现差异
- 在 Details 面板观察 FName 属性 — 显示为下拉框（有历史记录）
- 观察 FString 属性 — 普通文本框
- 观察 FText 属性 — 本地化文本编辑器（可多语言）

### 实验 2：切换语言环境
- 在控制台输入 `culture zh-Hans` / `culture en` 切换语言
- 观察 `FText::AsNumber(1234567)` 的输出变化（中文：1,234,567 / 英文：1,234,567）

### 实验 3：性能测试
- 修改 `Iterations` 为 100 万次，对比 FName 和 FString 的比较耗时差距
