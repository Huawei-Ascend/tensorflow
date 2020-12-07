"""
    check interface
"""
import os
# 获取接口规范的目录
specs_dir = os.path.realpath("../interface_spec")
# 获取tf-adapter的根目录
tf_adapter_root = os.path.realpath("../")

class FuncIntfSpec:
    """
    函数API的规格定义，主要包含函数的名称，与函数的入参
    暂时校验不了出参
    """
    def __init__(self, func_name,  param_list):
        self.func_name = func_name
        self.param_list = param_list
        self.class_list = {}
        self.func_list = {}

    def add_func_spec(self, func_info):
        self.func_list[func_info.func_name] = func_info

    def add_class_spec(self, spec):
        self.class_list[spec.class_name]=spec

    def print_detail(self):
        print("func name: %s, param_list: [%s]" % (self.func_name, ",".join(self.param_list)))

class GlobalVarSpec:
    """
    全局变量定义，例如elewise_compute接口中的NAME_INDEX
    类型主要包含：全局变量名，全局变量值
    """
    def __init__(self, name, values):
        self.global_var_name = name
        self.global_var_values = values
        self.func_list = {}
        self.param_list = {}

class ClassIntfSpec:
    """
    class 类型的接口规格定义，例如Tik的各种API都是封装在类中的
    类型主要包含：类的名称，类的父类，类提供的api函数接口
    """
    def __init__(self, name, supper_classes):
        self.class_name = name
        self.supper_classes = supper_classes
        self.func_list = {}
        self.param_list = {}
        self.class_list = {}

    def add_func_spec(self, func_info: FuncIntfSpec):
        self.func_list[func_info.func_name] = func_info

    def add_class_spec(self, spec):
        self.class_list[spec.class_name]=spec

    def print_detail(self):
        print("-------------------------------------------")
        print("class name: %s, supper classes: %s" % (self.class_name, ",".join(self.supper_classes)))
        print("func list:")
        for (idx, func_spec) in self.func_list.items():
            func_spec.print_detail()
        print("params list:")
        for param_spec in self.param_list:
            param_spec.print_detail()
        print("-------------------------------------------")


class FileSpec:
    """
    文件的规格定义，对应到一个源码文件
    """
    def __init__(self, spec_file_name, source_file_name=""):
        self.spec_file_name = spec_file_name
        self.source_file_name = source_file_name
        self.class_specs = {}
        self.func_specs = {}
        self.global_var_spec = {}

    def add_class_spec(self, spec: ClassIntfSpec):
        self.class_specs[spec.class_name]=spec

    def add_func_spec(self, spec: FuncIntfSpec):
        self.func_specs[spec.func_name]=spec

    def add_global_var_spec(self, spec: GlobalVarSpec):
        self.global_var_spec[spec.global_var_name] = spec

    def print_detail(self):
        print("===========================================")
        print("file name: %s" % self.spec_file_name)
        print("class specs:")
        for _, class_spec in self.class_specs.items():
            class_spec.print_detail()
        print("func specs:")
        for _, func_spec in self.func_specs.items():
            func_spec.print_detail()
        print("===========================================")

last_tree_idx = 1
last_tab_str = "  "

def get_tree_idx(str_info: str):
    """
    python 代码的层级靠缩进来保证，这里获取缩进了几层。
    python代码的缩进其实没有规定多大，只要同一层次的缩进是一样的就可以。
    这里假定我们的代码格式是规范的，按照4/2个空格来做
    """
    global last_tab_str
    if str_info.startswith(last_tab_str):
        return 1 + get_tree_idx(str_info[len(last_tab_str):])
    else:
        return 1

def get_tree_idx_2(str_info: str):
    """
    按照2个空格来计算
    """
    TAB_STR = "  "
    if str_info.startswith(TAB_STR):
        return 1 + get_tree_idx_2(str_info[2:])
    else:
        return 1

def get_tree_idx_final(str_info: str):
    global last_tree_idx
    global last_tab_str
    if last_tree_idx == 1:
        if get_tree_idx_2(str_info) > 2:
            last_tab_str = "    "
        else:
            last_tab_str = "  "
    last_tree_idx = get_tree_idx(str_info)
    return last_tree_idx


def build_file_spec(spec_lines, file_spec: FileSpec):
    """
    根据文件中的代码解析出规格定义。
    """
    spec_tree = [file_spec, ]
    for spec_line in spec_lines:
        spec_line = spec_line.rstrip()
        if spec_line.startswith("#"):
            continue
        elif "class " in spec_line and "class _" not in spec_line:
            # 表示定义了一个class
            tree_idx = get_tree_idx_final(spec_line)
            if "(" in spec_line:
                class_name = spec_line[6:spec_line.index("(")]
                super_classes = spec_line[spec_line.index("(")+1: spec_line.index(")")].split(",")
            else:
                class_name = spec_line[6:-1]
                super_classes = []
            class_spec = ClassIntfSpec(class_name, super_classes)
            if len(spec_tree) < (tree_idx + 1):
                spec_tree.append(class_spec)
            else:
                spec_tree[tree_idx] = class_spec
            spec_tree[tree_idx-1].add_class_spec(class_spec)
        elif "def " in spec_line:
            # 表示定义了一个函数api
            tree_idx = get_tree_idx_final(spec_line)
            spec_line = spec_line.strip()
            func_name = spec_line[4:spec_line.index("(")]
            param_list = [x.strip() for x in spec_line[spec_line.index("(")+1:spec_line.index(")")].split(",")]
            func_spec = FuncIntfSpec(func_name, param_list)
            if len(spec_tree) < (tree_idx + 1):
                spec_tree.append(func_spec)
            else:
                spec_tree[tree_idx] = func_spec
            spec_tree[tree_idx-1].add_func_spec(func_spec)
        elif is_global_variable(spec_line):
            tree_idx = get_tree_idx_final(spec_line)
            name = spec_line[:spec_line.index("=")].rstrip()
            values = spec_line[(len(name) + 3):]
            global_var_spec = GlobalVarSpec(name, values)
            if len(spec_tree) < (tree_idx + 1):
                spec_tree.append(global_var_spec)
            else:
                spec_tree[tree_idx] = global_var_spec
            spec_tree[tree_idx - 1].add_global_var_spec(global_var_spec)


def is_global_variable(spec_line):
    if spec_line.startswith("    ") or spec_line.startswith("  "):
        return False
    if "=" not in spec_line:
        return False
    name = spec_line[:spec_line.index("=")].rstrip()
    name_split = name.split("_")
    for i in name_split:
        if not i.isupper():
            return False
    return True


def get_spec_info_list():
    # 从接口规范文件夹中，扫描出所有的接口规范定义
    def _get_class_spec_info(spec_line, spec_tree):
        tree_idx = get_tree_idx_final(spec_line)
        if "(" in spec_line:
            class_name = spec_line[6:spec_line.index("(")]
            super_classes = spec_line[spec_line.index("(") + 1: spec_line.index(")")].split(",")
        else:
            class_name = spec_line[6:-1]
            super_classes = []
        class_spec = ClassIntfSpec(class_name, super_classes)
        if len(spec_tree) < (tree_idx + 1):
            spec_tree.append(class_spec)
        else:
            spec_tree[tree_idx] = class_spec
        spec_tree[tree_idx - 1].add_class_spec(class_spec)

    def _get_def_spec_info(spec_line, spec_tree):
        tree_idx = get_tree_idx_final(spec_line)
        spec_line = spec_line.strip()
        func_name = spec_line[4:spec_line.index("(")]
        param_list = [x.strip() for x in spec_line[spec_line.index("(") + 1:spec_line.index(")")].split(",")]
        func_spec = FuncIntfSpec(func_name, param_list)
        if len(spec_tree) < (tree_idx + 1):
            spec_tree.append(func_spec)
        else:
            spec_tree[tree_idx] = func_spec
        spec_tree[tree_idx - 1].add_func_spec(func_spec)

    def _get_global_var_spec_info(spec_line, spec_tree):
        tree_idx = get_tree_idx_final(spec_line)
        name = spec_line[:spec_line.index("=")].rstrip()
        values = spec_line[(len(name) + 3):]
        global_var_spec = GlobalVarSpec(name, values)
        if len(spec_tree) < (tree_idx + 1):
            spec_tree.append(global_var_spec)
        else:
            spec_tree[tree_idx] = global_var_spec
        spec_tree[tree_idx - 1].add_global_var_spec(global_var_spec)

    specs = os.listdir(specs_dir)
    file_spec_list = []
    for spec_file in specs:
        spec_tree = []
        source_file_path = os.path.realpath(os.path.join(specs_dir, spec_file))
        spec_lines = get_spec_from_file(source_file_path, True)

        for spec_line in spec_lines:
            spec_line = spec_line.rstrip()
            if "# source file:" in spec_line:
                spec_source_file = spec_line[14:].strip()
                file_spec = FileSpec(spec_file, spec_source_file)
                file_spec_list.append(file_spec)
                if len(spec_tree) < 1:
                    spec_tree.append(file_spec)
                else:
                    spec_tree[0] = file_spec
            elif spec_line.startswith("#"):
                continue

            elif "class " in spec_line:
                _get_class_spec_info(spec_line, spec_tree)

            elif "def " in spec_line:
                _get_def_spec_info(spec_line, spec_tree)

            elif is_global_variable(spec_line):
                _get_global_var_spec_info(spec_line, spec_tree)

    return file_spec_list


def get_spec_from_file(file_path, is_defined_file=False):
    with open(file_path) as ff:
        lines = ff.readlines()

    def _get_new_lines(line, line_end, new_lines):
        if "def " in line or "class " in line:
            if not line.rstrip().endswith(":"):
                line_end = False
                new_lines.append(line.rstrip())
            else:
                new_lines.append(line.rstrip())
        elif is_global_variable(line):
            new_lines.append(line.rstrip())
        return line_end

    new_lines = []
    block_comment = False
    line_end = True
    for line in lines:
        tmp_line = line.strip()
        if block_comment:
            # 前面是块状注释, 直到获取到块状注释结束符为止
            if "\"\"\"" in tmp_line:
                block_comment = False
        else:
            if is_defined_file:
                if "# source file:" in line:
                    new_lines.append(line)
                    continue
            if tmp_line.startswith("#"):
                continue
            if "\"\"\"" in tmp_line and not tmp_line.endswith("\"\"\""):
                # 包含连续的3个引号，是块状注释， 这里也比较粗暴，这里块状注释一般都是独立行的
                block_comment = True
                continue
            if "#" in line:
                # 去除行尾注释
                line = line[:line.index("#")]
            if line_end:
                # 因为函数以及class申明的地方可能换行了，如果前面一个函数的定义还未结束，则继续拼接上去，去除换行
                line_end = _get_new_lines(line, line_end, new_lines)
            else:
                new_lines[-1]+=line.strip()
                # 函数定义和class定义都是以冒号结尾的，检测冒号
                if line.rstrip().endswith(":"):
                    line_end = True

    return new_lines


def remove_sub_func_under_func(lines):
    """
    因为get_spec_from_file提取的是文件中的接口和class定义。
    由于函数内部还可以定义函数，这里也会被提取出来，因此去除函数内部的函数。
    """
    tree_info = [""]
    new_lines = []
    for line in lines:
        idx = get_tree_idx_final(line)
        if line.strip().startswith("def "):
            type_name = "func"
        elif line.strip().startswith("class "):
            type_name = "class"
        elif is_global_variable(line):
            name = line[:line.index("=")].rstrip()
            type_name = name
        if type_name == "func" and tree_info[min(len(tree_info)-1, idx-1)] == "func":
            continue
        if len(tree_info) <= idx:
            tree_info.append(type_name)
        else:
            tree_info[idx] = type_name
        new_lines.append(line)
    return new_lines


def check_source_file_match(defined_spec: FileSpec):
    """
    校验源码是否符合规范定义
    """
    source_file_path = os.path.realpath(os.path.join(tf_adapter_root, defined_spec.source_file_name))
    lines = get_spec_from_file(source_file_path)
    new_lines = remove_sub_func_under_func(lines)

    spec_in_source = FileSpec("", source_file_path)
    build_file_spec(new_lines, spec_in_source)

    return compare_file_spec(defined_spec, spec_in_source)


def compare_func_spec(spec1: FuncIntfSpec, spec2: FuncIntfSpec, file_name1="", file_name2=""):
    params_1 = sorted([x.strip() for x in spec1.param_list])
    params_2 = sorted([x.strip() for x in spec2.param_list])
    if params_1 != params_2:
        print("[EEEE] compare \"%s\" func failed"  % spec1.func_name)
        print("[EEEE] file path: \"%s\"" % file_name1)
        print("[EEEE] param list: \"%s\"" % ",".join(params_1))
        print("[EEEE] file path: \"%s\"" % file_name2)
        print("[EEEE] param list: \"%s\"" % ",".join(params_2))
        return False
    else:
        print("[====] compare \"%s\" func success" % spec1.func_name)
        return True


def build_diff_list_result(func_names_1, func_names_2):
    func_names_1 = func_names_1[:]
    func_names_2 = func_names_2[:]
    diff_print_name1 = []
    diff_print_name2 = []
    len_1 = len(func_names_1)
    len_2 = len(func_names_2)
    i = 0
    j = 0
    while i<len_1 or j<len_2:
        if i < len_1 and j < len_2:
            if func_names_1[i] == func_names_2[j]:
                diff_print_name1.append(func_names_1[i])
                diff_print_name2.append(func_names_2[j])
                i += 1
                j += 1
            else:
                if func_names_1[i] in func_names_2:
                    diff_print_name1.append("_"*len(func_names_2[j]))
                    diff_print_name2.append(func_names_2[j])
                    j += 1
                elif func_names_2[j] in func_names_1:
                    diff_print_name2.append("_"*len(func_names_1[i]))
                    diff_print_name1.append(func_names_1[i])
                    i += 1
                else:
                    diff_print_name1.append(func_names_1[i])
                    diff_print_name2.append("_" * len(func_names_1[i]))
                    diff_print_name1.append("_" * len(func_names_2[j]))
                    diff_print_name2.append(func_names_2[j])
                    i += 1
                    j += 1
        else:
            if i == len_1:
                for item in func_names_2[j:]:
                    diff_print_name1 +=["_" * len(item)]
                    diff_print_name2.append(item)
                j = len_2
            elif j == len_2:
                for item in func_names_1[i:]:
                    diff_print_name1.append(item)
                    diff_print_name2 += ["_" * len(item)]
                i = len_1

    return diff_print_name1, diff_print_name2


def compare_class_spec(spec1: ClassIntfSpec, spec2: ClassIntfSpec, file_name1="", file_name2=""):
    compare_matched = True
    print("[====] compare class: \"%s\"" % spec1.class_name)
    func_list_1 = spec1.func_list
    func_list_2 = spec2.func_list
    # remove private func api
    func_names_1 = sorted([x for x in func_list_1.keys() if x == "__init__" or x == "__new__" \
        or not x.startswith("__") and not x.startswith("_")])
    func_names_2 = sorted([x for x in func_list_2.keys() if x == "__init__" or x == "__new__" \
        or not x.startswith("__") and not x.startswith("_")])
    if spec1.supper_classes != spec2.supper_classes:
        print("[EEEE] compare class: \"%s\", supper_classes is different in interface define: \"%s\","
              " in source file: \"%s\"" % (spec1.class_name, spec1.supper_classes, spec2.supper_classes))
        return False
    for func_name in func_names_1:
        if func_name not in func_names_2:
            print("[EEEE] func list in class: \"%s\" is different" % spec1.class_name)
            print("[EEEE] func in interface define:", spec1.func_list)
            print("[EEEE] func in source file:", spec2.func_list)
            print("[EEEE] compare func: \"%s\" in interface define, not in source file" % func_name)
            compare_matched = False
        elif not compare_func_spec(func_list_1[func_name], func_list_2[func_name], file_name1, file_name2):
            compare_matched = False
    print("[====] compare class: \"%s\" end, result: \"%s\"" % (spec1.class_name, "Success" if compare_matched else "Fail"))
    return compare_matched


def compare_global_var_spec(spec1: GlobalVarSpec, spec2: GlobalVarSpec, file_name1="", file_name2=""):
    values_1 = sorted([x.strip() for x in spec1.global_var_values])
    values_2 = sorted([x.strip() for x in spec2.global_var_values])
    if values_1 != values_2:
        print("[EEEE] compare \"%s\" global_var failed" % spec1.global_var_name)
        print("[EEEE] file path: \"%s\"" % file_name1)
        print("[EEEE] param list: \"%s\"" % ",".join(values_1))
        print("[EEEE] file path: \"%s\"" % file_name2)
        print("[EEEE] param list: \"%s\"" % ",".join(values_2))
        return False
    else:
        print("[====] compare \"%s\" global_var success" % spec1.global_var_name)
        return True


def compare_file_spec(spec1: FileSpec, spec2: FileSpec):
    print("\n[====] compare interface define: \"%s\", source file: \"%s\"" % (spec1.spec_file_name, spec1.source_file_name))
    compare_matched = True

    def _compare_global_var(compare_matched, spec1, spec2):
        global_var_list_1 = spec1.global_var_spec
        global_var_list_2 = spec2.global_var_spec
        global_var_name1 = sorted([x.strip() for x in global_var_list_1.keys()])
        global_var_name2 = sorted([x.strip() for x in global_var_list_2.keys()])
        for name in global_var_name1:
            if name not in global_var_name2:
                print("[EEEE] compare var: \"%s\" in interface define: \"%s\" not in source file：\"%s\" " \
                    % (name, spec1.spec_file_name, spec1.source_file_name))
                compare_matched = False
            elif not compare_global_var_spec(global_var_list_1[name1], global_var_list_2[name2], spec1.spec_file_name, spec1.source_file_name):
                compare_matched = False
        return compare_matched

    def _compare_func(compare_matched, spec1, spec2):
        func_list_1 = spec1.func_specs
        func_list_2 = spec2.func_specs
        func_names_1 = sorted([x for x in func_list_1.keys() if not x.startswith("__") and not x.startswith("_")])
        func_names_2 = sorted([x for x in func_list_2.keys() if not x.startswith("__") and not x.startswith("_")])
        for func_name in func_names_1:
            if func_name not in func_names_2:
                print("[EEEE] compare func: \"%s\" in interface define: \"%s\" not in source file：\"%s\" " \
                    % (func_name, spec1.spec_file_name, spec1.source_file_name))
                compare_matched = False
            elif not compare_func_spec(func_list_1[func_name], func_list_2[func_name], spec1.spec_file_name, spec1.source_file_name):
                compare_matched = False
        return compare_matched

    def _compare_class(compare_matched, spec1, spec2):
        class_list_1 = spec1.class_specs
        class_list_2 = spec2.class_specs
        class_name1 = sorted([x.strip() for x in class_list_1.keys()])
        class_name2 = sorted([x.strip() for x in class_list_2.keys()])
        for class_name in class_name1:
            if class_name not in class_name2:
                print("[EEEE] compare class: \"%s\" in interface define: \"%s\" not in source file：\"%s\" " \
                    % (class_name, spec1.spec_file_name, spec1.source_file_name))
                compare_matched = False
            elif not compare_class_spec(class_list_1[class_name], class_list_2[class_name], spec1.spec_file_name, spec1.source_file_name):
                compare_matched = False
        return compare_matched

    compare_matched = _compare_global_var(compare_matched, spec1, spec2)

    compare_matched = _compare_func(compare_matched, spec1, spec2)

    compare_matched = _compare_class(compare_matched, spec1, spec2)

    print("[====] compare result: \"%s\". interface define: \"%s\", source file: \"%s\"" % ("Success" if compare_matched else "Fail", spec1.spec_file_name, spec1.source_file_name))
    return compare_matched


def check_all():
    check_result = True
    spec_define_list = get_spec_info_list()
    for spec in spec_define_list:
        if not check_source_file_match(spec):
            check_result = False
    return check_result


if __name__ == "__main__":
    if not check_all():
        exit(-1)

