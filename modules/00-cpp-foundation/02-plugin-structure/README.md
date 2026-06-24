# 0.2 .uplugin / Plugin 结构 — 创建第一个插件

## 学习目标

理解 UE5 插件的组成结构，创建第一个插件，并验证主项目能调用插件代码。

## 核心概念

### 插件 vs 模块

- **插件（Plugin）** = 一个可独立启用/禁用的功能包，含 `.uplugin` 描述文件，可包含多个模块、资源、内容
- **模块（Module）** = 编译单元，含 `.Build.cs` 和代码，是插件的实际代码载体
- 一个插件至少包含一个模块

### 插件目录结构

```
StudyProject/Plugins/StudyPlugin/
├── StudyPlugin.uplugin              ← 插件描述文件（JSON）
├── Resources/                       ← 图标等资源（可选）
└── Source/
    └── StudyPlugin/                 ← 模块目录（名字 = 模块名）
        ├── StudyPlugin.Build.cs     ← 模块编译规则
        ├── Public/                  ← 对外暴露的头文件
        │   ├── StudyPluginModule.h  ← 模块接口（生命周期）
        │   └── StudyPluginExample.h ← 示例类（供主项目调用）
        └── Private/                 ← 内部实现
            ├── StudyPluginModule.cpp
            └── StudyPluginExample.cpp
```

### .uplugin 关键字段

| 字段 | 说明 |
|------|------|
| `FriendlyName` | 插件显示名（编辑器插件管理器中可见） |
| `Category` | 分类（编辑器中分组用） |
| `CanContainContent` | 是否能包含 Content 资源 |
| `Modules` | 插件包含的模块列表 |
| `Modules[].Type` | 模块类型（Runtime / Editor / Developer 等） |
| `Modules[].LoadingPhase` | 加载时机（Default / PreDefault / PostConfigInit 等） |

### 模块生命周期（IModuleInterface）

```cpp
class FStudyPluginModule : public IModuleInterface
{
    virtual void StartupModule() override;   // 模块加载时
    virtual void ShutdownModule() override;  // 模块卸载时
};
IMPLEMENT_MODULE(FStudyPluginModule, StudyPlugin)  // 注册宏，必不可少
```

- `StartupModule`：注册菜单、绑定委托、初始化数据
- `ShutdownModule`：注销、清理资源
- `IMPLEMENT_MODULE`：把类注册为模块，**没有它模块无法被加载**

### Public/Private 目录约定

- `Public/` 的头文件 → 其他模块可 `#include`
- `Private/` 的代码 → 只在模块内部使用
- 想让类被外部调用，必须放 `Public/` 并加 `STUDYPLUGIN_API` 导出宏

## 验证步骤

1. **启用插件**：`StudyProject.uproject` 的 `Plugins` 数组中加 `{"Name": "StudyPlugin", "Enabled": true}`
2. **主项目依赖插件**：`StudyProject.Build.cs` 的 `PrivateDependencyModuleNames` 加 `"StudyPlugin"`
3. **主项目调用插件**：`BuildConfigTestActor.cpp` 中 `#include "StudyPluginExample.h"`，调用 `FStudyPluginExample::GetGreeting()`
4. 清理 `Intermediate\Build` → 重新生成项目文件 → 编译

## 预期 Log 输出

启动编辑器时（StartupModule）：
```
LogStudyPlugin: Warning: ===== StudyPlugin 已加载（StartupModule）=====
LogStudyPlugin: 你好，这是来自 StudyPlugin 插件的问候！插件代码成功被调用。
```

Play 时（BuildConfigTestActor 调用插件）：
```
LogTemp: Warning: ===== 插件调用验证 =====
LogTemp: 插件问候: 你好，这是来自 StudyPlugin 插件的问候！插件代码成功被调用。
LogTemp: 插件计算 3 + 5 = 8
```

关闭编辑器时（ShutdownModule）：
```
LogStudyPlugin: Warning: ===== StudyPlugin 已卸载（ShutdownModule）=====
```

## 实验建议

### 实验 1：在编辑器查看插件
- 编辑器菜单 `Edit → Plugins` → 搜索 "Study"
- 能看到你的插件，可勾选启用/禁用

### 实验 2：改 LoadingPhase
- 把 `.uplugin` 中 Module 的 `LoadingPhase` 改成 `PostConfigInit`
- 观察 StartupModule 的 log 出现时机提前了

### 实验 3：Public vs Private
- 把 `StudyPluginExample.h` 从 `Public/` 移到 `Private/`
- 主项目编译会报「找不到头文件」→ 理解 Public/Private 的可见性边界

## 关键文件清单

| 文件 | 作用 |
|------|------|
| `StudyPlugin.uplugin` | 插件描述（JSON） |
| `StudyPlugin.Build.cs` | 模块编译规则 |
| `StudyPluginModule.h/.cpp` | 模块生命周期（StartupModule/ShutdownModule） |
| `StudyPluginExample.h/.cpp` | 示例类，验证主项目可调用 |
