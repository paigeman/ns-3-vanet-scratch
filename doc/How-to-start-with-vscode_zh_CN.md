# 如何使用 Visual Studio Code 进行开发

## Linux 解决方案

### 环境

Ubuntu 22.04

Visual Studio Code 1.96.2

### 安装依赖

```shell
sudo apt -y install cmake
sudo apt -y install ninja-build
```

### 安装 Visual Studio Code

跟随参考文献2照着做即可。

### 下载 ns-3 仓库

```shell
git clone https://gitlab.com/nsnam/ns-3-dev.git
```

### 打开 Visual Studio Code 并进行配置

安装一些插件：

![image](resources/398676436-d560ad75-d65f-4a71-8106-804c38782189.png)

![image](resources/398676827-05bd49da-bd6d-4736-b1cf-78fcb2fdd4a9.png)

![image](resources/398689614-b8d83673-7328-470e-a1a6-865056909cad.png)

用 Visual Studio Code 打开 ns-3 的项目并配置它。

按 `Ctrl+shift+P` 键并选择：

![image](resources/398678439-9cbac1ef-825d-44b2-88d6-bc4e77e18c00.png)

选择你电脑上存在的工具链：

![image](resources/398678756-ea09e82f-5b05-416a-8083-7b37db196345.png)

然后你可以看到 CMake 开始工作。

![image](resources/398679444-5cd1f0a5-73f2-472a-88e8-7de6cc9c838d.png)

为了看到一些输出，设置 CMAKE_BUILD_TYPE 的值为 Debug ：

![image](resources/398684841-e3341350-93e2-4f31-9231-29c2d36c5bb0.png)

按 `Ctrl+shift+P` 键并选择：

![image](resources/398685681-ebaf0017-526a-4d26-ab98-8e5938131d6e.png)

为了构建官方示例，设置 NS3_EXAMPLES 的值为 ON ：

![image](resources/398686663-2c2241b7-081a-43d9-a387-fd5df2141388.png)

### 构建项目

![image](resources/398690058-eb7b8917-66bf-44cc-9169-8723f7ec14c4.png)

### 测试 ns-3

打开 `examples/tutorial/first.cc` 并运行：

![image](resources/398691283-4847e45e-75d4-4c4e-b77c-e6064c56aa4e.png)

![image](resources/398691410-84d48a5b-4572-442d-852d-69ac98e30cb0.png)

### 其它配置

Visual Studio Code 原来的 C/C++ IntelliSense engine 效果并不好。我们可以使用 clangd 插件作为代替。

![image](resources/399157078-cd67d5a1-edd8-4d10-9bdb-db94a6e81717.png)

你需要确保你的系统上安装了 clangd 。如果没有请尝试：

```shell
sudo apt -y install clangd
```

然后禁用原来的 Visual Studio Code C/C++ IntelliSense engine ：

![image](resources/399158018-d194cf2d-3152-458f-a827-d007e1eb15f0.png)

![image](resources/399158030-4dd6dc98-f58c-40d0-a76c-a0b432202623.png)

ns-3 官方提供了编码风格，你可以安装以下插件来启用并在设置里开启保存时格式化：

![image](resources/399158253-c49cf371-3a22-4d05-8f83-46c4c5784e3d.png)

![image](resources/399158270-942a1990-2390-41fc-bfe1-314b8dba5306.png)

![image](resources/399159463-66e01305-de48-4232-8821-b1d806e6895c.png)

### 在使用 CMake 时如何在 Visual Studio Code 里传递环境变量和程序参数给程序

往 `.vscode/settings.json` 里添加以下配置：

```json
    "cmake.debugConfig": {
        "args": [
            "--traceFile=/home/h3c/ns-3-dev/scratch/ns-3-vanet-scratch/area1ns2mobility.tcl",
            "--configFile=/home/h3c/ns-3-dev/scratch/ns-3-vanet-scratch/area1ns2config.tcl",
            "--rsuNum=4"
        ],
    },
    "cmake.environment": {
        "NS_LOG": "experiment=debug:VehicleApp=debug:RsuApp=debug"
    }
```

![image](resources/399158853-04d6be8f-eb1f-4253-ad6a-8fd6544f773e.png)

### 如何运行或调试程序

![image](resources/399165588-7c4a87a0-8132-4cdb-a530-fd1771d72d19.png)

### 参考文献

1. [4. Linux](https://www.nsnam.org/docs/release/3.42/installation/html/linux.html)
2. [Ubuntu 22.04安装Visual Studio Code(VS Code)](https://blog.csdn.net/u010044182/article/details/128977610)
3. [How to set CMake build configuration in VSCode?](https://stackoverflow.com/questions/73328916/how-to-set-cmake-build-configuration-in-vscode)
4. [关于在Vscode安装clangd的教程（分别在linux和windows）[很详细，很细节，很全！]【Windows端：缺少 language enginee的解决方法】](https://blog.csdn.net/qq_42764906/article/details/135541847)
5. [4.3. Working with CMake](https://www.nsnam.org/docs/manual/html/working-with-cmake.html)
6. [Configure CMake Tools settings](https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/cmake-settings.md)
7. [[vscode]使用cmake时将命令行参数传递给调试目标](https://blog.csdn.net/Strengthennn/article/details/136661436)
