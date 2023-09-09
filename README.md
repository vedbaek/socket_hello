# build
```
cmake -B build
cd build
make
```

```
-G 或 --generator：指定生成器。例如，-G "Unix Makefiles"表示使用Unix Makefile作为生成器，-G "Visual Studio 16 2019"表示使用Visual Studio 2019作为生成器。可以通过 cmake --help 查看系统支持的所有生成器。

-C 或 --cache-entry：设置缓存项的值。可以使用该选项设置某个缓存项的值，例如：-C CMAKE_BUILD_TYPE=Release 设置 CMAKE_BUILD_TYPE 缓存项的值为 Release。

-D 或 --define：定义变量。通过该选项可以定义或覆盖CMake变量的值。例如，-D VAR_NAME=value 定义一个名为 VAR_NAME 的变量，并将其值设置为 value。

-B 或 --build-dir：指定构建目录。可以使用该选项指定一个不同于源代码目录的构建目录。例如，-B build 将构建目录设置为 build。

-S 或 --source-dir：指定源代码目录。可以使用该选项指定源代码目录。例如，-S /path/to/source 将源代码目录设置为 /path/to/source。

--build：构建项目。该选项用于执行构建过程。例如：cmake --build . 在当前目录执行构建。

--install：安装项目。该选项用于执行项目的安装步骤，将生成的可执行文件、库文件和其他资源复制到指定位置。

--config：指定配置类型。在多配置生成器中，可以使用该选项指定所需的配置类型。例如，--config Debug 指定 Debug 配置类型进行构建。

--target：构建指定的目标。使用该选项可以指定只构建某个特定目标，而不是整个项目。例如，--target myapp 构建名为 myapp 的目标。
```
