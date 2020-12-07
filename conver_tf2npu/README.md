# Tensorflow Adapter For Ascend

[View English](README.en.md)


## 使用指导
1、迁移脚本主要是将原生的tensorflow脚本自动迁移成NPU支持的脚本。

2、执行方式请直接下载脚本后执行，执行以下命令获取脚本的使用帮助：

   python3 main.py -h

   举例说明：

   python3 main.py -i /home/BERT -o /home/out -r /home/report

   其中：

    main.py  --这个是主函数

    /home/BERT --这个是被迁移的脚本路径

    /home/out  --这个是迁移后的脚本路径

    /home/report --这个是迁移过程的迁移报告

           迁移报告分三种：

               1、success_report.txt  --这个是迁移成功的结果

                   例如：下面的报告的内容表示把run_ner.py的第522行的hvd.ran迁移为NPU的get_ran_id

                   /home/BERT/run_ner.py:522 "change hvd.rank to get_rank_id"
                   
               2、failed_report.txt   --这个是迁移失败的结果

               3、need_migration_doc.txt   --这个是需要参考迁移指导进行手动迁移的

  
3、目前支持自动迁移的功能列表

    Tensorflow函数 --> 迁移后的NPU函数

    "tf.gelu" --> "npu_unary_ops.gelu"

    "tf.nn.dropout" --> "npu_ops.dropout"

    "hvd.init" --> "None"

    "hvd.DistributedOptimizer" --> "NPUDistributedOptimizer"

    "hvd.rank" --> "get_rank_id"

    "hvd.local_rank" --> "get_local_rank_id"

    "hvd.size" --> "get_rank_size"

    "BroadcastGlobalVariablesHook" --> "None"

    "dataset.shard(xxx, xxx)" --> "dataset.shard(get_rank_size(), get_rank_id())"

    "tf.estimator.EstimatorSpec" --> "NPUEstimatorSpec"

    "tf.estimator.RunConfig" --> "NPURunConfig"

    "tf.estimator.Estimator" --> "NPUEstimator"

    "batch(xxx)" --> "batch(xxx, drop_remainder=True)"

    "map_and_batch(xxx)" --> "map_and_batch(xxx, drop_remainder=True)"

    "tf.device(xxx)" --> "tf.device(/cpu:0)"

## 贡献

欢迎参与贡献。

## Release Notes

Release Notes请参考[RELEASE](RELEASE.md).

## License

[Apache License 2.0](LICENSE)
