# UE5 学习大纲 — 示例驱动

> 每个条目 = 一个独立的示例练习。
> 难度：🟢 入门 | 🟡 进阶 | 🔴 高级

---

## 1. 核心框架 & Gameplay 架构

| # | 难度 | 知识点 | 示例目标 |
|---|:---:|--------|----------|
| 1.1 | 🟢 | UObject & UFUNCTION / UPROPERTY 宏 | 写一个带反射属性的数据类，打印 GetClass()->GetName() |
| 1.2 | 🟢 | AActor 生命周期 | 写一个 Actor，在 BeginPlay / Tick / EndPlay 中打 log |
| 1.3 | 🟢 | UActorComponent vs USceneComponent | 创建带位置 Actor + 纯逻辑 Component，理解区别 |
| 1.4 | 🟢 | APawn & ACharacter | 创建可操控角色：WASD 移动 + 空格跳跃 |
| 1.5 | 🟢 | APlayerController & APlayerState | PC 处理输入，PS 存取玩家分数 |
| 1.6 | 🟢 | AGameModeBase vs AGameMode | 自定义 GameMode：控制出生点 + 胜利条件 |
| 1.7 | 🟢 | AGameStateBase | GameState 维护全局分数，客户端自动同步 |
| 1.8 | 🟡 | Subsystem | 写一个全局音频管理 Subsystem，跨关卡存活 |
| 1.9 | 🟡 | DataAsset (UPrimaryDataAsset) | 创建武器 DataAsset：伤害/射速/图标 |
| 1.10| 🟡 | Interface (UInterface) | 定义 IDamageable，角色和箱子都实现它 |
| 1.11| 🟡 | Delegate (单播/多播/动态) | 血量变化广播 OnHealthChanged，UI 和音效分别监听 |
| 1.12| 🟡 | FTimerHandle / Timer | 技能冷却计时器 + 延时爆炸 |
| 1.13| 🟡 | FStreamableManager 异步加载 | 运行时异步加载武器 Mesh + 音效 |
| 1.14| 🔴 | ObjectPool 对象池 | 子弹对象池：Spawn/Recycle 循环复用 |
| 1.15| 🔴 | SaveGame 系统 | 存档/读档：玩家位置 + 背包 + 关卡进度 |
| 1.16| 🔴 | 软引用 / 硬引用 / TSoftObjectPtr | 对比三种引用方式，用 Asset Manager 加载 |

## 2. 蓝图 & C++ 协作

| # | 难度 | 知识点 | 示例目标 |
|---|:---:|--------|----------|
| 2.1 | 🟢 | BlueprintNativeEvent / BlueprintImplementableEvent | C++ 定义事件 → 蓝图重写（双向互调） |
| 2.2 | 🟢 | BlueprintCallable / BlueprintPure | 暴露 C++ 函数给蓝图，区分副作用 vs 纯查询 |
| 2.3 | 🟢 | UPROPERTY EditAnywhere / BlueprintReadOnly | 策划在蓝图面板调参，C++ 负责逻辑 |
| 2.4 | 🟡 | Blueprint Macros vs Functions vs Event | 同样「计算伤害」逻辑，对比三种方式 |
| 2.5 | 🟡 | Blueprint Interface 通信 | 纯蓝图项目：BPI 让不同类互相发消息 |
| 2.6 | 🟡 | Event Dispatcher 事件分发 | 关卡开关触发 → 多个门/灯/警报响应 |
| 2.7 | 🔴 | Blueprint Nativization | 复杂蓝图 → C++，对比性能 |

## 3. Enhanced Input 系统

| # | 难度 | 知识点 | 示例目标 |
|---|:---:|--------|----------|
| 3.1 | 🟢 | InputAction + InputMappingContext 基础 | 绑定 Jump / Fire，替换旧版 AxisMapping |
| 3.2 | 🟢 | InputModifier (Negate / DeadZone / Swizzle) | 摇杆输入加死区 + 反向处理 |
| 3.3 | 🟡 | InputTrigger (Tap / Hold / Chord / Combo) | 长按蓄力 + 双击闪避 + A+B 组合技 |
| 3.4 | 🟡 | 多 IMC 切换 (人/车/UI 模式) | 人形态 vs 载具形态，两套 IMC |
| 3.5 | 🟡 | UEnhancedInputComponent 绑定 | C++ 中用 EnhancedInputComponent 直接绑定 |
| 3.6 | 🔴 | 自定义 InputModifier / Trigger | 写「摇杆画圈检测」自定义 Trigger |

## 4. GAS (Gameplay Ability System)

| # | 难度 | 知识点 | 示例目标 |
|---|:---:|--------|----------|
| 4.1 | 🟢 | ASC 初始化 + 绑定输入 | 角色挂 ASC，BindAbilityActivationToInputComponent |
| 4.2 | 🟢 | 第一个 GameplayAbility — 火球术 | GA：按左键发射投射物，冷却 3 秒 |
| 4.3 | 🟢 | AttributeSet — HP / MP / Str | 创建属性集 + UI 血条 |
| 4.4 | 🟡 | GameplayEffect — 伤害 + 持续 Buff | 即时伤害 GE + 5 秒加攻 Buff GE |
| 4.5 | 🟡 | GameplayTag — 状态管理 | Tag 阻挡技能：被眩晕时所有技能不可用 |
| 4.6 | 🟡 | GameplayCue — 视觉/音效反馈 | GE 触发命中特效 + 音效（Batched Cue） |
| 4.7 | 🔴 | MMC / ExecCalc — 自定义伤害公式 | ExecCalc：伤害 = (ATK - DEF) × 克制系数 |
| 4.8 | 🔴 | Prediction — 客户端预测 | 带预测的冲刺技能，对比有/无预测手感 |

## 5. 动画系统

| # | 难度 | 知识点 | 示例目标 |
|---|:---:|--------|----------|
| 5.1 | 🟢 | Animation Blueprint 状态机 | Idle → Walk → Run → Jump 状态切换 |
| 5.2 | 🟢 | Blend Space (1D / 2D) | 速度+方向驱动 BlendSpace，平滑混合走/跑/转向 |
| 5.3 | 🟢 | Aim Offset / Layered Blend Per Bone | 上半身瞄准 + 下半身移动，不冲突 |
| 5.4 | 🟡 | Montage + Slot + Notify | 攻击 Montage，关键帧 Notify 触发伤害判定 |
| 5.5 | 🟡 | Animation Blueprint State Alias | 多状态共享同一逻辑入口 |
| 5.6 | 🟡 | IK (Two Bone IK / FABRIK) | 脚部 IK 贴合地形 + 手部 IK 拾取物品 |
| 5.7 | 🟡 | Control Rig + Sequencer | Control Rig 自定义过场动画 |
| 5.8 | 🔴 | Motion Warping | 滑铲后自动贴合掩体 + 翻越障碍 |
| 5.9 | 🔴 | Animation Budget Allocator / Significance | 远处角色降频更新动画 |

## 6. 物理 & 碰撞

| # | 难度 | 知识点 | 示例目标 |
|---|:---:|--------|----------|
| 6.1 | 🟢 | Collision Channel / Trace / Object Type | 自定义碰撞通道 + 射线检测 |
| 6.2 | 🟢 | OnComponentBeginOverlap / EndOverlap | 自动门：角色进入触发区开门，离开关门 |
| 6.3 | 🟢 | Simulate Physics / AddForce / AddImpulse | 打碎花瓶：碎片物理模拟 + 爆炸力 |
| 6.4 | 🟡 | Shape Trace (Sphere / Box / Capsule) | 扇形 AOE 技能范围检测 |
| 6.5 | 🟡 | Physics Constraint | 绳子/铰链连接两物体，物理摆锤 |
| 6.6 | 🔴 | Chaos Destruction | GeometryCollection 建筑破坏 |
| 6.7 | 🔴 | Chaos Vehicles | 可驾驶四轮载具 |

## 7. UI 系统 (UMG + Common UI)

| # | 难度 | 知识点 | 示例目标 |
|---|:---:|--------|----------|
| 7.1 | 🟢 | UUserWidget 创建与显示 | HUD：血条 + 蓝条 + 弹药数 |
| 7.2 | 🟢 | 数据绑定 UMG | 血条属性绑定自动更新，不写 Tick |
| 7.3 | 🟢 | Button / Text / Image / ProgressBar | 主菜单：开始游戏、设置、退出 |
| 7.4 | 🟡 | ListView / TileView 动态列表 | 背包 UI，物品列表动态生成 + 滚动 |
| 7.5 | 🟡 | UMG Animation | 伤害数字弹出 + 淡出动画 |
| 7.6 | 🟡 | WidgetComponent (3D Widget) | 敌人头顶血条，始终面向摄像机 |
| 7.7 | 🔴 | Common UI — 跨平台输入 | 同一 UI 自动适配手柄/鼠标/触屏 |
| 7.8 | 🔴 | Common UI — ActivatableWidget 栈 | WidgetStack 管理多层 UI |

## 8. 渲染 & 视觉效果

| # | 难度 | 知识点 | 示例目标 |
|---|:---:|--------|----------|
| 8.1 | 🟢 | Material 基础 — 节点/常量/纹理 | 金属材质 + 发光材质 |
| 8.2 | 🟢 | Material Instance 动态参数 | BP 运行时改材质颜色/发光强度 |
| 8.3 | 🟢 | Niagara 粒子 — 基础发射器 | 篝火：火焰 + 烟雾 + 火星 |
| 8.4 | 🟡 | Material Function & Material Layer | 用 Material Layer 做可组合角色皮肤 |
| 8.5 | 🟡 | Niagara — GPU 粒子 + 碰撞 | 流星雨，粒子与地面碰撞产生火花 |
| 8.6 | 🟡 | Post Process Volume | 自定义后处理：风格化色调 + 景深 + 泛光 |
| 8.7 | 🔴 | Lumen & Nanite 实战 | 对比开启前后复杂场景表现 |
| 8.8 | 🔴 | Render Dependency Graph (RDG) | 自定义渲染 Pass：屏幕空间描边 |

## 9. 网络 & 多人游戏

| # | 难度 | 知识点 | 示例目标 |
|---|:---:|--------|----------|
| 9.1 | 🟢 | Replicated / RepNotify 属性复制 | 多人 Demo：血量服务器变化，客户端自动同步 |
| 9.2 | 🟢 | Server / Client / NetMulticast RPC | 聊天系统：客户端发 → 服务器广播 |
| 9.3 | 🟢 | Role / RemoteRole / Authority | HasAuthority() 区分 Server/Client |
| 9.4 | 🟡 | Owner / Instigator / AutonomousProxy | 网络射击：子弹服务器生成，客户端播放特效 |
| 9.5 | 🟡 | Replication Graph | 分析 + 优化大量 NPC 网络开销 |
| 9.6 | 🟡 | RPC 可靠性 (Reliable vs Unreliable) | 对比两种 RPC，理解适用场景 |
| 9.7 | 🔴 | Seamless Travel 关卡切换 | 无缝切换，数据不丢失 |
| 9.8 | 🔴 | Online Subsystem (EOS / Steam) | 接入在线子系统，Session 创建/加入 |

## 10. AI 系统

| # | 难度 | 知识点 | 示例目标 |
|---|:---:|--------|----------|
| 10.1| 🟢 | AIController + NavMesh + SimpleMoveTo | 追踪 AI，追到后停住 |
| 10.2| 🟢 | Behavior Tree + Blackboard | BT 实现：巡逻 → 发现 → 追击 → 攻击 |
| 10.3| 🟡 | BTTask / BTService / BTDecorator 自定义 | C++ 写自定义 Task + Service |
| 10.4| 🟡 | EQS (Environment Query System) | AI 寻找最佳掩体，避开玩家视线 |
| 10.5| 🟡 | AI Perception (视觉/听觉) | 听到脚步声转头，看到玩家追击 |
| 10.6| 🔴 | Mass AI / ZoneGraph | MassEntity 千人城市人群模拟 |
| 10.7| 🔴 | StateTree | StateTree 替代 BT 管理 AI 状态 |

## 11. 关卡设计 & 世界构建

| # | 难度 | 知识点 | 示例目标 |
|---|:---:|--------|----------|
| 11.1| 🟢 | Level Streaming (LoadStreamLevel) | 传送门：走进加载新关卡区域 |
| 11.2| 🟢 | DataLayer 基本用法 | DataLayer 控制白天/黑夜场景显隐 |
| 11.3| 🟡 | World Partition | 4km×4km 开放世界自动流式加载 |
| 11.4| 🟡 | World Partition — DataLayer + HLOD | 配置 HLOD，远处自动合并模型 |
| 11.5| 🟡 | Landscape + Foliage | 刷山脉/湖泊地形 + 自动种草/树 |
| 11.6| 🟡 | Water System | 河流 + 湖泊与地形无缝融合 |
| 11.7| 🔴 | PCG (Procedural Content Generation) | PCG 自动在森林撒石头 + 坡地生树 |
| 11.8| 🔴 | Custom Streaming Source | 视野 + 剧情触发区双重加载策略 |

## 12. 音频系统

| # | 难度 | 知识点 | 示例目标 |
|---|:---:|--------|----------|
| 12.1| 🟢 | SoundCue / SoundWave 基础 | 枪声 SoundCue：随机选 3 变体 + 随机音高 |
| 12.2| 🟢 | Attenuation / 空间化 | 3D 衰减：远小近大 + Doppler |
| 12.3| 🟡 | Audio Volume / Reverb | 进山洞切换混响，出来还原 |
| 12.4| 🟡 | MetaSounds | 动态引擎声：随速度变音高 |
| 12.5| 🔴 | Submix / Dynamic Sound Class | 按类别控制音量（SFX/Music/Voice） |

## 13. 编辑器扩展 & 工具开发

| # | 难度 | 知识点 | 示例目标 |
|---|:---:|--------|----------|
| 13.1| 🟡 | Editor Utility Widget | 编辑器工具面板：批量重命名资源 |
| 13.2| 🟡 | Asset Action (UAssetActionUtility) | 右键菜单：批量修改贴图压缩格式 |
| 13.3| 🟡 | IDetailCustomization | 自定义 Detail 面板：分组 + 条件显示 |
| 13.4| 🔴 | Asset Manager / PrimaryAssetType | 自定义 Asset Manager 管理武器加载 |
| 13.5| 🔴 | FMenuBuilder / FToolBarBuilder | 编辑器菜单栏加自定义按钮 |
| 13.6| 🔴 | DataTable / CSV 导入导出 | CSV ↔ DataTable 双向转换工具 |

## 14. 性能分析 & 优化

| # | 难度 | 知识点 | 示例目标 |
|---|:---:|--------|----------|
| 14.1| 🟢 | Stat 命令 (stat fps / stat game / stat unit) | 读懂 Game/Draw/GPU 三个耗时 |
| 14.2| 🟢 | Unreal Insights 基础使用 | 录制 Trace，分析帧耗时分布 |
| 14.3| 🟡 | LOD (StaticMesh / SkeletalMesh) | 角色 4 级 LOD，对比远近 DrawCall |
| 14.4| 🟡 | Culling (Distance / Frustum / Precomputed) | 配置 Distance Culling + 可见性体积 |
| 14.5| 🔴 | Blueprint Nativization / 蓝图性能 | Profiler 找最慢蓝图 → 转 C++ |
| 14.6| 🔴 | GC 优化 | TWeakObjectPtr vs UPROPERTY 的 GC 行为 |
| 14.7| 🔴 | 打包优化 (Chunking / Cook / Shader) | Pak 分块，首包 ≤ 500MB |

---

> **总计**：14 模块 / 108 知识点
> 🟢 入门 42 项 | 🟡 进阶 44 项 | 🔴 高级 22 项
