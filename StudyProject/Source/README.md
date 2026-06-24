# 0.1 Build.cs / Target.cs — 模块依赖与编译配置

## 学习目标

理解 UE5 工程编译的两大配置文件：
1. **Target.cs** —— 定义「编译出什么」（Game / Editor / Client / Server）
2. **Build.cs** —— 定义「模块依赖什么」（Public / Private / 条件依赖）

## 核心概念

### Target.cs vs Build.cs

| | Target.cs | Build.cs |
|---|---|---|
| **管什么** | 编译目标（可执行文件） | 模块规则（代码依赖） |
| **一个项目几个** | 通常 2 个（Game + Editor） | 每个模块 1 个 |
| **关键属性** | `Type`, `ExtraModuleNames` | `PublicDependencyModuleNames`, `PrivateDependencyModuleNames` |
| **位置** | `Source/` 根目录 | `Source/模块名/` 目录 |

### Public vs Private 依赖（最重要的概念）

```
模块 A 的 .h 中 #include "ModuleB.h"  →  Build.cs 里写 Public 依赖 B
模块 A 的 .cpp 中 #include "ModuleC.h" →  Build.cs 里写 Private 依赖 C
```

- **Public**：头文件里用了 → 依赖本模块的人也会自动继承 → 传递性
- **Private**：只在 .cpp 里用 → 不暴露给使用者 → 封装性更好

**原则**：能用 Private 就用 Private，减少耦合。

## 文件说明

| 文件 | 说明 |
|------|------|
| `StudyProject.Target.cs` | Game Target —— 打包用，不含编辑器 |
| `StudyProjectEditor.Target.cs` | Editor Target —— 开发用，带编辑器 |
| `StudyProject.Build.cs` | 模块规则 —— 定义依赖、PCH、宏定义 |
| `BuildConfigTestActor.h` | 测试 Actor 头文件 —— 用到反射宏、GameplayTag |
| `BuildConfigTestActor.cpp` | 测试 Actor 实现 —— 验证各模块 API 可用 |

## 验证步骤

1. 用 UE5.7 创建一个空的 C++ 项目 `StudyProject`
2. 将本目录的 Target.cs 和 Build.cs 复制到项目 `Source/` 目录
3. 将 Actor 的 .h/.cpp 复制到 `Source/StudyProject/` 目录
4. 重新生成项目文件（右键 .uproject → Generate Project Files）
5. 用 Rider 打开编译
6. 在编辑器中拖入 `BuildConfigTestActor` 到关卡
7. 按 Play，查看 Output Log 中的验证输出

## 预期 Log 输出

```
LogTemp: Warning: ===== BuildConfig 验证 =====
LogTemp: 类名: BuildConfigTestActor
LogTemp: 位置: X=... Y=... Z=...
LogTemp: TestTag: 未设置（在编辑器 Detail 面板设置试试）
LogTemp: STUDY_PROJECT_VERSION = 1
LogTemp: Warning: ===== 验证完成，如果上面都有输出说明依赖配置正确 =====
```

## 实验建议

### 实验 1：删掉依赖看报错
- 把 Build.cs 中的 `"GameplayTags"` 删掉
- 重新编译 → 会看到链接错误（unresolved external symbol）
- 理解：**用了哪个模块的 API，就必须在 Build.cs 声明依赖**

### 实验 2：Public vs Private
- 把 `BuildConfigTestActor.h` 中的 `#include "GameplayTagContainer.h"` 移到 .cpp 中
- 把 GameplayTags 从 Public 移到 Private → 编译通过（因为头文件不再直接引用）
- 理解：**Private 依赖的前提是头文件不直接 #include**

### 实验 3：条件依赖
- 取消 Build.cs 中 `if (Target.bBuildEditor)` 块的注释
- 添加 `#include "Editor/UnrealEd/Public/UnrealEd.h"` 到 .cpp
- 编辑器编译通过，打包时不会包含（减小包体）
