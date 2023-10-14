'''
Resources:
    - https://github.com/PX4/ecl/commit/264c8c4e8681704e4719d0a03b848df8617c0863
'''

from sympy import ccode, cse, symbols, Matrix
from sympy.codegen.ast import float32, real
import os


class CodeGenerator:
    def __init__(self, file_name):
        path = os.path.dirname(os.path.abspath(__file__))
        file_dir = os.path.join(path, "generated")

        if not os.path.exists(file_dir):
            os.makedirs(file_dir)

        file_path = os.path.join(file_dir, file_name)

        self.file = open(file_path, 'w')

    def print_string(self, string):
        self.file.write("// " + string + "\n")

    def get_ccode(self, expression):
        return ccode(expression, type_aliases={real: float32})

    def write_subexpressions(self, subexpressions):
        write_string = ""

        for item in subexpressions:
            write_string = write_string + "const float " + str(item[0]) + " = " + self.get_ccode(item[1]) + ";\n"

        write_string = write_string + "\n\n"
        self.file.write(write_string)

    def write_matrix(self, matrix, variable_name, is_symmetric=False, pre_bracket="[", post_bracket="]", separator="]["):
        write_string = ""

        if matrix.shape[0] * matrix.shape[1] == 1:
            write_string = write_string + variable_name + " = " + self.get_ccode(matrix[0]) + ";\n"
        elif matrix.shape[0] == 1 or matrix.shape[1] == 1:
            for i in range(0, len(matrix)):
                write_string = write_string + variable_name + pre_bracket + str(i) + post_bracket + " = " + self.get_ccode(matrix[i]) + ";\n"
        else:
            for j in range(0, matrix.shape[1]):
                for i in range(0, matrix.shape[0]):
                    if j >= i or not is_symmetric:
                        write_string = write_string + variable_name + pre_bracket + str(i) + separator + str(j) + post_bracket + " = " + self.get_ccode(matrix[i, j]) + ";\n"

        write_string = write_string + "\n\n"

        self.file.write(write_string)

    def close(self):
        self.file.close()


def write_cov_matrix(name, P):
    P_new = cse(P, symbols("PS0:1000"), optimizations='basic')

    gen = CodeGenerator(f'{name}.c')
    gen.write_subexpressions(P_new[0])
    gen.write_matrix(Matrix(P_new[1]), 'P_Next', True)
    gen.close()


def write_obs_eqs(name, HK):
    H = cse(HK[0], symbols("HS0:1000"), optimizations='basic')
    K = cse(HK[1], symbols("KS0:1000"), optimizations='basic')

    gen = CodeGenerator(f'{name}.c')
    gen.write_subexpressions(H[0])
    gen.write_matrix(Matrix(H[1]), 'H_Fusion')
    gen.write_subexpressions(K[0])
    gen.write_matrix(Matrix(K[1]), 'K_Fusion')
    gen.close()
