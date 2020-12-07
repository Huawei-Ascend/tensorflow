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
import os
import shutil
import util_global

def before_clear():
    exit_folder = os.path.exists(util_global.get_value('output'))
    if exit_folder:
        shutil.rmtree(util_global.get_value('output'))
    exit_folder = os.path.exists(util_global.get_value('report'))
    if exit_folder:
        shutil.rmtree(util_global.get_value('report'))

def mkdir(path):
    folder = os.path.exists(path)
    if not folder:
        os.makedirs(path)

def mkdir_and_copyfile(srcfile, dstpath, file_name):
    mkdir(dstpath)
    shutil.copyfile(os.path.join(srcfile, file_name), os.path.join(dstpath, file_name))

def write_output_after_conver(out_file, dst_content):
    file = open(out_file, 'w')
    file.write(dst_content)
    file.close()

def write_report_after_conver(new_file_path, report_file, dst_content):
    mkdir(new_file_path)
    file = open(os.path.join(new_file_path, report_file), 'w')
    file.write(dst_content)
    file.close()

def write_report_terminator(content):
    report_path = util_global.get_value('report')
    for file in util_global.get_value('report_file'):
        if os.path.exists(os.path.join(report_path, file)):
            file = open(os.path.join(report_path, file), 'a')
            file.write(content)
            file.write("\r\n")
            file.write("\r\n")
            file.close()

def write_conver_report(content, file):
    report_path = util_global.get_value('report')
    mkdir(report_path)
    file = open(os.path.join(report_path, file), 'a')
    file.write(content)
    file.write("\r\n")
    file.close()

def abs_join(abs1, abs2):
    abs2 = os.fspath(abs2)
    abs2 = os.path.splitdrive(abs2)[1]
    abs2 = abs2.strip('\\/') or abs2
    return os.path.join(abs1, abs2)