"""Inter-process communication using HCOM."""

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import os
import json
from ctypes import cdll
from ctypes import c_uint
from ctypes import byref

from tensorflow.python.platform import tf_logging as logging
from npu_bridge.estimator.npu import util as util_lib


class JobInfo:
    """Job information send by CSA."""

    def __init__(self,
                 job_id=0,
                 job_config=None,
                 heartbeat_time=-1,
                 region_id=None,
                 ak=None,
                 sk=None,
                 endpoint_url=None,
                 device_info=None,
                 rank_table_file=None,
                 restart_flag=0,
                 local_app_dir=None,
                 local_data_dir=None,
                 local_checkpoint_dir=None,
                 local_log_dir=None,
                 local_result_dir=None,
                 local_boot_file=None,
                 rank_size=1
                 ):
        """
        Constructs a JobInfo.
        Args:
        job_id: the unique identifier.
        heartbeat_time: the frequency that framework records the heartbeat.
        job_config: the configuration of the training task. It's a json string.
        region_id: the region id to access the OBS.
        ak: the ak to access the OBS.
        sk: the sk to access the OBS.
        endpoint_url: the host name to access the OBS.
        device_info: the device information of the training task. It's a json string.
        rank_table_file:  the communication routing information.
        restart_flag: the abnormal re-issued ID (0: Normally issued; 1: Abnormally re-issued).
        local_app_dir: the local path of the user script downloaded from OBS, for example: userfile/code/
        local_data_dir: the local path of the user data downloaded from OBS, for example: userfile/data/
        local_checkpoint_dir: the local path of the checkpoint file downloaded from OBS, for example: checkpoint/
        local_log_dir: the user-created log path, for example: userfile/log/
        local_result_dir: the user-created output file path, for example: userfile/result/
        local_boot_file: the local path of the user startup script, for example: userfile/code/boot.py
        rank_size: Rank size.
        """
        self._job_id = job_id
        self._job_config = job_config
        self._heartbeat_time = heartbeat_time
        self._region_id = region_id
        self._ak = ak
        self._sk = sk
        self._endpoint_url = endpoint_url
        self._device_info = device_info
        self._rank_table_file = rank_table_file
        self._restart_flag = restart_flag
        self._local_app_dir = local_app_dir
        self._local_data_dir = local_data_dir
        self._local_checkpoint_dir = local_checkpoint_dir
        self._local_log_dir = local_log_dir
        self._local_result_dir = local_result_dir
        self._local_boot_file = local_boot_file
        self._rank_size = rank_size


class JobConfig():
    """Job configuration."""

    def __init__(self, learning_rate=None, batch_size=None):
        """
        Constructs a JobConfig.
        Args:
        learning_rate: A Tensor or a floating point value.  The learning rate to use.
        batch_size: Integer, size of batches to return.
        """
        self._learning_rate = learning_rate
        self._batch_size = batch_size


class DeviceInfo():
    """Device information."""

    def __init__(self,
                 index="0",
                 server_id="123456",
                 dev_index=1):
        """
        Constructs a DeviceInfo.
        Args:
        index: the unique identifier.
        server_id: the server resource unique identifier, obtained from resource management.
        dev_index: the device serial number in AI server.
        """
        self._index = index
        self._server_id = server_id
        self._dev_index = dev_index
        self._root_rank = 0

    def is_master_node(self):
        """Determines whether the current node is the primary node."""
        return self._index == self._root_rank


class NPUBasics(object):
    """Wrapper class for the basic NPU API."""
    __instance = None
    __has_init = False
    def __new__(cls, file_name):
        if not cls.__instance:
            cls.__instance = object.__new__(cls)
        return cls.__instance

    def __init__(self, file_name):
        if not self.__has_init:
            self._job_info = self._read_job_info(file_name)
            self.__has_init = True

    @property
    def jobinfo(self):
        return self._job_info

    def size(self):
        """A function that returns the number of Tensorflow processes.
        Returns:
        An integer scalar containing the number of Tensorflow processes.
      """
        return self._job_info._rank_size

    def _read_job_info(self, file_name):
        """Read the job information.
        Args:
            file_name: it's a json file which contains the job info from CSA.
        Returns:
            The job information.
        """
        try:
            with open(file_name, 'r', encoding='UTF-8') as f:
                content = f.read()
                data = json.loads(content, encoding='UTF-8')

                # 1. Get the device_info and check it.
                device_info = data.get('device_info')
                util_lib.check_not_none(device_info, 'device_info')

                index = device_info.get('Index', None)
                util_lib.check_nonnegative_integer(index, 'Index')

                dev_index = device_info.get('dev_index', None)
                util_lib.check_nonnegative_integer(dev_index, 'dev_index')

                server_id = device_info.get('server_id', None)
                util_lib.check_not_none(device_info, 'server_id')

                # 2. Get the rank_table_file and check it.
                rank_table_file = data.get('rank_table_file', None)
                util_lib.check_not_none(rank_table_file, 'rank_table_file')

                # 3. Get the rank_size and check it.
                rank_size = data.get('rank_size', None)
                util_lib.check_positive_integer(rank_size, 'rank_size')

                # 4. Get the local_checkpoint_dir and check it.
                local_checkpoint_dir = data.get('local_checkpoint_dir', None)

                # 5. Init the JobInfo.
                device_info = DeviceInfo(index=str(index), server_id=server_id, dev_index=dev_index)
                job_info = JobInfo(device_info=device_info, rank_table_file=rank_table_file,
                                   local_checkpoint_dir=local_checkpoint_dir, rank_size=rank_size)
                return job_info
        except IOError:
            logging.warning('Warning:job config file does not exist')

            job_id = os.getenv('JOB_ID', "")
            if(job_id == ""):
                logging.error('Error:can not get job config from env')
                return None

            heartbeat = os.getenv('HEARTBEAT', "")

            rank_table_file = os.getenv('RANK_TABLE_FILE', "")

            identity = os.getenv('POD_NAME', "")
            if(identity == ""):
                identity = os.getenv('RANK_ID', "")

            dev_index = os.getenv('DEVICE_ID')
            if dev_index != None and dev_index.isdigit() and int(dev_index) <=7 and int(dev_index) >= 0:
                dev_index = int(dev_index)
            else:
                raise RuntimeError("DEVICE_ID environment variable should in [0, 7]")

            checkpoint_dir = os.getenv('LOCAL_CHECKPOINT_DIR', "")

            # cann't get rank_size from env, set to default 1
            rank_size = os.getenv('RANK_SIZE', 1)
            if(rank_size.isdigit() is False):
                print("set rank_size to default 1")
                rank_size = 1

            device_info = DeviceInfo(index=str(identity), server_id="192.168.1.1", dev_index=int(dev_index))
            job_info = JobInfo(job_id=job_id,
                               heartbeat_time=heartbeat,
                               device_info=device_info,
                               rank_table_file=rank_table_file,
                               local_checkpoint_dir=checkpoint_dir,
                               rank_size=int(rank_size)
                               )
        return job_info
