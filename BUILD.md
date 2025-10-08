# 构建指南

## 快速开始

### 方式一：使用 Qt Creator（推荐新手）

1. 安装 Qt Creator 和 Qt 5.x SDK
2. 打开 Qt Creator
3. 点击 "文件" → "打开文件或项目"
4. 选择 `ScheduleManager.pro` 文件
5. 选择一个构建套件（Kit），点击 "配置项目"
6. 点击左下角的绿色 "运行" 按钮（或按 Ctrl+R / Cmd+R）

### 方式二：命令行构建（Linux/macOS）

```bash
# 进入项目目录
cd qt5

# 生成 Makefile
qmake ScheduleManager.pro

# 编译
make

# 运行
./bin/ScheduleManager
```

### 方式三：命令行构建（Windows）

```cmd
:: 进入项目目录
cd qt5

:: 生成 Makefile（使用 MSVC）
qmake ScheduleManager.pro

:: 编译（使用 MSVC）
nmake

:: 或者如果使用 MinGW
mingw32-make

:: 运行
bin\ScheduleManager.exe
```

## 环境要求

### 必需

- **Qt**: 5.9 或更高版本（推荐 5.12+）
  - 需要安装 Qt Widgets 模块
- **C++ 编译器**: 支持 C++11 标准
  - Windows: MSVC 2015+ 或 MinGW
  - macOS: Xcode Command Line Tools (Clang)
  - Linux: GCC 5.0+ 或 Clang 3.4+

### 可选

- **Qt Creator**: 最新版本（推荐用于开发）

## 安装 Qt

### Windows

1. 从 [Qt官网](https://www.qt.io/download-qt-installer) 下载在线安装器
2. 运行安装器，选择以下组件：
   - Qt 5.15.2（或最新的5.x版本）
   - MinGW 编译器（或使用已安装的 MSVC）
   - Qt Creator
3. 按照向导完成安装

### macOS

```bash
# 使用 Homebrew 安装
brew install qt@5

# 将 Qt 添加到 PATH
echo 'export PATH="/usr/local/opt/qt@5/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```

### Linux (Ubuntu/Debian)

```bash
# 安装 Qt 开发工具
sudo apt-get update
sudo apt-get install qt5-default qtcreator build-essential
```

### Linux (Fedora/RHEL)

```bash
# 安装 Qt 开发工具
sudo dnf install qt5-qtbase-devel qt-creator gcc-c++
```

## 构建配置

### Debug 模式

```bash
qmake CONFIG+=debug ScheduleManager.pro
make
```

### Release 模式

```bash
qmake CONFIG+=release ScheduleManager.pro
make
```

### 清理构建

```bash
make clean
# 或者
make distclean  # 完全清理，包括 Makefile
```

## 常见问题

### Q: qmake 命令找不到

**A:** 确保 Qt 的 bin 目录在系统 PATH 中。

Windows 示例：
```cmd
set PATH=C:\Qt\5.15.2\mingw81_64\bin;%PATH%
```

Linux/macOS 示例：
```bash
export PATH=/usr/local/Qt-5.15.2/bin:$PATH
```

### Q: 编译时报错 "cannot find -lQt5Widgets"

**A:** Qt 库未正确链接。检查：
1. Qt 是否正确安装
2. qmake 版本是否正确（运行 `qmake --version`）
3. 尝试清理后重新构建

### Q: Windows 上编译成功但运行时缺少 DLL

**A:** 需要将 Qt 的 DLL 复制到程序目录，或使用 `windeployqt`：

```cmd
cd bin
windeployqt ScheduleManager.exe
```

### Q: macOS 上提示 "Qt5Widgets not found"

**A:** 设置 Qt 安装路径：

```bash
export Qt5_DIR=/usr/local/opt/qt@5/lib/cmake/Qt5
qmake ScheduleManager.pro
make
```

### Q: Linux 上编译时出现 "Project ERROR: Unknown module(s) in QT: widgets"

**A:** 安装 Qt Widgets 开发包：

```bash
# Ubuntu/Debian
sudo apt-get install qtbase5-dev

# Fedora
sudo dnf install qt5-qtbase-devel
```

## 项目结构说明

编译后的目录结构：

```
qt5/
├── bin/                    # 可执行文件输出目录
│   └── ScheduleManager     # 编译后的可执行文件
├── build/                  # 构建临时文件
│   ├── obj/               # 目标文件
│   ├── moc/               # MOC 生成的文件
│   └── ...
├── datastructure/         # 源代码：数据结构
├── modules/               # 源代码：业务逻辑
├── ui/                    # 源代码：界面组件
├── example_data/          # 示例CSV文件
├── data_storage/          # 运行时数据（首次运行时创建）
├── main.cpp
└── ScheduleManager.pro    # Qt项目文件
```

## 开发工作流

### 修改代码后重新编译

```bash
make
```

### 添加新的源文件

1. 在对应目录创建 `.h` 和 `.cpp` 文件
2. 编辑 `ScheduleManager.pro`，在 `SOURCES` 和 `HEADERS` 中添加新文件
3. 重新运行 `qmake` 和 `make`

```bash
qmake ScheduleManager.pro
make
```

### 调试

使用 Qt Creator 的调试功能，或者：

```bash
# GDB (Linux/macOS)
gdb ./bin/ScheduleManager

# LLDB (macOS)
lldb ./bin/ScheduleManager
```

## 打包发布

### Windows

```cmd
:: 创建发布目录
mkdir release_package
copy bin\ScheduleManager.exe release_package\

:: 复制 Qt 依赖
cd release_package
windeployqt ScheduleManager.exe

:: 复制示例数据
xcopy ..\example_data example_data\ /E /I
```

### macOS

```bash
# 使用 macdeployqt
macdeployqt bin/ScheduleManager.app

# 创建 DMG（可选）
hdiutil create -volname ScheduleManager -srcfolder bin/ScheduleManager.app -ov -format UDZO ScheduleManager.dmg
```

### Linux

```bash
# 创建发布目录
mkdir -p release_package
cp bin/ScheduleManager release_package/

# 复制示例数据
cp -r example_data release_package/

# 创建启动脚本
cat > release_package/run.sh << 'EOF'
#!/bin/bash
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export LD_LIBRARY_PATH=$DIR/lib:$LD_LIBRARY_PATH
"$DIR/ScheduleManager"
EOF
chmod +x release_package/run.sh
```

## 技术支持

如遇到构建问题，请检查：
1. Qt 和编译器版本是否兼容
2. 所有依赖是否已安装
3. 环境变量是否正确设置
4. 项目文件 (.pro) 是否完整

---

祝您构建成功！

