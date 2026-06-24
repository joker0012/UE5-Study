# 0.4 第三方库集成 (ThirdParty + .lib 链接)

## 学习目标

把一个自写的 C 静态库集成到 UE 模块中，掌握 Build.cs 配置第三方库的标准流程。

## 核心概念

### 集成第三方库三步走

```
1. PublicIncludePaths  → 头文件搜索路径（让 #include "xxx.h" 找得到）
2. PublicAdditionalLibraries → 链接 .lib 文件
3. （动态库额外步骤）RuntimeDependencies → 打包时自动拷贝 .dll
```

### 目录约定

```
Plugin/Source/
├── ThirdParty/              ← 第三方库存放位置
│   └── SimpleMathLib/
│       ├── include/         ← 头文件
│       │   └── simple_math.h
│       ├── lib/             ← 编译产物
│       │   └── SimpleMathLib.lib
│       └── simple_math.c    ← 源码（可选，用于重新编译）
└── StudyPlugin/
    └── StudyPlugin.Build.cs ← 在这里配置链接
```

### 静态库 vs 动态库

| | 静态库 (.lib) | 动态库 (.dll + .lib) |
|---|---|---|
| 链接方式 | 代码直接编入模块 | 运行时加载 |
| Build.cs | `PublicAdditionalLibraries` | `PublicAdditionalLibraries` + `RuntimeDependencies` |
| 打包 | 自动包含 | 需额外配 `RuntimeDependencies` 拷贝 .dll |
| 本示例 | ✅ 用静态库 | — |

### C 接口注意事项

第三方 C 库头文件必须用 `extern "C"` 包裹，否则 C++ 编译器会 name-mangle 导致链接失败：

```c
#ifdef __cplusplus
extern "C" {
#endif
    int sm_add(int a, int b);
#ifdef __cplusplus
}
#endif
```

## 文件说明

| 文件 | 作用 |
|------|------|
| `simple_math.h` | C 库头文件 — 声明 extern "C" 接口 |
| `simple_math.c` | C 库实现 — 编译为 SimpleMathLib.lib |
| `StudyPlugin.Build.cs` | Build.cs — SetupSimpleMathLib() 方法配置链接 |
| `SimpleMathWrapper.h/.cpp` | 包装类 — 封装 C API 供 UE 代码调用 |

## Build.cs 关键代码

```csharp
private void SetupSimpleMathLib()
{
    string LibDir = Path.Combine(ModuleDirectory, "..", "ThirdParty", "SimpleMathLib");

    // 1. 头文件路径
    PublicIncludePaths.Add(Path.Combine(LibDir, "include"));

    // 2. 链接库
    string LibPath = Path.Combine(LibDir, "lib", "SimpleMathLib.lib");
    if (File.Exists(LibPath))
    {
        PublicAdditionalLibraries.Add(LibPath);
    }
}
```

## 编译 C 库的命令

```powershell
# 用 MSVC 编译 C 源码为 .lib
$cl = "C:\...\MSVC\14.38.33130\bin\Hostx64\x64\cl.exe"
& "$cl" /c /O2 /I"include" "simple_math.c" /Fo:"lib\simple_math.obj"
& "C:\...\lib.exe" /OUT:"lib\SimpleMathLib.lib" "lib\simple_math.obj"
```

## 预期 Log 输出

Play 时（BuildConfigTestActor 调用）：
```
LogTemp: ===== 第三方库调用验证 =====
LogSimpleMath: SimpleMathLib 测试结果:
    add(100, 200) = 300
    subtract(50, 18) = 32
    multiply(7, 8) = 56
    divide(100, 4) = 25.0
    factorial(5) = 120
    gcd(48, 36) = 12
    => 第三方 C 库集成成功！
```

## 实验建议

### 实验 1：删掉 .lib 看报错
- 删除 `lib/SimpleMathLib.lib`
- 重新编译 → LNK2019 链接错误 → 理解 `.lib` 是链接时必需的

### 实验 2：去掉 extern "C" 看报错
- 把 `simple_math.h` 中的 `extern "C"` 去掉
- 重新编译 C 库 + UE 项目 → LNK2019 → 理解 name mangling

### 实验 3：多平台支持
- 在 Build.cs 中根据 `Target.Platform` 选择不同平台的库：
  ```csharp
  if (Target.Platform == UnrealTargetPlatform.Win64)
      LibPath = Path.Combine(LibDir, "lib", "Win64", "SimpleMathLib.lib");
  else if (Target.Platform == UnrealTargetPlatform.Mac)
      LibPath = Path.Combine(LibDir, "lib", "Mac", "libSimpleMathLib.a");
  ```
