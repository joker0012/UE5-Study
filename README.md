# UE5 系统性学习项目

> **项目目的**：通过「写一个示例」驱动学习 UE5 每一个核心知识点。
> 从核心框架到专项模块，由浅入深，逐个击破。

---

## 项目结构

```
UE_Project/Study/
├── README.md          ← 你正在读的这个文件
├── PROJECT.md         ← 给 AI 工具读的项目元信息（AI 友好格式）
├── OUTLINE.md         ← 完整学习大纲（16 模块 / 130 知识点）
├── PROGRESS.md        ← 学习进度追踪
├── TROUBLESHOOTING.md ← 编译报错记录与解决方案
├── PROJECT.md         ← 给 AI 工具读的项目元信息（AI 友好格式）
└── modules/           ← 按模块分子目录
    ├── 01-core-framework/    核心框架 & Gameplay 架构
    ├── 02-blueprint-cpp/     蓝图 & C++ 协作
    ├── 03-enhanced-input/    Enhanced Input 系统
    ├── 04-gas/               Gameplay Ability System
    ├── 05-animation/         动画系统
    ├── 06-physics/           物理 & 碰撞
    ├── 07-ui-umg/            UI 系统 (UMG + Common UI)
    ├── 08-rendering/         渲染 & 视觉效果
    ├── 09-networking/        网络 & 多人游戏
    ├── 10-ai/                AI 系统
    ├── 11-level-design/      关卡设计 & 世界构建
    ├── 12-audio/             音频系统
    ├── 13-editor-tools/      编辑器扩展 & 工具开发
    └── 14-performance/       性能分析 & 优化
```

## 学习方式

每个知识点遵循三步走：

1. **读大纲** → 看 `OUTLINE.md`，理解知识点在全局的位置
2. **写示例** → 在对应模块目录下创建一个小 Demo，完成大纲中列出的「示例目标」
3. **记进度** → 在 `PROGRESS.md` 中标记完成状态 + 记录笔记

## 给 AI 工具的说明

- 如果你是 AI 编程助手，请先阅读 `PROJECT.md` 了解如何参与这个项目
- `OUTLINE.md` 列出了所有知识点和学习目标
- `PROGRESS.md` 记录了完成状态
- 每个 `modules/XX-xxx/` 目录下有该模块的详细说明和已完成的示例
