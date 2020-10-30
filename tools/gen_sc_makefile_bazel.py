#!/usr/bin/python
#-*- coding: UTF-8 -*-

import os
import sys
import string
import datetime
import time
import re

source_suffix = ["c","cpp","cc","cce"]

def get_gcc_cmd(input_file, cpp_file_list, code_top_dir, custom_code_top_dir=""):
    gcc_cmd_set = {}
    lint_cmd_set = {}
    file = open(input_file, "r")
    lines = file.readlines()
    file.close()
    compile_cmd_regex = re.compile('(gcc|g\+\+|\"aarch64-linux-gnu-gcc\"|ccec)\s+[^ ].*\s-o\s+[^ ]+\.(o|s|cpp|obj)(\s|$)')
    if(custom_code_top_dir != "") :
        custom_code_top_dir = custom_code_top_dir.replace(code_top_dir + "/", "")
    for line in lines:
        line = line.strip()
        line = line.strip(')')
        if not compile_cmd_regex.search(line):
            continue
        items = line.split()
        item_list = []
        compiler = ""
        compiler_path = ""
        for i in range(len(items)):
            items[i] = items[i].strip("\"")
            if items[i].endswith("gcc") or items[i].endswith("g++") or items[i].endswith("clang") or items[i].endswith("clang++"):
                compiler = items[i].split('/')[-1]
                if compiler.startswith("aarch64"):
                    items[i] = "external/hcc/bin/" + compiler
                if custom_code_top_dir != "" and not items[i].startswith("/") :
                    items[i] = code_top_dir + "/" + custom_code_top_dir + items[i]
                item_list = items[i:]
                break
        if len(item_list) == 0:
            continue
        if "-MD" in item_list and "-MF" in item_list :
            index = item_list.index("-MF")
            if custom_code_top_dir != "" :
                item_list[index + 1] = custom_code_top_dir + item_list[index + 1]

        cpp_file = ""
        if item_list[-1].strip().split(".")[-1] in source_suffix:
            if custom_code_top_dir != "" :
                item_list[-1] = custom_code_top_dir + item_list[-1].strip()
            cpp_file = item_list[-1].strip()
        else:
            if "-c" not in item_list:
                continue
            index = item_list.index("-c")
            if custom_code_top_dir != "" :
                item_list[index+1] = custom_code_top_dir + item_list[index+1].strip()
            cpp_file = item_list[index+1]
        if cpp_file[-1].strip().split(".")[-1] not in source_suffix:
            continue

        if "-o" not in item_list:
            continue
        try:
            index = item_list.index("-o")
        except:
            continue
        obj_file = item_list[index+1]
        if custom_code_top_dir != "" :
            obj_file = custom_code_top_dir + obj_file
            item_list[index+1] = obj_file

        cpp_file_rel = cpp_file
        if cpp_file_rel.startswith("/"):
            code_top_prefix_len = len(code_top_dir)
            cpp_file_rel = cpp_file[code_top_prefix_len+1:]

        if cpp_file_rel in cpp_file_list:
            # gcc_cmd_set[obj_file] = " ".join(item_list)
            c_flags_list = []
            rest_list = []
            is_add_I = False
            is_add_D = False
            for item in item_list:
                item = item.strip()
                if is_add_I:
                    is_add_I = False
                    item = "-I" + custom_code_top_dir + item
                    c_flags_list.append(item)
                    continue
                if is_add_D:
                    is_add_D = False
                    item = "-D" + item
                    c_flags_list.append(item)
                    continue
                is_add_I = False
                is_add_D = False
                if item == "-I":
                    is_add_I = True
                elif item == "-D":
                    is_add_D = True
                elif item.startswith("-I") or item.startswith("-D"):
                    c_flags_list.append(item)
                else:
                    rest_list.append(item)
                    continue
            gcc_options = " ".join(c_flags_list)
            c_flags_list.append("-D_lint")
            c_flags_list.append("-DLINUX_PC_LINT")
            c_flags = " ".join(c_flags_list)
            if compiler.startswith("aarch64-linux-gnu"):
                wine_str = "export LINT_PATH=%s/vendor/hisi/llt/ci/tools/pc-lint;wine $(LINT_PATH)/LINT-NT.EXE %s $(LINT_PATH)/davinci_arm64.lnt %s"%(code_top_dir,c_flags, cpp_file)
            else:
                wine_str = "export LINT_PATH=%s/vendor/hisi/llt/ci/tools/pc-lint;wine $(LINT_PATH)/LINT-NT.EXE %s $(LINT_PATH)/davinci_x86.lnt %s"%(code_top_dir,c_flags, cpp_file)

            rest_option = " ".join(rest_list)
            gcc_cmd_str = rest_option + " " + gcc_options
            gcc_cmd_set[obj_file] = gcc_cmd_str
            lint_out_path = code_top_dir + "/out/tools/lint/" + obj_file
            lint_cmd_set[lint_out_path] = wine_str

    return gcc_cmd_set,lint_cmd_set

def walkDir(top_dir,directory):
    fileArray = []
    for root, dirs, files in os.walk(directory):
        for name in files:
            if name.endswith(".c") or name.endswith(".cc") or name.endswith(".cpp"):
                fileArray.append(os.path.abspath(os.path.join(root, name))[len(top_dir)+1:])
    return fileArray

def get_cpp_file_list(top_dir,input_file, custom_code_top_dir=""):
    cpp_file_list = []
    file = open(input_file, "r")
    lines = file.readlines()
    file.close()
    for line in lines:
        line = line.strip()
        if custom_code_top_dir != "" :
            line_path = os.path.join(custom_code_top_dir,line)
        else :
            line_path = os.path.join(top_dir,line)
        if os.path.isfile(line_path):
            if line.endswith(".c") or line.endswith(".cc") or line.endswith(".cpp"):
                cpp_file_list.append(line_path[len(top_dir)+1:])
        if os.path.isdir(line_path):
            filelist = walkDir(top_dir,line_path)
            if len(filelist) > 0:
                cpp_file_list.extend(filelist)
    return cpp_file_list

def main():
    analysis_file = sys.argv[1]
    cpp_file = sys.argv[2]
    output_file = sys.argv[3]
    code_top_dir = os.getcwd()
    custom_code_top_dir = ""
    if len(sys.argv) == 5 :
        custom_code_top_dir = sys.argv[4]
        cpp_file_list = get_cpp_file_list(code_top_dir,cpp_file, custom_code_top_dir)
    else :
        cpp_file_list = get_cpp_file_list(code_top_dir,cpp_file)

    gcc_cmd_set,lint_cmd_set = get_gcc_cmd(analysis_file, cpp_file_list, code_top_dir, custom_code_top_dir)
    if len(gcc_cmd_set) == 0:
        print "Error: can not get sc gcc cmd "
        sys.exit(-1)
    if len(lint_cmd_set) == 0:
        print "Error: can not get lint gcc cmd "
        sys.exit(-1)

    fd = open(output_file, "w")
    # content = "GCC_PATH_1 = " + code_top_dir + "/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-gnu-4.8/bin" + "\n"
    # content += "GCC_PATH = $(GCC_PATH_1)" + "\n"
    # content += "export GCC_PATH\n\n"
    content = ""
    content += ".PHONY: all\n"
    content += "\n"
    content += "ifeq ($(PCLINT_ENABLE),true)\n"
    for (obj_file,wine_cmd) in lint_cmd_set.items():
        obj_file = obj_file + ".lint"
        content += "all:" + obj_file  + "\n"
        content += obj_file + ": FORCE\n"
        content += "\tmkdir -p $(dir $@)\n"
        content += "\t"+ wine_cmd + " > $@ \n"
        content += "\n"
    content += "else\n"

    for (obj_file,gcc_cmd) in gcc_cmd_set.items():
        content += "all:" + obj_file + "\n"
        content += obj_file + ": FORCE\n"
        content += "\trm -rf $@\n"
        content += "\t $(SOURCEANALYZER) " + gcc_cmd + "\n"
        content += "\n"

    content += "endif\n"
    content += "\n"
    content += ".PHONY: FORCE\n"
    content += "FORCE: \n"
    fd.write(content)
    fd.close()

if __name__ == '__main__':
    main()
