# Tensorflow Adapter For Ascend

[View English](README.en.md)

Tensorflow Adapter For Ascend（简称TF_Adapter）致力于将昇腾AI处理器卓越的运算能力，便捷地提供给使用Tensorflow框架的开发者。
开发者只需安装TF_Adapter插件，并在现有Tensorflow脚本中添加少量配置，即可实现在昇腾AI处理器上加速自己的训练任务。

![tfadapter](https://images.gitee.com/uploads/images/2020/1027/094640_8f305b88_8175427.jpeg "framework.jpg")

您可以通过阅读 [TF_Adapter接口文档](https://support.huaweicloud.com/mprtg-A800_9000_9010/atlasprtg_13_0013.html) 获取更多使用细节。

## 使用指导
1、迁移脚本主要是将原生的tensorflow脚本自动迁移成NPU支持的脚本。
2、执行方式请直接下载脚本后执行，执行以下命令获取脚本的帮忙：
   python3 main.py -h
  迁移完成后，会在您配置的目录生成迁移后的脚本和迁移报告
3、目前支持的功能列表，请参考资料文档

## 贡献

欢迎参与贡献。

## Release Notes

Release Notes请参考[RELEASE](RELEASE.md).

## License

[Apache License 2.0](LICENSE)
