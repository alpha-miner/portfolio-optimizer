# Portfolio - Optimizer

该项目在[Alpha-Mind](https://github.com/wegamekinglc/alpha-mind)中被使用。

## 依赖

* cmake >= 2.8.0
* jdk >= 1.5（可选，仅当需要Ipopt Java接口的时候需要安装）
* gtest >= 1.7（可选，仅当需要运行单元测试的时候）

### Linux
在Linux下，只需要如下命令即可完成除gtest以外依赖的安装：

```bash
sudo apt-get install cmake openjdk-8-jdk libgtest-dev
```

gtest在安装完成之后,还需要手动生产静态库文件。关于gtest在linux上的安装，请参考：[getting-started-with-google-test-on-ubuntu](https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/)

### Windows

已上的依赖需要手动在官网上下载安装，具体方法请参考各个软件官网。

* gtest 需要在``build_windows.bat``文件中设置``GTETS_ROOT``环境变量；


## 安装

* Linux

    ```bash
    ./build_linux.sh
    ```

    * 会在 portfolio-optimizer/portfolio-optimizer/bin 文件夹下生成可执行文件;
    * 在各个目录的lib下会生产需要动态链接库:*.so;

* Windows


    ```bash
    ./build_windows.bat
    ```

    * 会在 portfolio-optimizer/portfolio-optimizer/bin 文件夹下生成可执行文件;
    * 在各个目录的lib下会生产需要动态链接库:*.lib;


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
