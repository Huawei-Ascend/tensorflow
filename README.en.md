# TF_Adapter

[查看中文](README.md)

TF_Adapter is committed to providing the outstanding computing power of Shengteng AI processor to developers who use the Tensorflow framework.
Developers only need to install the TF_Adapter plug-in and add a small amount of configuration to the existing Tensorflow script to accelerate their training tasks on the Shengteng AI processor.

![tfadapter](https://images.gitee.com/uploads/images/2020/1027/094640_8f305b88_8175427.jpeg "framework.jpg")

You can read [TF_Adapter Interface](https://support.huaweicloud.com/mprtg-A800_9000_9010/atlasprtg_13_0013.html) for more details。
## Installation Guide
### Building from source

You can build the TF_Adapter software package from the source code and install it on the Shengteng AI processor environment.
> The TF_Adapter plug-in has a strict matching relationship with Tensorflow. Before building from source code, you need to ensure that it has been installed correctly [Tensorflow v1.15.0
>版本](https://www.tensorflow.org/install/pip) 。

You may also build GraphEngine from source. To build GraphEngine, please make sure that you have access to an Ascend 910 environment as compiling environment, and make sure that following software requirements are fulfilled.
- Linux OS
- GCC >= 7.3.0
- CMake >= 3.14.0
- [SWIG](http://www.swig.org/download.html)

#### Download
```
git clone https://gitee.com/ascend/tensorflow.git
cd tensorflow
```

#### Execute script to generate installation package
```
chmod +x build.sh
./build.sh
```


After the script is successfully executed, a compressed file of tfadapter.tar will be generated in the output directory

#### Install
Unzip the tfadapter.tar file to generate npu_bridge-1.15.0-py3-none-any.whl,
After you can install the TF_Adapter plug-in using pip.
```
pip install ./dist/python/dist/npu_bridge-1.15.0-py3-none-any.whl
```

It should be noted that you should ensure that the installation path is the same as the python you specified when compiling
 The interpreter search path is consistent.

## Contributing

Welcome to contribute.

## Release Notes

Release Notes, see our [RELEASE](RELEASE.md).

## License

[Apache License 2.0](LICENSE)
