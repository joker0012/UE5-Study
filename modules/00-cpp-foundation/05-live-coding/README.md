# 0.5 Live Coding / Hot Reload 工作流

## 学习目标

理解 Live Coding 与 Hot Reload 的区别，掌握日常开发中「改代码不重启编辑器」的工作流。

## 核心概念

### Live Coding vs Hot Reload

| | Live Coding | Hot Reload (Build → Reload) |
|---|---|---|
| **原理** | 编译改动文件 → 注入到运行中的进程 | 完整重编译模块 → 卸载旧 DLL → 加载新 DLL |
| **速度** | 极快（秒级，只编译改动的 .cpp） | 较慢（重编整个模块） |
| **限制** | 只能改函数体，**不能加新类/新 UPROPERTY/新 UFUNCTION** | 可以加新类/新反射属性，但会丢失运行时状态 |
| **运行时状态** | **保留**（变量值、Actor 实例不受影响） | **重置**（模块重新加载） |
| **触发方式** | Rider: Ctrl+Alt+F10 / UE 编辑器: 🔧按钮 | Rider: Build → UE 自动提示 Reload |
| **适用场景** | 日常改逻辑、调参数、改算法 | 加新类、加新 UPROPERTY/UFUNCTION |

### Live Coding 能做什么 / 不能做什么

| 操作 | Live Coding 支持？ |
|------|:---:|
| 修改函数体逻辑 | ✅ |
| 修改 UPROPERTY 默认值 | ✅ |
| 修改 UE_LOG 文本 | ✅ |
| 修改局部变量 | ✅ |
| 新增 UPROPERTY / UFUNCTION | ❌ 需要完整重编 |
| 新增 UCLASS | ❌ |
| 修改 USTRUCT 布局 | ❌ |
| 修改头文件中的函数签名 | ❌ |
| 修改 Build.cs | ❌ |

### 正确的工作流

```
日常改逻辑 → Live Coding（秒级，不丢状态）
     ↓ 不够了（加新属性/新类）
完整重编 → Hot Reload（较慢，丢状态但能加反射）
     ↓ 也不够了（改了 Build.cs）
清理 Intermediate/Build → 全量重编 → 重启编辑器
```

## 验证步骤

### 步骤 1：放置 Actor
1. 编译通过后启动编辑器
2. 拖 `LiveCodingDemoActor` 到关卡
3. 按 Play → 看 Output Log 的 BeginPlay 输出
4. Actor 会持续旋转（Tick 中用 RotationSpeed）

### 步骤 2：Live Coding 验证
1. **不要关闭编辑器和 Play**
2. 在 Rider 中打开 `LiveCodingDemoActor.cpp`
3. 把 `CalculateSomething` 中的 `return Input * 2;` 改成 `return Input * Input;`
4. 按 `Ctrl+Alt+F10`（Live Coding 热重载）
5. 在编辑器 Output Log 应该看到 Live Coding 成功消息
6. 选中关卡中的 Actor → 蓝图调用 `PrintCurrentState` 或 `CalculateSomething`
7. 结果从 84 变成 1764 → **热重载成功！**

### 步骤 3：验证状态保留
1. 观察 Tick 计数器日志 —— Live Coding 后计数**不重置**
2. 对比 Hot Reload：如果用 Build → Reload，计数器会归零

## 关键文件

| 文件 | 作用 |
|------|------|
| `LiveCodingDemoActor.h` | Actor 头文件 — 可调参数 + 验证函数 |
| `LiveCodingDemoActor.cpp` | Actor 实现 — Tick 旋转 + 计数器 + 计算函数 |

## 踩坑要点

- **改了头文件（加 UPROPERTY/UFUNCTION）后 Live Coding 不生效** → 这是正常的，UHT 需要重新生成反射代码，必须完整重编
- **Live Coding 后蓝图节点可能不同步** → 右键蓝图 Refresh Nodes 或重启编辑器
- **Rider 的 Live Coding 需要在设置中启用** → Settings → Build → Enable Live Coding
- **改了 Build.cs 一定不要用 Live Coding** → 清理中间文件全量重编
