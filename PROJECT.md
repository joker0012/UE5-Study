# PROJECT.md — AI 可读的项目元信息

> 本文档面向 AI 编程助手，提供项目上下文和工作指引。
> 人类用户请读 `README.md`。

## 项目信息

- **名称**：UE5 系统性学习项目 (UE5 Study Project)
- **路径**：`H:\UE_Project\Study`
- **语言**：中文（学习笔记和文档）、C++ / Blueprint（示例代码）
- **引擎代码路径**：`H:\UE_Engine\UE_5.7` — 遇到 API 用法不清楚时查阅引擎源码
- **引擎版本**：Unreal Engine 5.7
- **开发 IDE**：JetBrains Rider（C++ 代码导航、查找引用、跳转定义均可在 Rider 中操作）
- **目标**：通过写示例逐个掌握 UE5 的 16 大模块、130 知识点
- **UE 项目路径**：`H:\UE_Project\Study\StudyProject` — 示例代码直接写在这里
- **UBT 路径**：`H:\UE_Engine\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe`

## 核心文件

| 文件 | 用途 | AI 行为 |
|------|------|---------|
| `OUTLINE.md` | 完整学习大纲 | **必读**：创建新示例前先查大纲确认知识点位置 |
| `PROGRESS.md` | 学习进度追踪 | **必须更新**：完成示例后标记状态 |
| `TROUBLESHOOTING.md` | 编译报错记录 | **必查**：遇到编译报错先查此文档；解决后必须追加记录 |
| `README.md` | 人类可读的项目说明 | 参考用 |
| `modules/XX-xxx/README.md` | 各模块子说明 | **必读**：进入某模块前先读其 README |

## 工作流程（AI 应遵循）

```
用户："我要学 XXX 知识点"
    ↓
1. 查 OUTLINE.md 找到该知识点在哪个模块、什么难度、示例目标是什么
2. 读 modules/XX-xxx/README.md 了解模块上下文
3. 在 StudyProject/Source/StudyProject/ 下创建示例代码（直接写项目里）
4. 用 UBT 命令行编译验证（见下方「编译流程」）
5. 编译通过后，同步一份到 modules/XX-xxx/YY-xxx/ 作为笔记存档
6. 更新 PROGRESS.md 标记完成
7. 如有心得，补充到 modules/XX-xxx/NOTES.md
8. 如遇到编译报错，记录到 TROUBLESHOOTING.md
```

## 每个知识点示例的文件命名规范

```
modules/04-gas/
├── README.md                    ← 模块说明
├── NOTES.md                     ← 学习笔记和心得
├── 01-asc-init/                 ← 示例 01：ASC 初始化
│   ├── README.md                ← 本示例说明
│   └── ...（UE 项目文件）
├── 02-fireball-ga/              ← 示例 02：火球术
│   └── ...
└── ...
```

## 进度追踪格式（PROGRESS.md）

采用 checklist 格式，每个知识点一行：

```markdown
## 模块 4: GAS
- [x] 4.1 ASC 初始化 + 绑定输入                                    | 2026-06-23 | 笔记: 理解了 ASC 的 Owner/Avatar 区别
- [ ] 4.2 第一个 GA — 火球术
- [ ] 4.3 AttributeSet — HP/MP/Str
```

## 约束

- 示例应该小而聚焦，一个示例只学一个知识点
- 优先用 C++，辅以蓝图展示可视化概念
- 代码要有充分的中文注释
- 每个示例要自包含，能独立编译运行
- 不允许跳过 `PROGRESS.md` 更新

---

## 项目组织规则

### 示例项目位置

- **优先在同一个 UE 项目里做多个示例**，避免创建过多零散项目
- 建议在 `H:\UE_Project\Study\` 下创建一个统一的 UE 学习项目（如 `StudyProject/`），每个模块用独立关卡或独立文件夹组织
- 如果两个示例存在严重冲突（如不同 GameMode 配置、插件版本不兼容），可以在 `H:\UE_Project\Study\` 下新建独立项目（如 `StudyProject_Physics/`）

### 引擎源码查阅

当遇到 API 不确定、参数含义不明、调用约定不清楚的情况：

1. **先搜引擎源码**：`H:\UE_Engine\UE_5.7\Engine\Source\`
2. 用 `Grep` 搜引擎头文件（`.h`）确认函数签名、参数、注释
3. 用 `Glob` 在引擎源码中找相关类名/文件名
4. 用户可在 Rider 中直接 `Ctrl+Click` 跳转定义（推荐）

### 代码搜索策略

CodeGraph 和 LSP 工具**主要用于 CFR TSProject 的 TypeScript 代码**，不适用于 UE5 C++。

- 搜 UE5 C++ 引擎源码 → 用 `Grep` + `Glob`（路径：`H:\UE_Engine\UE_5.7\Engine\Source\`）
- 搜本项目内的 C++ 示例代码 → 用 `Grep` 或直接 `Read`
- 理解 API 设计意图 → 搜引擎头文件中的 `/** ... */` Doxygen 注释

---

## 编译流程（AI 自主操作）

### 环境变量

```
UBT = H:\UE_Engine\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe
PROJECT = H:\UE_Project\Study\StudyProject\StudyProject.uproject
```

### 1. 常规增量编译（改了 .h/.cpp 但没改 Build.cs）

```powershell
& "$UBT" StudyProjectEditor Win64 Development -project="$PROJECT" -waitmutex
```

### 2. 修改了 Build.cs 后的编译（加/删依赖、改 PCH、加宏定义）

⚠️ **修改 Build.cs 后增量编译可能不生效**，必须清理中间文件：

```powershell
# 1. 清理编译中间文件
Remove-Item -Path "H:\UE_Project\Study\StudyProject\Intermediate\Build" -Recurse -Force

# 2. 重新生成项目文件
& "$UBT" -projectfiles -project="$PROJECT" -game -engine

# 3. 全量编译
& "$UBT" StudyProjectEditor Win64 Development -project="$PROJECT" -waitmutex
```

### 3. 编译结果判断

- `Result: Succeeded` + `exit_code: 0` → 编译通过
- `Result: Failed` + `error LNK2019` → 链接错误，查依赖
- `Result: Failed` + `Error CXXXX` → 编译错误，查语法

### 4. 读取编译日志

编译输出直接出现在命令行 stdout 中，AI 可直接读取分析。
UE 运行日志在 `H:\UE_Project\Study\StudyProject\Saved\Logs\StudyProject.log`。

---

## 编译报错排查规则（AI 必须遵循）

### 遇到编译报错时：

1. **先查 `TROUBLESHOOTING.md`** —— 确认是否已有相同报错的解决方案
2. **分析报错** —— 区分编译错误（CXXXX）vs 链接错误（LNKXXXX）
3. **排查思路**：
   - 编译错误（CXXXX）：查语法、宏、头文件包含、API 签名
   - 链接错误（LNK2019/LNK2001）：查 Build.cs 依赖、中间文件缓存、模块 API 宏
4. **查引擎源码** —— `H:\UE_Engine\UE_5.7\Engine\Source\` 确认 API 实际签名和所在模块
5. **修复后必须记录** —— 在 `TROUBLESHOOTING.md` 追加记录（用文档中的模板）
6. **修复后必须验证编译** —— 用 UBT 命令行重新编译确认通过

### 修改 Build.cs 的铁律：

> **凡是改了 Build.cs（加/删依赖、改 PCH 策略、加宏定义），一律清理 `Intermediate\Build` 后全量重编。**
> UBT 的增量机制对 Build.cs 变更的检测不可靠，不要信任增量编译。
