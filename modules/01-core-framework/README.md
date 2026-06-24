# 模块 1: 核心框架 & Gameplay 架构

UE 一切之基石。理解 UObject → AActor → Component 的层级关系和 GameMode/GameState/PlayerController/Pawn 的职责划分。

## 子目录

| # | 目录 | 知识点 |
|---|------|--------|
| 01 | `uobject-reflection/` | UObject & UFUNCTION / UPROPERTY 宏 |
| 02 | `actor-lifecycle/` | AActor 生命周期 |
| 03 | `component-types/` | UActorComponent vs USceneComponent |
| 04 | `pawn-character/` | APawn & ACharacter |
| 05 | `player-controller-state/` | APlayerController & APlayerState |
| 06 | `game-mode/` | AGameModeBase vs AGameMode |
| 07 | `game-state/` | AGameStateBase |
| 08 | `subsystem/` | UGameInstanceSubsystem / UWorldSubsystem |
| 09 | `data-asset/` | DataAsset (UPrimaryDataAsset) |
| 10 | `interface/` | Interface (UInterface) |
| 11 | `delegates/` | Delegate (单播/多播/动态) |
| 12 | `timer/` | FTimerHandle / Timer |
| 13 | `async-load/` | FStreamableManager 异步加载 |
| 14 | `object-pool/` | ObjectPool 对象池 |
| 15 | `save-game/` | SaveGame 系统 |
| 16 | `soft-references/` | 软引用 / 硬引用 / TSoftObjectPtr |
