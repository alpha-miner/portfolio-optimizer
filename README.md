# Portfolio - Optimizer

## 依赖

* cmake >= 2.8.0
* boost >= 1.58.0
* jdk >= 1.5（可选，仅当需要Ipopt Java接口的时候需要安装）
* gtest >= 1.7

在Linux下，只需要如下命令即可完成依赖的安装：

```bash
sudo apt-get install cmake libboost-all-dev openjdk-8-jdk
```

## 安装

* Linux

    ```bash
    ./build_linux.sh
    ```

    会在 portfolio-optimizer/portfolio-optimizer/bin 文件夹下生成可执行文件。

* Windows


    ```bash
    ./build_windows.bat
    ```

    进入 portfolio-optimizer/portfolio-optimizer/build 文件夹下由Visual Studio项目文件。


### Ipopt Java接口

在Linux构建过程的最后，我们制作了Ipopt的java接口，在Ipopt/Ipopt/contrib/JavaInterface目录下。请确保JAVA_HOME变量已经正确的配置。下面我们列出一些常见的报错信息，用户可以根据情况修正：

* ``/bin/bash: /bin/javah: No such file or directory``
    
    没有正确配置JAVA_HOME变量。请将JAVA_HOME指向本地的JAVA sdk安装目录；

* ``../include/jni.h:45:20: fatal error: jni_md.h: No such file or directory``

    无法找到头文件``jni_md.h``。这个处理的常见办法是将该文件所在目录添加至CPLUS_INCLUDE_PATH:

    ```bash
    export CPLUS_INCLUDE_PATH=$JAVA_HOME/include/linux
    ```

编译生产完成之后，具体使用请见：[Ipopt Java Interface](https://www.coin-or.org/Ipopt/documentation/node16.html)