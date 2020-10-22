# Tensorflow Adapter For Ascend

[View English](README.en.md)

Tensorflow Adapter For Ascend（简称TF_Adapter）致力于将昇腾AI处理器卓越的运算能力，便捷地提供给使用Tensorflow框架的开发者。
开发者只需安装TF_Adapter插件，并在现有Tensorflow脚本中添加少量配置，即可实现在昇腾AI处理器上加速自己的训练任务。

![framework](docs/framework.jpg)

您可以通过阅读 [TF_Adapter接口文档](https://support.huaweicloud.com/mprtg-A800_9000_9010/atlasprtg_13_0013.html) 获取更多使用细节。
## 安装
### 从源码安装

您可以从源代码构建 TF_Adapter 软件包并将其安装在昇腾AI处理器环境上。
> TF_Adapter 插件与 Tensorflow 有严格的匹配关系，从源码构建前，您需要确保已经正确安装了 [Tensorflow v1.15.0
>版本](https://www.tensorflow.org/install) 。

同时系统满足以下要求：
- Linux OS
- GCC >= 7.3.0
- CMake >= 3.14.0
- [SWIG](http://www.swig.org/download.html)
- 配置昇腾处理器开发套件安装路径的环境变量D_LINK_PATH
  export D_LINK_PATH=安装路径（例如：/usr/local/Ascend）
  

#### 下载源码

```
git clone https://gitee.com/ascend/tensorflow.git
cd tensorflow
```

#### 执行脚本生成安装包
```
./build.sh
```

脚本执行成功后，会在output目录生成tfadapter.tar压缩文件

#### 安装插件包
解压tfadapter.tar文件，生成npu_bridge-1.15.0-py3-none-any.whl
使用 pip 安装 TF_Adapter 插件。
```
pip install npu_bridge-1.15.0-py3-none-any.whl
```
需要注意的是， 您应当保证安装路径与您编译时指定的 python
 解释器搜索路径是一致的。

## 贡献

欢迎参与贡献。

## Release Notes

Release Notes请参考[RELEASE](RELEASE.md).

## License

[Apache License 2.0](LICENSE)
