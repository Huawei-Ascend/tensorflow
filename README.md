# TF_Adapter

TF_Adapter致力于将昇腾AI处理器卓越的运算能力，便捷地提供给使用Tensorflow框架的开发者。
开发者只需安装TF_Adapter插件，并在现有Tensorflow脚本中添加少量配置，即可实现在昇腾AI处理器上加速自己的训练任务。

![framework](docs/framework.jpg)

您可以通过阅读 [TF_Adapter接口文档](https://support.huaweicloud.com/mprtg-A800_9000_9010/atlasprtg_13_0013.html) 获取更多使用细节。
## 安装
### 从源码安装

您可以从源代码构建 TF_Adapter 软件包并将其安装在昇腾AI处理器环境上。
> TF_Adapter 插件与 Tensorflow 有严格的匹配关系，从源码构建前，您需要确保已经正确安装了 [Tensorflow v1.15.0
>版本](https://www.tensorflow.org/install) 。

同时系统满足以下要求：
- GCC >= 7.3.0
- CMake >= 3.14.0
- [SWIG](http://www.swig.org/download.html)

#### 下载源码

```
git clone https://gitee.com/ascend/tensorflow.git
cd tensorflow
```

#### 配置安装环境
```BASH
./configure
```
默认情况下，执行上述命会弹出如下的交互式会话窗口
> 您的会话可能有所不同。
```BASH
Please specify the location of python with available tensorflow v1.15.0 installed. [Default is /home/jenkins/prj/venv/bin/python]
(You can make this quiet by set env [ASCEND_TARGET_PYTHON_BIN_PATH]):
```
此时，要求您输入安装了 Tensorflow v1.15.0 版本的python解释器路径，如果默认路径是正确的，直接回车，否则请输入正确的 python
解释器路径。
> 你可以通过设置 ASCEND_TARGET_PYTHON_BIN_PATH
>的环境变量，来抑制交互式窗口弹出，但是要确保路径是有效的，否则，仍然会要求您输入正确的 python 解释器路径。

键入后，会耗费几秒钟以确保您的输入是有效的，接着，会弹出下面的交互式窗口
```
Please specify the location of ascend. [Default is /usr/local/Ascend]
(You can make this quiet by set env [ASCEND_INSTALL_PATH]):
```
此时，要求您输入昇腾处理器开发套件的安装路径，如果默认路径是正确的，直接回车，否则请输入正确的昇腾处理器开发套件安装路径。

> 你可以通过设置ASCEND_INSTALL_PATH
>的环境变量，来抑制交互式窗口弹出，但是要确保路径是有效的，否则，仍然会要求您输入正确的昇腾处理器开发套件安装路径。

键入后，会接着弹出下面的交互式窗口
```
Please specify the location of swig. [Default is /usr/local/bin/swig]
(You can make this quiet by set env [SWIG_INSTALL_PATH]):
```
此时，要求您输入SWIG可执行文件路径，如果默认路径是正确的，直接回车，否则请输入正确的SWIG可执行文件路径。

> 你可以通过设置SWIG_INSTALL_PATH
>的环境变量，来抑制交互式窗口弹出，但是要确保路径是有效的，否则，仍然会要求您输入正确的SWIG可执行文件路径。


键入后，等待配置完成。
#### 配置cmake
> 根据您的网络状况，可能需要数分钟来下载TF_Adapter的依赖项目以完成配置。
```
mkdir build
cd build
cmake ..
```
#### 执行编译
> 您应当根据实际编译环境，设置合适的并发编译数以提升编译速度。
```BASH
make -j8
```
编译结束后，安装包会生成在
```
./dist/python/dist/npu_bridge-1.15.0-py3-none-any.whl
```
#### 安装
您可以使用 pip 安装 TF_Adapter 插件。
```
pip install ./dist/python/dist/npu_bridge-1.15.0-py3-none-any.whl
```
需要注意的是， 您应当保证安装路径与您编译时指定的 python
 解释器搜索路径是一致的。

## 贡献

欢迎参与贡献。

## Release Notes

Release Notes请参考[RELEASE](RELEASE.md).

## License

[Apache License 2.0](LICENSE)
