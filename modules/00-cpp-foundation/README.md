# 模块 0: C++ 工程基建

在写任何 UE5 C++ 代码之前，先搞懂工程骨架：Build.cs 怎么配依赖、Target.cs 怎么定编译目标、Plugin 怎么搭、Module 怎么拆、第三方库怎么接、Live Coding 怎么用。这些是让代码「能编译、能跑、能维护」的地基。

## 子目录

| # | 目录 | 知识点 |
|---|------|--------|
| 01 | `01-build-config/` | Build.cs / Target.cs — 模块依赖与编译配置 ✅ |
| 02 | `02-plugin-structure/` | .uproject / Plugin 结构 — 创建第一个插件 ✅ |
| 03 | `03-module-split/` | Module 划分 (Runtime / Editor / PrimaryGameModule) ✅ |
| 04 | `04-thirdparty-lib/` | 第三方库集成 (ThirdParty + .lib 链接) |
| 05 | `05-live-coding/` | Live Coding / Hot Reload 工作流 |

## 引擎版本

UE 5.7 — 路径 `H:\UE_Engine\UE_5.7`
