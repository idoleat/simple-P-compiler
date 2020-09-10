#!/usr/bin/python3

import subprocess
import os
import sys
import json
from argparse import ArgumentParser

class Grader:

    basic_case_dir = "./basic_cases"
    basic_cases = {
        1 : "program",
        2 : "declaration",
        3 : "function",
        4 : "compound",
        5 : "PrintBinUnConstantInvocation",
        6 : "VarRefAssignRead",
        7 : "if",
        8 : "while",
        9 : "for",
        10: "return",
        11: "call"
    }
    basic_case_scores = [0, 5, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9]

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

        clist = [self.parser, test_case, "--dump-ast"]
        cmd = " ".join(clist)
        try:
            proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
        except Exception as e:
            print("Call of '%s' failed: %s" % (" ".join(clist), e))
            return False

        stdout = str(proc.stdout.read(), "utf-8")
        stderr = str(proc.stderr.read(), "utf-8")
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
