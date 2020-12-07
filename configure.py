# Copyright 2019-2020 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import os
import subprocess
import sys
try:
  from shutil import which
except ImportError:
  from distutils.spawn import find_executable as which

_COMPAT_TENSORFLOW_VERSION = "1.15.0"
_COMPAT_PYTHON_VERSION = "Python 3.7"
_COMPAT_SWIG_VERSION = "SWIG Version "
_ASCEND_INSTALL_PATH_ENV = "ASCEND_CUSTOM_PATH"



def run_command(cmd):
  output = subprocess.check_output(cmd)
  return output.decode('UTF-8').strip()


def get_input(question):
  try:
    try:
      answer = raw_input(question)
    except NameError:
      answer = input(question)
  except EOFError:
    answer = ''
  return answer

def real_config_path(file):
  return os.path.join("tools", file)

def setup_python():
  """Get python install path."""
  default_python_bin_path = which('python3')
  custom_python_bin_path = ''
  ask_python_bin_path = ''
  if default_python_bin_path:
    custom_python_bin_path = default_python_bin_path
    compile_args = run_command([
      custom_python_bin_path, '--version'])
    if not _COMPAT_PYTHON_VERSION in compile_args:
      print('Invalid default python version: %s, only support Python 3.7.' % compile_args)
      ask_python_bin_path = ('Please specify the location of python with valid '
                             'tensorflow 1.15.0 site-packages installed. [Default '
                             'is %s]\n(Please enter the correct python path: ') % default_python_bin_path
      custom_python_bin_path = ''
  else:
    ask_python_bin_path = ('Please specify the location of python with valid '
                           'tensorflow 1.15.0 site-packages installed. [Default '
                           'is %s]\n(Please enter the correct python path: ') % default_python_bin_path

  while True:
    if not custom_python_bin_path:
      python_bin_path = get_input(ask_python_bin_path)
    else:
      python_bin_path = custom_python_bin_path
      custom_python_bin_path = None
    if not python_bin_path:
      python_bin_path = default_python_bin_path
      pass
    # Check if the path is valid
    if os.path.isfile(python_bin_path) and os.access(python_bin_path, os.X_OK):
      pass
    elif not os.path.exists(python_bin_path):
      print('Invalid python path: %s cannot be found.' % python_bin_path)
      continue
    else:
      print('%s is not executable.  Is it the python binary?' % python_bin_path)
      continue

    try:
      compile_args = run_command([
        python_bin_path, '-c',
        'import distutils.sysconfig; import tensorflow as tf; print(tf.__version__ + "|" + tf.sysconfig.get_lib('
        ') + "|" + "|".join(tf.sysconfig.get_compile_flags()) + "|" + distutils.sysconfig.get_python_inc())'
      ]).split("|")
      if not compile_args[0].startswith(_COMPAT_TENSORFLOW_VERSION):
        print('Invalid python path: %s compat tensorflow version is %s'
              ' got %s.' % (python_bin_path, _COMPAT_TENSORFLOW_VERSION,
              compile_args[0]))
        continue
    except subprocess.CalledProcessError:
      print('Invalid python path: %s tensorflow not installed.' %
            python_bin_path)
      continue
    # Write tools/python_bin_path.sh
    with open(real_config_path('PYTHON_BIN_PATH'), 'w') as f:
      f.write(python_bin_path)
    with open(real_config_path('COMPILE_FLAGS'), 'w') as f:
      for flag in compile_args[2:-1]:
        f.write(flag + '\n')
      f.write("-I" + compile_args[-1] + '\n')
    with open(real_config_path('LINK_FLAGS'), 'w') as f:
      f.write(os.path.join(compile_args[1], 'libtensorflow_framework.so.1\n'))
      f.write(os.path.join(compile_args[1], 'python', '_pywrap_tensorflow_internal.so\n'))
    break


def setup_ascend(env_path):
  """Get ascend install path."""
  default_ascend_path = "/usr/local/Ascend"
  custom_ascend_path = env_path
  while True:
    if not custom_ascend_path:
      ascend_path = default_ascend_path
    else:
      ascend_path = custom_ascend_path
    # Check if the path is valid
    if os.path.isdir(ascend_path) and os.access(ascend_path, os.X_OK):
      break
    elif not os.path.exists(ascend_path):
      print('Invalid ascend path: %s cannot be found.' % ascend_path)
  with open(real_config_path('LINK_FLAGS'), 'a') as f:
    f.write(os.path.join(ascend_path, "driver", "lib64", "driver", "libtsdclient.so\n"))
    f.write(os.path.join(ascend_path, "driver", "lib64", "common", "libc_sec.so\n"))
    f.write(os.path.join(ascend_path, "fwkacllib", "lib64", "libge_runner.so\n"))
    f.write(os.path.join(ascend_path, "fwkacllib", "lib64", "libfmk_parser.so\n"))
    f.write(os.path.join(ascend_path, "fwkacllib", "lib64", "libdatatransfer.so\n"))
    f.write(os.path.join(ascend_path, "fwkacllib", "lib64", "libindextransform.so\n"))

def setup_swig():
  """Get swig install path."""
  default_swig_path = which('swig')
  custom_swig_path = ''
  ask_swig_path = ''
  if default_swig_path:
    custom_swig_path = default_swig_path
    compile_args = run_command([
      custom_swig_path, '-version'])
    if not _COMPAT_SWIG_VERSION in compile_args:
      print('Invalid default python version: %s.' % compile_args)
      ask_swig_path = ('Please specify the location of swig. [Default is '
                       '%s]\n(Please enter the correct swig path: ') % default_swig_path
      custom_swig_path = ''
  else:
    ask_swig_path = ('Please specify the location of swig. [Default is '
                     '%s]\n(Please enter the correct swig path: ') % default_swig_path

  while True:
    if not custom_swig_path:
      swig_path = get_input(ask_swig_path)
    else:
      swig_path = custom_swig_path
      custom_swig_path = None
    if not swig_path:
      swig_path = default_swig_path
    # Check if the path is valid
    if os.path.isfile(swig_path) and os.access(swig_path, os.X_OK):
      break
    elif not os.path.exists(swig_path):
      print('Invalid swig path: %s cannot be found.' % swig_path)
      continue
    else:
      print('%s is not executable.  Is it the swig binary?' % swig_path)
      continue

  with open(real_config_path('SWIG_BIN_PATH'), 'w') as f:
    f.write(swig_path)

def main():
  env_snapshot = dict(os.environ)
  setup_python()
  setup_ascend(env_snapshot.get(_ASCEND_INSTALL_PATH_ENV))
  setup_swig()


if __name__ == '__main__':
  main()
