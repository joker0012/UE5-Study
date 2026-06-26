# 0.2/0.3 知识点扩展 — 模块深度解析

> 本文档是对模块 0 中「模块生命周期」「模块管理」「ToolMenus」「FAppStyle」等知识点的深度扩展。

---

## 一、模块概念与项目结构

### 什么是模块？

模块（Module）是 UE5 的**最小编译单元**。每个模块对应一个 DLL（开发时）或被合并进一个 EXE（打包时）。模块之间通过 Build.cs 声明依赖关系，编译器据此决定链接哪些库。

### 项目中的模块层级

```
项目（.uproject）
├── PrimaryGameModule（主游戏模块）
│   └── .uproject 中 Modules[0] 注册的模块
│   └── 如 StudyProject —— 游戏入口
│
├── 游戏插件模块
│   └── Plugins/XXX/Source/ 下声明的模块
│   └── 如 StudyPlugin（Runtime）、StudyPluginEditor（Editor）
│
└── 引擎模块
    └── Engine/Source/Runtime/ 下的 Core、Engine、CoreUObject 等
```

### 模块在项目中的声明方式

**方式 1：项目主模块**（在 .uproject 中声明）
```json
{
    "Modules": [
        {
            "Name": "StudyProject",
            "Type": "Runtime",         // 模块类型
            "LoadingPhase": "Default"  // 加载时机
        }
    ]
}
```

**方式 2：插件模块**（在 .uplugin 中声明）
```json
{
    "Modules": [
        { "Name": "StudyPlugin",       "Type": "Runtime" },
        { "Name": "StudyPluginEditor", "Type": "Editor" }
    ]
}
```

**方式 3：Target.cs 中引用**
```csharp
ExtraModuleNames.Add("StudyProject");  // 告诉编译器这个 Target 要编译哪些模块
```

### 如何管理模块

| 管理维度 | 在哪里做 | 做什么 |
|---------|---------|--------|
| 声明模块存在 | `.uproject` / `.uplugin` | Modules 数组中列模块名+类型 |
| 编译依赖 | `Build.cs` | Public/PrivateDependencyModuleNames |
| 编译目标 | `Target.cs` | ExtraModuleNames 指定编译哪些 |
| 模块入口 | `.cpp` 文件末尾 | `IMPLEMENT_MODULE` 宏 |
| 模块生命周期 | 继承 `IModuleInterface` | StartupModule / ShutdownModule |
| 获取模块实例 | `FModuleManager` | `LoadModuleChecked<T>("ModuleName")` |

---

## 二、模块生命周期（IModuleInterface 详解）

### 完整生命周期回调

源码位置：`Engine/Source/Runtime/Core/Public/Modules/ModuleInterface.h`

```cpp
class IModuleInterface
{
public:
    virtual ~IModuleInterface() {}

    // ━━━━━━━━━━━━━━━━ 5 个回调（按调用顺序）━━━━━━━━━━━━━━━━

    // 1. DLL 加载后、模块对象创建后立即调用
    //    适合：加载依赖模块、注册委托、初始化数据
    virtual void StartupModule() {}

    // 2. 模块加载完成后调用（StartupModule 之后）
    //    适合：需要在所有模块 Startup 后执行的初始化
    virtual void PostLoadCallback() {}

    // 3. 卸载前调用（还在正常运行状态）
    //    适合：提前保存状态、通知其他模块
    virtual void PreUnloadCallback() {}

    // 4. 模块即将卸载、对象即将销毁前调用
    //    适合：注销注册的内容（菜单、委托等）
    //    ⚠️ 按 StartupModule 的逆序调用 —— 保证依赖模块还没被卸载
    virtual void ShutdownModule() {}

    // ━━━━━━━━━━━━━━━━ 控制方法 ━━━━━━━━━━━━━━━━

    // 是否支持运行时热重载（默认 true）
    virtual bool SupportsDynamicReloading() { return true; }

    // 是否支持应用退出时自动关闭（默认 true）
    virtual bool SupportsAutomaticShutdown() { return true; }

    // 是否是游戏模块（影响某些引擎行为）
    // FDefaultGameModuleImpl 返回 true，FDefaultModuleImpl 返回 false
    virtual bool IsGameModule() const { return false; }
};
```

### 生命周期调用时序

```
引擎启动
  ↓
加载模块 DLL
  ↓
创建模块对象（new FMyModule()）
  ↓
StartupModule()      ← 注册菜单、委托、初始化数据
  ↓
PostLoadCallback()   ← 所有模块都 Startup 后
  ↓
... 编辑器/游戏正常运行 ...
  ↓
PreUnloadCallback()  ← 卸载前通知
  ↓
ShutdownModule()     ← 注销注册内容（逆序）
  ↓
销毁模块对象（delete）
  ↓
卸载模块 DLL
```

### 两个默认实现类

| 类 | 用途 | IsGameModule() |
|---|---|---|
| `FDefaultModuleImpl` | 最小实现，什么都不做 | false（引擎/插件模块用） |
| `FDefaultGameModuleImpl` | 继承 FDefaultModuleImpl | true（游戏模块用） |

如果你不需要自定义生命周期逻辑，可以不继承 IModuleInterface，直接用默认实现：
```cpp
IMPLEMENT_MODULE(FDefaultGameModuleImpl, StudyProject)  // 无自定义逻辑
```

---

## 三、IMPLEMENT_MODULE 详解

### 放在哪里？

**必须放在模块的某个 `.cpp` 文件的末尾，全局作用域中。**

```cpp
// StudyPluginModule.cpp

#include "StudyPluginModule.h"
// ... 其他 include ...

// ... 代码实现 ...

// ━━━━━━━━━━━ 末尾，全局作用域 ━━━━━━━━━━━
IMPLEMENT_MODULE(FStudyPluginModule, StudyPlugin)
```

### 为什么只能有一个？

`IMPLEMENT_MODULE` 宏展开后会：
1. 生成一个 `InitializeModuleNameModule()` 函数
2. 注册一个 `FModuleInitializerEntry` 静态对象

如果一个模块在两个 .cpp 中写了 `IMPLEMENT_MODULE`，会报链接错误（重复定义）。

### IMPLEMENT_MODULE vs IMPLEMENT_GAME_MODULE

| 宏 | 用途 |
|---|---|
| `IMPLEMENT_MODULE` | 非游戏模块（引擎模块、工具模块、插件模块） |
| `IMPLEMENT_GAME_MODULE` | 游戏模块（`IsGameModule()` 返回 true） |

实际上 `IMPLEMENT_GAME_MODULE` 只是调用了 `IMPLEMENT_MODULE`：
```cpp
#define IMPLEMENT_GAME_MODULE(ModuleImplClass, ModuleName) \
    IMPLEMENT_MODULE(ModuleImplClass, ModuleName)
```

区别在于你继承的类：
- `FDefaultModuleImpl` → `IsGameModule()` 返回 false → 用 `IMPLEMENT_MODULE`
- `FDefaultGameModuleImpl` → `IsGameModule()` 返回 true → 用 `IMPLEMENT_GAME_MODULE`

### 宏展开做了什么

```cpp
// 你的代码
IMPLEMENT_MODULE(FStudyPluginModule, StudyPlugin)

// 宏展开后（简化版）
static IModuleInterface* InitializeStudyPluginModule()
{
    return new FStudyPluginModule();   // ← 创建模块实例
}

static FModuleInitializerEntry StudyPluginInitializerEntry(
    TEXT("StudyPlugin"),               // ← 模块名
    InitializeStudyPluginModule        // ← 工厂函数
);

// 编译时检查：IMPLEMENT_MODULE 中的名字必须与 UE_MODULE_NAME 宏一致
extern "C" void IMPLEMENT_MODULE_StudyPlugin() {
    UE_STATIC_ASSERT_WARN(
        UE::Core::Private::ModuleNameEquals("StudyPlugin", UE_MODULE_NAME),
        "Module name mismatch"
    );
}
```

引擎启动时，`FModuleManager` 遍历所有 `FModuleInitializerEntry`，按需调用工厂函数创建模块实例。

---

## 四、Runtime vs Editor 模块定义详解

### 怎么把模块定义为 Runtime 或 Editor

在 `.uplugin`（插件模块）或 `.uproject`（项目模块）的 `Modules` 数组中设置 `Type` 字段：

```json
// .uplugin — 插件的双模块配置
"Modules": [
    {
        "Name": "StudyPlugin",
        "Type": "Runtime",           // ← Runtime 模块：编辑器+打包都编译
        "LoadingPhase": "Default"
    },
    {
        "Name": "StudyPluginEditor",
        "Type": "Editor",            // ← Editor 模块：仅编辑器编译，打包排除
        "LoadingPhase": "Default"
    }
]
```

### 所有模块类型

| Type | 编译时机 | 能依赖 | 典型用途 |
|------|---------|--------|---------|
| `Runtime` | 编辑器 + 打包 | Runtime 模块 | Actor、Component、游戏逻辑 |
| `Editor` | 仅编辑器 | Runtime + Editor | 编辑器工具、Detail 自定义 |
| `Developer` | Dev/Staging（非 Shipping） | Runtime + Developer | 开发工具、性能分析器 |
| `ServerOnly` | 仅服务端 | Runtime + Server | 专用服务器逻辑 |
| `ClientOnly` | 仅客户端 | Runtime + Client | 客户端专属逻辑 |
| `Program` | 独立程序 | 视情况 | 不在引擎内运行 |

### LoadingPhase（加载时机）

| Phase | 时机 | 典型用途 |
|-------|------|---------|
| `PostConfigInit` | 最早，配置加载后 | 需要读配置的底层模块 |
| `PreEarlyLoading` | 引擎核心初始化前 | 需要修改引擎启动行为的模块 |
| `PreDefault` | 大多数模块之前 | 需要被其他模块依赖的底层模块 |
| `Default` | 正常时机（最常用） | 90% 的模块 |
| `PostDefault` | 大多数模块之后 | 需要等所有 Default 模块加载完 |
| `PostEngineInit` | 引擎完全初始化后 | 不影响启动速度的模块 |

### 依赖方向铁律

```
Editor → 可以依赖 → Runtime  ✅
Runtime → 不能依赖 → Editor  ❌  （打包时 Editor 模块不存在，链接失败）
```

在 Build.cs 中体现：
```csharp
// StudyPluginEditor.Build.cs (Editor 模块)
PublicDependencyModuleNames.Add("StudyPlugin");  // ✅ Editor 依赖 Runtime

// StudyPlugin.Build.cs (Runtime 模块)
// 不能写 PublicDependencyModuleNames.Add("StudyPluginEditor");  // ❌
```

---

## 五、PrimaryGameModule 详解

### 什么是 PrimaryGameModule？

PrimaryGameModule 是 `.uproject` 中 `Modules` 数组的**第一个模块**，也叫**主游戏模块**。

```json
{
    "Modules": [
        { "Name": "StudyProject", "Type": "Runtime" }  // ← 这就是 PrimaryGameModule
    ]
}
```

### 它有什么特殊之处？

| 特性 | PrimaryGameModule | 普通模块 |
|------|-------------------|---------|
| 是否自动被 Target.cs 引用 | ✅ 自动（不用在 ExtraModuleNames 中声明） | ❌ 需要手动声明 |
| 是否是游戏入口 | ✅ 是 | ❌ 不是 |
| `IsGameModule()` | 通常返回 true | 视情况 |
| 能否被其他模块依赖 | ✅ 可以 | ✅ 可以 |
| UHT 是否自动扫描 | ✅ 优先扫描 | ✅ 扫描 |

### Target.cs 中的关系

```csharp
// StudyProjectEditor.Target.cs
ExtraModuleNames.Add("StudyProject");
// 这里声明的就是 PrimaryGameModule 的名字
// 它和 .uproject 中 Modules[0] 的 Name 必须一致
```

### 多游戏模块的情况

一个项目可以有多个游戏模块（都在 .uproject 的 Modules 中声明），但只有第一个是 Primary。其他游戏模块需要在 Build.cs 中依赖 PrimaryGameModule 或互相依赖。

---

## 六、ToolMenus 详解 — 在编辑器各位置注册 UI

### 核心 API

```cpp
// 获取 ToolMenus 单例
UToolMenus* ToolMenus = UToolMenus::Get();

// 检查是否可用
bool bEnabled = UToolMenus::IsToolMenuUIEnabled();

// 在已有菜单上追加内容（不覆盖原有项）
UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("菜单路径");

// 注册全新菜单（如果不存在则创建）
UToolMenu* Menu = UToolMenus::Get()->RegisterMenu("菜单路径", Parent路径, Type);

// 注销某个 Owner 注册的所有条目
UToolMenus::Get()->UnregisterOwner(this);
```

### 常用菜单路径（从引擎源码中提取）

| 位置 | 路径 | 说明 |
|------|------|------|
| **Play 工具栏** | `LevelEditor.LevelEditorToolBar.PlayToolBar` | 顶部 Play 按钮所在工具栏 |
| **Modes 工具栏** | `LevelEditor.LevelEditorToolBar.ModesToolBar` | 左侧模式选择栏 |
| **Assets 工具栏** | `LevelEditor.LevelEditorToolBar.AssetsToolBar` | 资产工具栏 |
| **Settings 工具栏** | `LevelEditor.LevelEditorToolBar.SettingsToolBar` | 设置工具栏 |
| **Window 菜单** | `LevelEditor.MainMenu.Window` | 顶部菜单栏 Window 项 |
| **File 菜单** | `LevelEditor.MainMenu.File` | 顶部菜单栏 File 项 |
| **Edit 菜单** | `LevelEditor.MainMenu.Edit` | 顶部菜单栏 Edit 项 |
| **Actor 右键菜单** | `LevelEditor.ActorContextMenu` | 视口选中 Actor 右键 |
| **Component 右键菜单** | `LevelEditor.ComponentContextMenu` | 组件右键 |
| **Scene Outliner 右键** | `LevelEditor.SceneOutlinerContextMenu` | 世界大纲右键 |
| **空白处右键** | `LevelEditor.EmptySelectionContextMenu` | 视口未选中右键 |

### 两种初始化方法

```cpp
// 工具栏按钮 —— 用 InitToolBarButton
// 参数：名称、点击动作、标签、tooltip、图标
Section.AddEntry(FToolMenuEntry::InitToolBarButton(
    "MyButton",
    FExecuteAction::CreateRaw(this, &FMyModule::OnClicked),
    LOCTEXT("Label", "按钮文字"),
    LOCTEXT("Tooltip", "提示文字"),
    FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Settings")
));

// 菜单项 —— 用 InitMenuEntry
// 参数：名称、标签、tooltip、图标、点击动作
// ⚠️ 注意参数顺序！Icon 在 Action 前面
Section.AddEntry(FToolMenuEntry::InitMenuEntry(
    "MyMenuItem",
    LOCTEXT("Label", "菜单项文字"),
    LOCTEXT("Tooltip", "提示文字"),
    FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Info"),
    FExecuteAction::CreateRaw(this, &FMyModule::OnClicked)
));
```

### 本项目的三个示例位置

```
1. 主工具栏 PlayToolBar → InitToolBarButton → 工具栏上多一个按钮
2. 顶部菜单 Window 菜单  → InitMenuEntry    → Window 菜单下多一项
3. 视口右键 Actor 菜单   → InitMenuEntry    → 右键 Actor 多一项
```

### ShutdownModule 中注销

```cpp
void FStudyPluginEditorModule::ShutdownModule()
{
    // UnregisterOwner 一次性清除该 Owner 注册的所有菜单项
    // 不需要逐个删除 —— 引擎自动跟踪每个条目的 Owner
    if (UToolMenus::IsToolMenuUIEnabled())
    {
        UToolMenus::UnregisterOwner(this);
    }
}
```

---

## 七、FAppStyle 详解

### 什么是 FAppStyle？

`FAppStyle` 是 UE5 的**统一编辑器样式系统**，提供图标、颜色、字体、边距等样式资源的访问接口。

源码位置：`Engine/Source/Runtime/SlateCore/Public/Styling/AppStyle.h`

### 为什么用它替代 FEditorStyle？

- `FEditorStyle` 是旧版样式系统，UE5.4+ 开始废弃
- `FAppStyle` 是新版统一系统，支持自定义主题、跨平台样式
- UE5.7 中 `FEditorStyle::GetStyleSetName()` 已标记为 deprecated（C4996 警告）

### 主要 API

```cpp
// 获取样式集名 —— 最常用于 FSlateIcon
static const FName GetAppStyleSetName();

// 获取样式引用
static const ISlateStyle& Get();

// 获取画刷（图标）—— 用于 Slate UI
static const FSlateBrush* GetBrush(FName PropertyName);

// 获取颜色
static const FLinearColor& GetColor(FName PropertyName);

// 获取字体
static FSlateFontInfo GetFontStyle(FName PropertyName);

// 获取边距
static const FMargin& GetMargin(FName PropertyName);

// 获取浮点值
static float GetFloat(FName PropertyName);
```

### 常用图标名

```cpp
FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Settings")    // 齿轮
FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Search")      // 搜索
FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Info")        // 信息
FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Delete")      // 删除
FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Plus")        // 添加
FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Edit")        // 编辑
```

### 使用场景

```cpp
// 1. 给菜单/工具栏按钮设置图标
FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "Icons.Settings");

// 2. 在 Slate UI 中获取画刷
const FSlateBrush* Brush = FAppStyle::Get().GetBrush("Icons.Settings");

// 3. 获取字体样式
FSlateFontInfo Font = FAppStyle::GetFontStyle("NormalText");

// 4. 获取颜色
FLinearColor Color = FAppStyle::GetColor("AccentColor");
```

### 需要的依赖

```csharp
// Build.cs
PrivateDependencyModuleNames.AddRange(new string[] {
    "SlateCore",    // FAppStyle 在这个模块
    "Slate",        // Slate UI 框架
});
```

```cpp
// 头文件
#include "Styling/AppStyle.h"
```
