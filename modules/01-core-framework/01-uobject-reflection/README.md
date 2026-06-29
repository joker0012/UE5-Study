# 1.1 UObject & UFUNCTION / UPROPERTY 宏

## 学习目标

理解 UE5 反射系统的三大核心宏，以及运行时如何通过反射查询对象的类信息、属性列表、函数列表。

- **UCLASS** —— 声明一个可被反射的 UObject 派生类
- **UPROPERTY** —— 声明一个可被反射的属性（编辑器可见 / 蓝图可读写 / 序列化）
- **UFUNCTION** —— 声明一个可被反射的函数（蓝图可调用 / 可重写）

## 核心概念

### 反射是什么

C++ 原生没有反射。UE 通过 **UHT（UnrealHeaderTool）** 在编译前扫描 UCLASS/UPROPERTY/UFUNCTION 宏，生成 `.generated.h` 和 `.gen.cpp` 文件，在运行时构建反射元数据（`UClass`、`FProperty`、`UFunction`）。

反射是以下系统的基石：
- **蓝图交互** — 蓝图能调 C++ 函数、读写 C++ 属性，靠的就是反射元数据
- **编辑器编辑** — Details 面板能显示和编辑属性，靠的是 UPROPERTY 标志
- **序列化** — 保存/加载对象状态，靠的是反射遍历属性
- **网络复制** — 服务器同步属性到客户端，靠的是反射标记
- **GC 垃圾回收** — 追踪 UObject 指针引用，靠的是 UPROPERTY 注册

### UPROPERTY 访问控制矩阵

| Specifier | 编辑器 | 蓝图 | 适用场景 |
|-----------|--------|------|----------|
| `EditAnywhere` | 任意实例可编辑 | — | 运行时可变的参数 |
| `EditDefaultsOnly` | 仅 CDO 可编辑 | — | 所有实例共享的默认值 |
| `VisibleAnywhere` | 任意实例只读 | — | 运行时计算的只读状态 |
| `BlueprintReadWrite` | — | 可读可写 | 蓝图需要修改的属性 |
| `BlueprintReadOnly` | — | 只读 | 蓝图只需查询的属性 |

### UPROPERTY meta 常用项

| meta | 作用 |
|------|------|
| `ClampMin` / `ClampMax` | 编辑器中限制数值范围 |
| `EditCondition` | 条件编辑（某 bool 为 true 时才可编辑） |
| `AllowPrivateAccess` | 让 private 成员也能被蓝图/编辑器访问 |
| `ToolTip` | 编辑器中显示的提示文字 |

### UFUNCTION 类型

| Specifier | 蓝图行为 | 说明 |
|-----------|----------|------|
| `BlueprintCallable` | 有执行引脚，可调用 | 适合有副作用的操作 |
| `BlueprintPure` | 无执行引脚，纯查询 | 适合无副作用的 getter |
| `BlueprintNativeEvent` | 可重写 | C++ 提供 `_Implementation` 默认实现 |
| `BlueprintImplementableEvent` | 必须实现 | C++ 不提供实现，蓝图实现（不实现则 no-op） |

### BlueprintNativeEvent vs BlueprintImplementableEvent

```
BlueprintNativeEvent:
  .h  →  UFUNCTION(BlueprintNativeEvent) void MyFunc();
  .cpp →  void MyClass::MyFunc_Implementation() { /* C++ 默认逻辑 */ }
  蓝图可重写 → 执行蓝图版本；不重写 → 执行 C++ 版本

BlueprintImplementableEvent:
  .h  →  UFUNCTION(BlueprintImplementableEvent) void MyEvent();
  .cpp →  不写实现（UHT 生成 thunk）
  蓝图实现 → 执行蓝图版本；不实现 → no-op（不崩溃）
```

### 反射 API 速查

| API | 作用 |
|-----|------|
| `GetClass()->GetName()` | 获取类名（不含前缀） |
| `GetClass()->GetPathName()` | 获取完整路径 |
| `GetClass()->FindPropertyByName(TEXT("X"))` | 按名称查找属性 |
| `TFieldIterator<FProperty>` | 遍历类所有属性 |
| `TFieldIterator<UFunction>` | 遍历类所有函数 |
| `Property->ContainerPtrToValuePtr<void>(Obj)` | 获取属性值的内存地址 |
| `CastField<FIntProperty>(Property)` | 属性类型转换 |
| `IntProp->GetPropertyValue(Ptr)` / `SetPropertyValue(Ptr, Val)` | 反射读写值 |

## 文件说明

| 文件 | 说明 |
|------|------|
| `ReflectionDataObject.h/.cpp` | UObject 数据类 — 演示各种 UPROPERTY/UFUNCTION |
| `ReflectionDemoActor.h/.cpp` | Actor — 创建对象、反射遍历属性、调用函数 |

## 验证步骤

1. 编译项目（已通过 UBT 验证）
2. 在编辑器中将 `ReflectionDemoActor` 拖入关卡
3. 按 Play，查看 Output Log 中 `LogReflectionDemo` 的输出
4. 选中 DataObject 属性，在 Details 面板观察各种 UPROPERTY 的编辑器表现

## 预期 Log 输出（节选）

```
LogReflectionDemo: ===== 知识点 1.1 — UObject & UFUNCTION / UPROPERTY =====
LogReflectionDemo: --- 1. GetClass()->GetName() ---
LogReflectionDemo: Actor 类名: ReflectionDemoActor
LogReflectionDemo: --- 2. NewObject<T>() 创建实例 ---
LogReflectionDemo: 创建对象: ReflectionDataObject
LogReflectionDemo: 对象类名: ReflectionDataObject
LogReflectionDemo: --- 3. 数据对象反射信息 ---
LogReflection: ===== 类反射信息 =====
LogReflection: 类名 (GetName):         ReflectionDataObject
LogReflection: 属性数量: 11
LogReflection: 函数数量: 7
LogReflectionDemo: --- 4. TFieldIterator 遍历属性 ---
LogReflectionDemo:   [0] Health (int32)
LogReflectionDemo:       标志: Edit BPVisible
LogReflectionDemo:       值: 100
LogReflectionDemo:   [1] MaxHealth (int32)
LogReflectionDemo:       标志: Edit BPVisible
LogReflectionDemo:       值: 100
...
LogReflectionDemo: --- 7. FindPropertyByName 反射读写 ---
LogReflectionDemo: 找到属性 'Health' → 类型: int32, 当前值: 45
LogReflectionDemo: 通过反射修改 → Health = 999
LogReflectionDemo: 验证: DataObject->Health = 999
LogReflectionDemo: ===== 反射演示完成 =====
```

## 实验建议

### 实验 1：UPROPERTY 访问控制
- 在编辑器中选中 DataObject，观察哪些属性可编辑、哪些只读
- 修改 `Health` 为 `EditDefaultsOnly`，重新编译，观察区别

### 实验 2：BlueprintNativeEvent
- 创建一个基于 `ReflectionDataObject` 的蓝图子类
- 重写 `InitializeDefaults` 事件，添加 PrintString
- 对比 C++ 默认实现和蓝图重写的执行

### 实验 3：EditCondition
- 在编辑器中切换 `bIsInvincible` 的勾选
- 观察 `InvincibleDuration` 是否随之可编辑/禁用
