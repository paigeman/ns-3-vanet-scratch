# How to start with Visual Studio Code

## Linux Solution

### Environment

Ubuntu 22.04

Visual Studio Code 1.96.2

### Install the requirements

```shell
sudo apt -y install cmake
sudo apt -y install ninja-build
```

### Install Visual Studio Code

Follow the Ref. 2.

### Clone ns-3 project

```shell
git clone https://gitlab.com/nsnam/ns-3-dev.git
```

### Open Visual Studio Code and do some configurations

Install some plugins:

![image](resources/398676436-d560ad75-d65f-4a71-8106-804c38782189.png)

![image](resources/398676827-05bd49da-bd6d-4736-b1cf-78fcb2fdd4a9.png)

![image](resources/398689614-b8d83673-7328-470e-a1a6-865056909cad.png)

Open ns-3 project and configure it.

Press `Ctrl+shift+P` key and select:

![image](resources/398678439-9cbac1ef-825d-44b2-88d6-bc4e77e18c00.png)

Choose existed kit in your computer:

![image](resources/398678756-ea09e82f-5b05-416a-8083-7b37db196345.png)

Then you can see cmake begin to work.

![image](resources/398679444-5cd1f0a5-73f2-472a-88e8-7de6cc9c838d.png)

In order to see some output, set the CMAKE_BUILD_TYPE to Debug:

![image](resources/398684841-e3341350-93e2-4f31-9231-29c2d36c5bb0.png)

Press `Ctrl+shift+P` key and select:

![image](resources/398685681-ebaf0017-526a-4d26-ab98-8e5938131d6e.png)

In order to build the official examples, set the NS3_EXAMPLES to ON:

![image](https://github.com/user-attachments/assets/2c2241b7-081a-43d9-a387-fd5df2141388)

### Build the project

![image](https://github.com/user-attachments/assets/eb7b8917-66bf-44cc-9169-8723f7ec14c4)

### Test ns-3

Open `examples/tutorial/first.cc` , run it.

![image](https://github.com/user-attachments/assets/4847e45e-75d4-4c4e-b77c-e6064c56aa4e)

![image](https://github.com/user-attachments/assets/84d48a5b-4572-442d-852d-69ac98e30cb0)

### Other configurations

The performance of the original Visual Studio Code C/C++ IntelliSense engine is not very good. We can use the clangd plugin as an alternative.

![image](https://github.com/user-attachments/assets/cd67d5a1-edd8-4d10-9bdb-db94a6e81717)

You also need to ensure that clangd is installed on your system. If not, try:

```shell
sudo apt -y install clangd
```

Then, disable the original Visual Studio Code C/C++ IntelliSense engine:

![image](https://github.com/user-attachments/assets/d194cf2d-3152-458f-a827-d007e1eb15f0)

![image](https://github.com/user-attachments/assets/4dd6dc98-f58c-40d0-a76c-a0b432202623)

The ns-3 official offers coding styles, you can install some plugins:

![image](https://github.com/user-attachments/assets/c49cf371-3a22-4d05-8f83-46c4c5784e3d)

![image](https://github.com/user-attachments/assets/942a1990-2390-41fc-bfe1-314b8dba5306)

![image](https://github.com/user-attachments/assets/66e01305-de48-4232-8821-b1d806e6895c)

### How to pass environment variables and program arguments to the program in Visual Studio Code while using CMake

Add the following configuration to `.vscode/settings.json` :

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

![image](https://github.com/user-attachments/assets/04d6be8f-eb1f-4253-ad6a-8fd6544f773e)

### References

1 [4. Linux](https://www.nsnam.org/docs/release/3.42/installation/html/linux.html)
2. [Ubuntu 22.04安装Visual Studio Code(VS Code)](https://blog.csdn.net/u010044182/article/details/128977610)
3. [How to set CMake build configuration in VSCode?](https://stackoverflow.com/questions/73328916/how-to-set-cmake-build-configuration-in-vscode)
4. [关于在Vscode安装clangd的教程（分别在linux和windows）[很详细，很细节，很全！]【Windows端：缺少 language enginee的解决方法】](https://blog.csdn.net/qq_42764906/article/details/135541847)
