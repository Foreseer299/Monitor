<div align="center">
  <img src="app.png" alt="Monitor Icon" width="96" />
  <h1>Monitor</h1>
  <p><strong>极轻量 Windows 桌面透明硬件监控浮窗 · Ultra-lightweight Native Win32/Direct2D Hardware Monitor Overlay</strong></p>
  <p>
    单文件免安装 · 原生 DirectWrite 次像素渲染 · 鼠标穿透不遮挡 · 丰富外观与文字自定义<br/>
    Single-file standalone executable · Native Direct2D/DirectWrite · Click-through · Rich customization
  </p>
</div>

---

## ✨ 特性亮点 | Features

### 1. 默认极简，开箱即用
- **原生 Win32 + Direct2D + DirectWrite**：无 Electron / .NET / 运行库依赖，静态编译单文件仅约 **356 KB**，任务管理器专用内存仅约 **1.7 MB**。
- **极致低占用优化**：支持脏检测跳帧（数值与样式未变时跳过重绘与桌面合成）、离屏 DIB 位图与文字宽度缓存（零每秒 GDI/COM 反复分配），以及启动/弹窗后自动内存工作集修剪。
- **高 DPI 锐利渲染**：支持 `Per-Monitor DPI v2`，在 100% / 125% / 150% / 200% 缩放屏下文字边缘依然清晰锐利。
- **完全穿透不挡操作**：默认置顶、透明无底板、鼠标点击穿透，不抢占焦点，不影响游戏与办公。
- **固定列宽不晃动**：所有数据槽位按最大字符宽度锁定，数字变化时整行纹丝不动。

### 2. 丰富的监控与显示选项（托盘右键菜单随时勾选）
- **核心硬件指标（默认开启）**：
  - `CPU` 使用率（原生 `GetSystemTimes` 零开销采样）
  - `GPU` 使用率与温度（通过 `nvml.dll` 动态读取 NVIDIA 显卡数据，无 N 卡自动优雅降级显示 `--`）
  - `RAM` 内存使用率
- **扩展可选指标（按需勾选）**：
  - `GPU 显存 (VRAM)`（已用 / 总量 GB）
  - `GPU 实时功耗 (W)`
  - `RAM 已用 / 总量 (GB)`
  - `实时网速 (↓ / ↑)`（基于 Windows IP Helper `GetIfTable2`）
  - `当前时间 (HH:MM:SS)`
  - `温度 °C 单位显示开关`

### 3. 文字、字体与标签样式自定义
- **10 款精选预设字体**（`Segoe UI Variable Text`、`Segoe UI`、`Bahnschrift`、`Constantia`、`Cascadia Mono`、`Cascadia Code`、`Consolas`、`Microsoft YaHei UI`、`Calibri`、`Arial`）+ **调用系统字体选择器**任选本机已装字体。
- **8 档字号**（`10 / 11 / 12 / 13(默认) / 14 / 15 / 18 / 22 px`）+ **4 档字重**（`Light / Normal / SemiBold / Bold`）。
- **6 种标签风格**：
  - 标准英文：`CPU: 12%`（默认）
  - 紧凑简写：`C:12%`
  - 中文标签：`处理器 12%`
  - 无冒号：`CPU 12%`
  - 纯数值：`12%`
  - 方括号：`[CPU 12%]`

### 4. 外观美化与色彩定制
- **8 套预设文字主题色**（纯白、墨黑、冰霜银灰、赛博青、极客荧光绿、暖琥珀金、樱花粉、幻夜紫）+ **系统调色盘任意颜色**。
- **分项多彩模式**：CPU / GPU / 温度 / 显存 / 功耗 / 内存 / 网速 / 时钟各自独立配色。
- **高负载自动变色**：占用 ≥70%（或温度 ≥75°C）自动呈橙黄提示，≥90%（或温度 ≥85°C）呈红色警示。
- **5 种背景底板**：完全透明（默认）、暗色磨砂胶囊、浅色玻璃胶囊、极简圆角暗底条、细描边霓虹胶囊。
- **4 种阴影与描边**：`1px` 锐利微阴影（默认）、双层柔和强阴影、四向细描边、无阴影。
- **5 档整体不透明度**：`100% / 85% / 70% / 55% / 40%`。

### 5. 布局、交互与系统集成
- **3 种排列方式**：单行横排（默认）、双行紧凑、竖排列表，搭配 **3 档列间距**（紧凑 / 标准 / 宽松）。
- **6 处屏幕快捷停靠** + **解锁鼠标拖动任意定位**（支持多显示器坐标保存）。
- **5 档刷新频率**（`0.25s / 0.5s / 1.0s(默认) / 2.0s / 3.0s`）。
- **全屏应用自动隐藏**、**开机自启动**、**简体中文 / English 菜单即时切换**。
- **配置自动持久化**：所有设置自动保存至注册表 `HKCU\Software\Monitor`，并支持一键「恢复默认设置」。

---

## 🚀 快速使用 | Quick Start

1. 前往 [GitHub Releases](https://github.com/Foreseer299/Monitor/releases/latest) 下载最新的 `Monitor.exe` 单文件，双击即可运行（无需安装任何运行库）。
2. 在系统托盘区右键（或左键）点击 Monitor 图标，即可打开全部自定义设置菜单。

---

## 🛠️ 从源码编译 | Build from Source

仓库自带 [`build.bat`](build.bat)，安装 **MSYS2 (UCRT64)** 并具备 `g++` 与 `windres` 后，在项目根目录直接运行：

```bat
build.bat
```

或手动使用 UCRT64 `g++` 编译（全静态链接 C/C++ 运行库，零额外 DLL 依赖）：

```bat
windres resource.rc -O coff -o resource.res
g++ -O2 -s -Wall -Wextra -Wno-unused-parameter -mwindows Monitor.cpp resource.res -o Monitor.exe ^
    -static -ld2d1 -ldwrite -lgdi32 -luser32 -lshell32 -ladvapi32 -lcomdlg32 -liphlpapi -lws2_32 -lole32
```

---

## 📁 文件结构 | Project Files

- [`Monitor.cpp`](Monitor.cpp) — 全部核心实现（监控采集、Direct2D 渲染、配置持久化、双语托盘菜单）
- [`app.ico`](app.ico) / [`app.png`](app.png) — 稀疏低多边形风格应用与托盘图标
- [`resource.rc`](resource.rc) — Windows 图标资源定义
- [`build.bat`](build.bat) — 一键编译脚本

> 预编译好的单文件可执行程序 `Monitor.exe` 请在 [Releases](https://github.com/Foreseer299/Monitor/releases/latest) 页面下载。

---

## 📄 开源协议 | License

本项目基于 [MIT License](LICENSE) 开源。
