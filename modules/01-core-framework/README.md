# 模块 1: 核心框架 & Gameplay 架构

UE 一切之基石。理解 UObject → AActor → Component 的层级关系和 GameMode/GameState/PlayerController/Pawn 的职责划分。

## 子目录

| # | 目录 | 知识点 |
|---|------|--------|
| 01 | `uobject-reflection/` | UObject & UFUNCTION / UPROPERTY 宏 |
| 02 | `debug-macros/` | UE_LOG / check / ensure 调试宏体系 |
| 03 | `string-types/` | FName / FString / FText 区别与转换 |
| 04 | `actor-lifecycle/` | AActor 生命周期 |
| 05 | `component-types/` | UActorComponent vs USceneComponent |
| 06 | `pawn-character/` | APawn & ACharacter |
| 07 | `player-controller-state/` | APlayerController & APlayerState |
| 08 | `game-mode/` | AGameModeBase vs AGameMode |
| 09 | `game-state/` | AGameStateBase |
| 10 | `subsystem/` | UGameInstanceSubsystem / UWorldSubsystem |
| 11 | `data-asset/` | DataAsset (UPrimaryDataAsset) |
| 12 | `interface/` | Interface (UInterface) |
| 13 | `delegates/` | Delegate (单播/多播/动态) |
| 14 | `timer/` | FTimerHandle / Timer |
| 15 | `async-load/` | FStreamableManager 异步加载 |
| 16 | `object-pool/` | ObjectPool 对象池 |
| 17 | `save-game/` | SaveGame 系统 |
| 18 | `soft-references/` | 软引用 / 硬引用 / TSoftObjectPtr |
