#!/usr/bin/python3

import subprocess
import os
import sys
import json
import textwrap
from argparse import ArgumentParser

class Grader:

    basic_case_dir = "./basic_cases"
    basic_cases = {
        1 : "table",
        2 : "redecl",
        3 : "variable",
        4 : "VariableReference",
        5 : "BinaryOperator",
        6 : "UnaryOperator",
        7 : "FunctionInvocation",
        8 : "PrintStatement",
        9 : "ReadStatement",
        10: "assignment",
        11: "condition",
        12: "ForLoop",
        13: "ret",
        14: "AdvSymbol",
        15: "AdvSema"
    }
    basic_case_scores = [0, 10, 9, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5]

    diff_result = ""

    def __init__(self, parser):
        self.parser = parser

        self.output_dir = "result"
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)

    def get_case_id_list(self, basic_id):
        if basic_id == 0:
            self.basic_id_list = self.basic_cases.keys()
        else:
            if not basic_id in self.basic_cases:
                print("ERROR: Invalid case ID %d" % basic_id)
                exit(1)
            self.basic_id_list = [basic_id]

    def gen_output(self, case_id):
        test_case = "%s/%s/%s.p" % (self.basic_case_dir, "test-cases", self.basic_cases[case_id])
        output_file = "%s/%s" % (self.output_dir, self.basic_cases[case_id])

        clist = [self.parser, test_case]
        cmd = " ".join(clist)
        try:
            proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
        except Exception as e:
            print("Call of '%s' failed: %s" % (" ".join(clist), e))
            return False

        def convert_byte_seq_to_str(byte_seq):
            src = byte_seq.readlines()
            buffer = ""
            for line in src:
                try:
                    buffer += str(line, "utf-8")
                except UnicodeDecodeError as e:
                    print('\n'*3)
                    print(textwrap.indent(buffer, f'Last Output -> '))
                    print(f'  Non unicode output in output stream: {line}')
                    print(f'  Please remove non-unicode characters in your output')
                    print('\n'*3)
                    sys.exit(0)
            return buffer

        stdout = convert_byte_seq_to_str(proc.stdout)
        stderr = convert_byte_seq_to_str(proc.stderr)

        retcode = proc.wait()
        with open(output_file, "w") as out:
            out.write(stdout)
            out.write(stderr)

    def test_sample_case(self, case_id):
        self.gen_output(case_id)

        output_file = "%s/%s" % (self.output_dir, self.basic_cases[case_id])
        solution = "%s/%s/%s" % (self.basic_case_dir, "sample-solutions", self.basic_cases[case_id])

        clist = ["diff", "-Z", "-u", output_file, solution, f'--label="your output:({output_file})"', f'--label="answer:({solution})"']
        cmd = " ".join(clist)
        try:
            proc = subprocess.Popen(cmd, stdout=subprocess.PIPE,
                                    stderr=subprocess.STDOUT, shell=True)
        except Exception as e:
            print("Call of '%s' failed: %s" % (cmd, e))
            return False


        output = str(proc.stdout.read(), "utf-8")
        retcode = proc.wait()
        if retcode != 0:
          self.diff_result += "{}\n".format(self.basic_cases[case_id])
          self.diff_result += "{}\n".format(output)

        return retcode == 0

    def run(self):
        print("---\tCase\t\tPoints")

        total_score = 0
        max_score = 0

        for b_id in self.basic_id_list:
            c_name = self.basic_cases[b_id]
            print("+++ TESTING case %s:" % c_name)
            ok = self.test_sample_case(b_id)
            max_val = self.basic_case_scores[b_id]
            get_val = max_val if ok else 0
            print("---\t%s\t%d/%d" % (c_name, get_val, max_val))
            total_score += get_val
            max_score += max_val

        print("---\tTOTAL\t\t%d/%d" % (total_score, max_score))

        with open("{}/{}".format(self.output_dir, "score.txt"), "w") as result:
            result.write("---\tTOTAL\t\t%d/%d" % (total_score, max_score))

        diff = open("{}/{}".format(self.output_dir, "diff.txt"), 'w')
        diff.write(self.diff_result)
        diff.close()

def main():
    parser = ArgumentParser()
    parser.add_argument("--parser", help="parser to grade", default="../src/parser")
    parser.add_argument("--basic_case_id", help="test case's ID", type=int, default=0)
    args = parser.parse_args()

    g = Grader(parser = args.parser)
    g.get_case_id_list(args.basic_case_id)
    g.run()

if __name__ == "__main__":
    main()
