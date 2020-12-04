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
import sys
import getopt
import util_global
from file_op import before_clear
from conver import conver

def para_check_and_set(argv):
    input  = "input"
    output = "output"
    report = "report"

    try:
        opts, args = getopt.getopt(argv, "hi:o:r:", ["help", "input=", "output=", "report="])
    except getopt.GetoptError:
        print('Parameter error, please check.')
        print('    main.py -i <input> -o <output> -r <report>')
        print('or: main.py --input=<input> --output=<output> --report=<report>')
        print('-i or --input:  The source script to be converted, Default value: input/')
        print('-o or --output: The destination script after converted, Default value: output/')
        print('-r or --report: Conversion report, Default value: report/')
        sys.exit(2)

    for opt, arg in opts:
        if opt in ("-h", "--help"):
            print('    main.py -i <input> -o <output> -r <report>')
            print('or: main.py --input=<input> --output=<output> --report=<report>')
            print('-i or --input:  The source script to be converted, Default value: input/')
            print('-o or --output: The destination script after converted, Default value: output/')
            print('-r or --report: Conversion report, Default value: report/')
            sys.exit()
        elif opt in ("-i", "--input"):
            input = arg
        elif opt in ("-o", "--output"):
            output = arg
        elif opt in ("-r", "--report"):
            report = arg
    util_global.set_value('input',  input)
    util_global.set_value('output', output)
    util_global.set_value('report', report)

if __name__ == "__main__":
    util_global._init()
    para_check_and_set(sys.argv[1:])
    before_clear()
    conver()