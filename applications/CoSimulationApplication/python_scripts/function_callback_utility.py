from __future__ import print_function, absolute_import, division  # makes KratosMultiphysics backward compatible with python 2.6 and 2.7

# executing a function specified as a string, similar to "kratos/utilities/python_function_callback_utility.h"

from math import *

# list of safe methods that can be used
safe_methods_list = ['acos', 'asin', 'atan', 'atan2', 'ceil', 'cos',
             'cosh', 'degrees', 'e', 'exp', 'fabs', 'floor',
             'fmod', 'frexp', 'hypot', 'ldexp', 'log', 'log10',
             'modf', 'pi', 'pow', 'radians', 'sin', 'sinh', 'sqrt',
             'tan', 'tanh']

safe_dict = {}
for k in safe_methods_list:
    safe_dict[k] = locals()[k]

def GenericCallFunction(func_string, scope_vars, check=True):
    safe_dict.update(scope_vars)

    if check: # can be disabled to improve performance
        splitted_func_args = [""]
        cur_idx = 0
        for letter in func_string: # extracting the arguments from the function string
            if letter.isalpha():
                splitted_func_args[cur_idx]+=letter
            else:
                if len(splitted_func_args[cur_idx]) > cur_idx: # if multiple numeric characters are consecutive
                    cur_idx += 1
                    splitted_func_args.append("")

        for func_arg in splitted_func_args:
            if not func_arg in safe_dict:
                raise Exception('Argument "{}" in function string "{}" was not recognized!\nOnly the following expressions can be used:\n\t{}'.format(func_arg, func_string, "\n\t".join(sorted(safe_dict.keys()))))

    return eval(func_string, {"__builtins__" : None}, safe_dict)
