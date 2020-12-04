# Copyright 2020 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless REQUIRED by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================
import util_global
from file_op import write_conver_report

def log_info(msg):
    print(util_global.get_value('path') + ': ' + msg)

def log_info(lineno, msg, file):
    content = (util_global.get_value('path', '') + ':' + str(lineno) +
               ' change ' + util_global.get_value(msg)[1] +
               ' to ' + util_global.get_value(msg)[2])
    print(content)
    write_conver_report(content, file)

def log_success_report(lineno, msg):
    content = (util_global.get_value('path', '') + ':' + str(lineno) +
               ' "change ' + util_global.get_value(msg)[1] +
               ' to ' + util_global.get_value(msg)[2])
    print(content)
    write_conver_report(content, util_global.get_value('report_file')[0])

def log_migration_report(lineno, msg):
    content = (util_global.get_value('path', '') + ':' + str(lineno) + ' "' + msg +
               '" feature needs to be migrated manually, Please refer to the migration guide: ' +
               util_global.get_value(msg)[0])
    print(content)
    write_conver_report(content, util_global.get_value('report_file')[2])