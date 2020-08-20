# Copyright 2015 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================
"""npu bridge for tensorflow v1.15.0.


"""

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import fnmatch
import os
import re
import sys

from setuptools import Command
from setuptools import find_packages
from setuptools import setup
from setuptools.command.install import install as InstallCommandBase
from setuptools.dist import Distribution

DOCLINES = __doc__.split('\n')

# This version string is semver compatible, but incompatible with pip.
# For pip, we will remove all '-' characters from this string, and use the
# result for pip.
_VERSION = '1.15.0'

setup(
    name='npu_bridge',
    version=_VERSION,
    description=DOCLINES[0],
    long_description='\n'.join(DOCLINES[2:]),
    author='HuaWei Inc.',
    # Contained modules and scripts.
    packages=find_packages(),
    #install_requires=REQUIRED_PACKAGES,
    # Add in any packaged data.
    include_package_data=True,
    keywords='tensorflow tensor machine learning',
)
