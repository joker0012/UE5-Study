# 0.3 Module 划分 (Runtime / Editor / PrimaryGameModule)

## 学习目标

理解 UE5 模块的三种类型，把 StudyPlugin 从单模块拆分为 **Runtime + Editor** 双模块，验证依赖方向。

## 核心概念

### 模块类型（.uplugin 中 `Type` 字段）

| 类型 | 编译时机 | 用途 | 能依赖谁 |
|------|---------|------|---------|
| `Runtime` | 编辑器 + 打包都编译 | 游戏逻辑、Actor、Component | 只能依赖 Runtime 模块 |
| `Editor` | 仅编辑器编译，打包排除 | 编辑器工具、Detail 自定义、Asset Action | 可依赖 Runtime + Editor 模块 |
| `Developer` | 仅 Development/Staging 编译 | 开发工具（不上 Shipping） | 可依赖 Runtime + Developer |
| `Program` | 独立程序 | 不在引擎内运行的工具 | 视情况 |
| `ServerOnly` | 仅服务端 | 专用服务器逻辑 | Runtime + Server |
| `ClientOnly` | 仅客户端 | 客户端专属逻辑 | Runtime + Client |

### PrimaryGameModule（主游戏模块）

- `.uproject` 中 `Modules[0]` 注册的就是 PrimaryGameModule（如 `StudyProject`）
- 它是游戏入口，自动被 Target.cs 的 `ExtraModuleNames` 引用
- 其他游戏模块/插件模块都依赖它或与它平级

### 依赖方向铁律

```
Editor 模块 → 可以依赖 → Runtime 模块  ✅
Runtime 模块 → 不能依赖 → Editor 模块  ❌（打包时编译失败）
```

这是本示例要验证的核心：**单向依赖**。

## 拆分前后对比

### 拆分前（0.2，单模块）
```
StudyPlugin/               ← 一个 Runtime 模块干所有事
└── Source/StudyPlugin/
```

### 拆分后（0.3，双模块）
```
StudyPlugin/
└── Source/
    ├── StudyPlugin/           ← Runtime 模块（运行时逻辑）
    │   ├── StudyPlugin.Build.cs
    │   ├── Public/
    │   │   ├── StudyPluginModule.h       ← 生命周期
    │   │   └── StudyPluginExample.h      ← 示例类
    │   └── Private/
    │       ├── StudyPluginModule.cpp
    │       └── StudyPluginExample.cpp
    └── StudyPluginEditor/     ← Editor 模块（编辑器工具栏）
        ├── StudyPluginEditor.Build.cs    ← 依赖 StudyPlugin + UnrealEd
        ├── Public/
        │   └── StudyPluginEditorModule.h
        └── Private/
            └── StudyPluginEditorModule.cpp   ← 注册工具栏按钮，调用 Runtime 方法
```

### .uplugin 变化

```json
// 拆分前：1 个 Runtime 模块
"Modules": [{ "Name": "StudyPlugin", "Type": "Runtime" }]

// 拆分后：2 个模块
"Modules": [
    { "Name": "StudyPlugin",       "Type": "Runtime" },
    { "Name": "StudyPluginEditor", "Type": "Editor" }   // ← 新增
]
```

### Build.cs 依赖关系

```
StudyPluginEditor.Build.cs:
    PublicDependencyModuleNames: ["Core", "StudyPlugin"]     ← 依赖 Runtime 模块
    PrivateDependencyModuleNames: ["UnrealEd", "ToolMenus"]  ← 编辑器专属

StudyPlugin.Build.cs:
    不依赖 StudyPluginEditor  ← Runtime 不能依赖 Editor
```

## Editor 模块做了什么

在编辑器工具栏注册了一个按钮 "Study Plugin"，点击后：
1. 调用 Runtime 模块的 `FStudyPluginExample::GetGreeting()`
2. 调用 Runtime 模块的 `FStudyPluginExample::Add(10, 20)`
3. 输出到 Output Log 验证 Editor → Runtime 调用成功

## 预期 Log 输出

启动编辑器：
```
LogStudyPluginEditor: ===== StudyPluginEditor 已加载 =====
LogStudyPluginEditor: 工具栏按钮已注册
```

点击工具栏 "Study Plugin" 按钮：
```
LogStudyPluginEditor: ===== 工具栏按钮被点击 =====
LogStudyPluginEditor: 调用 Runtime 模块结果：
LogStudyPluginEditor:   GetGreeting() = 你好，这是来自 StudyPlugin 插件的问候！...
LogStudyPluginEditor:   Add(10, 20) = 30
LogStudyPluginEditor: ===== 验证完成：Editor 模块成功调用了 Runtime 模块 =====
```

## 实验建议

### 实验 1：反向依赖验证（会编译失败）
- 在 `StudyPlugin.Build.cs`（Runtime）中加 `"StudyPluginEditor"` 依赖
- 编译会报错 → 理解 Runtime 不能依赖 Editor

### 实验 2：查看编辑器中的模块加载
- `Edit → Plugins` → 找到 Study Plugin
- `.uplugin` 中 `Type: Editor` 的模块在打包时不包含

### 实验 3：改 LoadingPhase
- 把 StudyPluginEditor 的 `LoadingPhase` 改为 `PreDefault`
- 观察 StartupModule 日志出现时机变化
