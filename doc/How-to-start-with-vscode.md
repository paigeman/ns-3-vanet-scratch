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

Follow the references.

### Clone ns-3 project

```shell
git clone https://gitlab.com/nsnam/ns-3-dev.git
```

### Open Visual Studio Code and do some configurations

Install some plugins:

![image](https://github.com/user-attachments/assets/d560ad75-d65f-4a71-8106-804c38782189)

![image](https://github.com/user-attachments/assets/05bd49da-bd6d-4736-b1cf-78fcb2fdd4a9)

![image](https://github.com/user-attachments/assets/b8d83673-7328-470e-a1a6-865056909cad)

Open ns-3 project and configure it.

Press `Ctrl+shift+P` key and select:

![image](https://github.com/user-attachments/assets/9cbac1ef-825d-44b2-88d6-bc4e77e18c00)

Choose existed kit in your computer:

![image](https://github.com/user-attachments/assets/ea09e82f-5b05-416a-8083-7b37db196345)

Then you can see cmake begin to work.

![image](https://github.com/user-attachments/assets/5cd1f0a5-73f2-472a-88e8-7de6cc9c838d)

In order to see some output, set the CMAKE_BUILD_TYPE to Debug:

![image](https://github.com/user-attachments/assets/e3341350-93e2-4f31-9231-29c2d36c5bb0)

Press `Ctrl+shift+P` key and select:

![image](https://github.com/user-attachments/assets/ebaf0017-526a-4d26-ab98-8e5938131d6e)

In order to build the official examples, set the NS3_EXAMPLES to ON:

![image](https://github.com/user-attachments/assets/2c2241b7-081a-43d9-a387-fd5df2141388)

### Build the project

![image](https://github.com/user-attachments/assets/eb7b8917-66bf-44cc-9169-8723f7ec14c4)

### Test ns-3

Open `examples/tutorial/first.cc` , run it.

![image](https://github.com/user-attachments/assets/4847e45e-75d4-4c4e-b77c-e6064c56aa4e)

![image](https://github.com/user-attachments/assets/84d48a5b-4572-442d-852d-69ac98e30cb0)

### References

* [4. Linux](https://www.nsnam.org/docs/release/3.42/installation/html/linux.html)
* [Ubuntu 22.04安装Visual Studio Code(VS Code)](https://blog.csdn.net/u010044182/article/details/128977610)
* [How to set CMake build configuration in VSCode?](https://stackoverflow.com/questions/73328916/how-to-set-cmake-build-configuration-in-vscode)
