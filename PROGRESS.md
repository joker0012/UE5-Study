# 学习进度

> 最后更新：2026-06-26
> 总进度：7 / 130
> 对应大纲：OUTLINE.md (v2)

## 进度总览

| 模块 | 完成 | 总计 | 进度 |
|------|:---:|:---:|:----:|
| 0. C++ 工程基建 | 5 | 5 | 100% |
| 1. 核心框架 & Gameplay 架构 | 2 | 19 | 11% |
| 2. 蓝图 & C++ 协作 | 0 | 7 | 0% |
| 3. Enhanced Input 系统 | 0 | 6 | 0% |
| 4. GAS (Gameplay Ability System) | 0 | 8 | 0% |
| 5. 动画系统 | 0 | 11 | 0% |
| 6. 物理 & 碰撞 | 0 | 7 | 0% |
| 7. UI 系统 (UMG + Common UI) | 0 | 8 | 0% |
| 8. 渲染 & 视觉效果 | 0 | 9 | 0% |
| 9. 网络 & 多人游戏 | 0 | 8 | 0% |
| 10. AI 系统 | 0 | 8 | 0% |
| 11. 关卡设计 & 世界构建 | 0 | 9 | 0% |
| 12. 音频系统 | 0 | 5 | 0% |
| 13. 编辑器扩展 & 工具开发 | 0 | 6 | 0% |
| 14. 调试 & 性能分析优化 | 0 | 11 | 0% |
| 15. 综合实战切片 | 0 | 3 | 0% |
| **合计** | **0** | **130** | **0%** |

---

## 模块 0: C++ 工程基建

- [x] 0.1 Build.cs / Target.cs — 模块依赖与编译配置                    | 2026-06-23 | 笔记: 理解 Public/Private 依赖区别，Target.cs 管编译目标，Build.cs 管模块依赖
- [x] 0.2 .uproject / Plugin 结构 — 创建第一个插件                    | 2026-06-24 | 笔记: 插件=描述文件+模块，IModuleInterface 生命周期，Public/Private 可见性，主项目可依赖插件模块
- [x] 0.3 Module 划分 (Runtime / Editor / PrimaryGameModule)          | 2026-06-24 | 笔记: Runtime+Editor 双模块拆分，Editor→Runtime 单向依赖，ToolMenus 注册工具栏按钮
- [x] 0.4 第三方库集成 (ThirdParty + .lib 链接)                       | 2026-06-24 | 笔记: C 静态库三步集成法(include path + .lib 链接 + Wrapper 包装), extern "C" 防 name mangling
- [x] 0.5 Live Coding / Hot Reload 工作流                              | 2026-06-25 | 笔记: Live Coding 改函数体秒级热重载不丢状态，加 UPROPERTY/UFUNCTION 需完整重编，改 Build.cs 需清缓存

## 模块 1: 核心框架 & Gameplay 架构

- [x] 1.1 UObject & UFUNCTION / UPROPERTY 宏                         | 2026-06-26 | 笔记: UCLASS/UPROPERTY/UFUNCTION 三大反射宏; EditAnywhere/EditDefaultsOnly/VisibleAnywhere 访问控制; BlueprintCallable/BlueprintPure/BlueprintNativeEvent/BlueprintImplementableEvent 函数类型; TFieldIterator 遍历属性; FindPropertyByName 反射读写; NewObject<T>() 创建实例
- [x] 1.2 UE_LOG / check / ensure 调试宏体系                           | 2026-06-26 | 笔记: UE_LOG 格式化+日志级别; 自定义 Log Category (DECLARE_LOG_CATEGORY_CLASS); check 崩溃断言 vs ensure 软断言(返回bool不崩); verify 在 Shipping 保留表达式; UE_CLOG 条件日志; UE_LOGFMT 结构化日志
- [ ] 1.3 FName / FString / FText 区别与转换
- [ ] 1.4 TArray / TMap / TSet 容器 + 迭代器
- [ ] 1.5 AActor 生命周期
- [ ] 1.6 UActorComponent vs USceneComponent
- [ ] 1.7 APawn & ACharacter
- [ ] 1.8 APlayerController & APlayerState
- [ ] 1.9 AGameModeBase vs AGameMode
- [ ] 1.10 AGameStateBase
- [ ] 1.11 Subsystem
- [ ] 1.12 DataAsset (UPrimaryDataAsset)
- [ ] 1.13 Interface (UInterface)
- [ ] 1.14 Delegate (单播/多播/动态)
- [ ] 1.15 FTimerHandle / Timer
- [ ] 1.16 FStreamableManager 异步加载
- [ ] 1.17 ObjectPool 对象池
- [ ] 1.18 SaveGame 系统
- [ ] 1.19 软引用 / 硬引用 / TSoftObjectPtr

## 模块 2: 蓝图 & C++ 协作

- [ ] 2.1 BlueprintNativeEvent / BlueprintImplementableEvent
- [ ] 2.2 BlueprintCallable / BlueprintPure
- [ ] 2.3 UPROPERTY EditAnywhere / BlueprintReadOnly
- [ ] 2.4 Blueprint Macros vs Functions vs Event
- [ ] 2.5 Blueprint Interface 通信
- [ ] 2.6 Event Dispatcher 事件分发
- [ ] 2.7 Blueprint Nativization

## 模块 3: Enhanced Input 系统

- [ ] 3.1 InputAction + InputMappingContext 基础
- [ ] 3.2 InputModifier (Negate / DeadZone / Swizzle)
- [ ] 3.3 InputTrigger (Tap / Hold / Chord / Combo)
- [ ] 3.4 多 IMC 切换 (人/车/UI 模式)
- [ ] 3.5 UEnhancedInputComponent 绑定
- [ ] 3.6 自定义 InputModifier / Trigger

## 模块 4: GAS

- [ ] 4.1 ASC 初始化 + 绑定输入
- [ ] 4.2 第一个 GameplayAbility — 火球术
- [ ] 4.3 AttributeSet — HP / MP / Str
- [ ] 4.4 GameplayEffect — 伤害 + 持续 Buff
- [ ] 4.5 GameplayTag — 状态管理
- [ ] 4.6 GameplayCue — 视觉/音效反馈
- [ ] 4.7 MMC / ExecCalc — 自定义伤害公式
- [ ] 4.8 Prediction — 客户端预测

## 模块 5: 动画系统

- [ ] 5.1 Animation Blueprint 状态机
- [ ] 5.2 Blend Space (1D / 2D)
- [ ] 5.3 Aim Offset / Layered Blend Per Bone
- [ ] 5.4 Montage + Slot + Notify
- [ ] 5.5 Animation Blueprint State Alias
- [ ] 5.6 Animation Layers (Linked Anim Graph)
- [ ] 5.7 IK (Two Bone IK / FABRIK)
- [ ] 5.8 Control Rig + Sequencer
- [ ] 5.9 Motion Matching / Chooser Table
- [ ] 5.10 Motion Warping
- [ ] 5.11 Animation Budget Allocator / Significance

## 模块 6: 物理 & 碰撞

- [ ] 6.1 Collision Channel / Trace / Object Type
- [ ] 6.2 OnComponentBeginOverlap / EndOverlap
- [ ] 6.3 Simulate Physics / AddForce / AddImpulse
- [ ] 6.4 Shape Trace (Sphere / Box / Capsule)
- [ ] 6.5 Physics Constraint
- [ ] 6.6 Chaos Destruction
- [ ] 6.7 Chaos Vehicles

## 模块 7: UI 系统

- [ ] 7.1 UUserWidget 创建与显示
- [ ] 7.2 数据绑定 UMG
- [ ] 7.3 Button / Text / Image / ProgressBar
- [ ] 7.4 ListView / TileView 动态列表
- [ ] 7.5 UMG Animation
- [ ] 7.6 WidgetComponent (3D Widget)
- [ ] 7.7 Common UI — 跨平台输入
- [ ] 7.8 Common UI — ActivatableWidget 栈

## 模块 8: 渲染 & 视觉效果

- [ ] 8.1 Material 基础
- [ ] 8.2 Material Instance 动态参数
- [ ] 8.3 Niagara 粒子基础
- [ ] 8.4 Material Function & Material Layer
- [ ] 8.5 Substrate 材质系统
- [ ] 8.6 Niagara GPU 粒子 + 碰撞
- [ ] 8.7 Post Process Volume
- [ ] 8.8 Lumen & Nanite 实战
- [ ] 8.9 Render Dependency Graph (RDG)

## 模块 9: 网络 & 多人

- [ ] 9.1 Replicated / RepNotify
- [ ] 9.2 Server / Client / NetMulticast RPC
- [ ] 9.3 Role / RemoteRole / Authority
- [ ] 9.4 Owner / Instigator / AutonomousProxy
- [ ] 9.5 Replication Graph
- [ ] 9.6 RPC 可靠性
- [ ] 9.7 Seamless Travel
- [ ] 9.8 Online Subsystem

## 模块 10: AI 系统

- [ ] 10.1 AIController + NavMesh + SimpleMoveTo
- [ ] 10.2 Behavior Tree + Blackboard
- [ ] 10.3 BTTask / BTService / BTDecorator 自定义
- [ ] 10.4 EQS
- [ ] 10.5 AI Perception
- [ ] 10.6 StateTree
- [ ] 10.7 Smart Objects
- [ ] 10.8 Mass AI / ZoneGraph

## 模块 11: 关卡设计

- [ ] 11.1 Level Streaming
- [ ] 11.2 Level Instance / Packed Level Actor
- [ ] 11.3 DataLayer 基本用法
- [ ] 11.4 World Partition
- [ ] 11.5 World Partition — DataLayer + HLOD
- [ ] 11.6 Landscape + Foliage
- [ ] 11.7 Water System
- [ ] 11.8 PCG
- [ ] 11.9 Custom Streaming Source

## 模块 12: 音频

- [ ] 12.1 SoundCue / SoundWave 基础
- [ ] 12.2 Attenuation / 空间化
- [ ] 12.3 Audio Volume / Reverb
- [ ] 12.4 MetaSounds
- [ ] 12.5 Submix / Dynamic Sound Class

## 模块 13: 编辑器扩展

- [ ] 13.1 Editor Utility Widget
- [ ] 13.2 Asset Action
- [ ] 13.3 IDetailCustomization
- [ ] 13.4 Asset Manager / PrimaryAssetType
- [ ] 13.5 FMenuBuilder / FToolBarBuilder
- [ ] 13.6 DataTable / CSV 导入导出

## 模块 14: 调试 & 性能

- [ ] 14.1 Stat 命令
- [ ] 14.2 Gameplay Debugger
- [ ] 14.3 Unreal Insights 基础
- [ ] 14.4 Visual Logger
- [ ] 14.5 LOD
- [ ] 14.6 Culling
- [ ] 14.7 Memory Insights / 内存追踪
- [ ] 14.8 崩溃 Callstack / 断点调试
- [ ] 14.9 Blueprint Nativization / 蓝图性能
- [ ] 14.10 GC 优化
- [ ] 14.11 打包优化 (Chunking / Cook / Shader)

## 模块 15: 综合实战切片

- [ ] 15.1 第三人称 ARPG 切片 (整合 1/3/4/5/7)
- [ ] 15.2 联机射击切片 (整合 6/9/12)
- [ ] 15.3 开放世界 Demo (整合 10/11/14)
