#!/usr/bin/python3

import subprocess
import os
import sys
import json
from argparse import ArgumentParser

class Grader:
    
    basic_case_dir = "./basic_cases"
    basic_cases = {
        1 : "1_keywords1",
        2 : "2_keywords2",
        3 : "3_delimiters",
        4 : "4_arithmetic",
        5 : "5_logical",
        6 : "6_relational",
        7 : "7_identifier",
        8 : "8_constant",
        9 : "9_scientific_string",
        10: "10_comment",
        11: "11_pseudocomment"
    }
    basic_case_scores = [0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5]

    advance_case_dir = "./advance_cases"
    advance_cases = {
        1 : "1_pseudo_optS_1",
        2 : "2_pseudo_optS_2",
        3 : "3_pseudo_optT_1",
        4 : "4_pseudo_optT_2",
        5 : "5_comment_1",
        6 : "6_comment_2",
        7 : "7_operator",
        8 : "8_zero",
        9 : "9_float",
        10: "10_scientific",
        11: "11_string_1",
        12: "12_string_2",
        13: "13_string_3",
        14: "14_error_1",
        15: "15_error_2",
        16: "16_error_3",
        17: "17_error_4",
        18: "18_error_5"
    }
    advance_case_scores = [0, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 
                           3, 3, 3, 2, 2, 2, 2, 2]

    diff_result = ""

    def __init__(self, scanner):
        self.scanner = scanner

        self.output_dir = "result"
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)

    def get_case_id_list(self, basic_id, advance_id):
        if basic_id == 0:
            self.basic_id_list = self.basic_cases.keys()
        else:
            if not basic_id in self.basic_cases:
                print("ERROR: Invalid basic case ID %d" % basic_id)
                exit(1)
            self.basic_id_list = [basic_id]
        if advance_id == 0:
            self.advance_id_list = self.advance_cases.keys()
        else:
            if not advance_id in self.advance_cases:
                print("ERROR: Invalid advance case ID %d" % advance_id)
                exit(1)
            self.advance_id_list = [advance_id]

    def gen_output(self, case_type, case_id):
        if case_type == "basic":
            test_case = "%s/%s/%s.p" % (self.basic_case_dir, "test_cases", self.basic_cases[case_id])
            output_file = "%s/%s" % (self.output_dir, self.basic_cases[case_id])
        elif case_type == "advance":
            test_case = "%s/%s/%s.p" % (self.advance_case_dir, "test_cases", self.advance_cases[case_id])
            output_file = "%s/%s" % (self.output_dir, self.advance_cases[case_id])

        out = open(output_file, "w")
        clist = [self.scanner, test_case]
        try:
            retcode = subprocess.call(clist, stdout=out, stderr=subprocess.STDOUT, close_fds=True)
        except Exception as e:
            print("Call of '%s' failed: %s" % (" ".join(clist), e))
            return False

    def test_sample_case(self, case_type, case_id):
        self.gen_output(case_type, case_id)

        if case_type == "basic":
            output_file = "%s/%s" % (self.output_dir, self.basic_cases[case_id])
            solution = "%s/%s/%s" % (self.basic_case_dir, "sample_solutions", self.basic_cases[case_id])
        elif case_type == "advance":
            output_file = "%s/%s" % (self.output_dir, self.advance_cases[case_id])
            solution = "%s/%s/%s" % (self.advance_case_dir, "sample_solutions", self.advance_cases[case_id])

        clist = ["diff", "-Z", output_file, solution]
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
          if case_type == "basic":
            self.diff_result += "{}\n".format(self.basic_cases[case_id])
          elif case_type == "advance":
            self.diff_result += "{}\n".format(self.advance_cases[case_id])
          self.diff_result += "{}\n".format(output)

        return retcode == 0    

    def run(self):
        print("---\tCase\t\tPoints")

        total_score = 0
        max_score = 0

        for b_id in self.basic_id_list:
            c_name = self.basic_cases[b_id]
            print("+++ TESTING basic case %s:" % c_name)
            ok = self.test_sample_case("basic", b_id)
            max_val = self.basic_case_scores[b_id]
            get_val = max_val if ok else 0
            print("---\t%s\t%d/%d" % (c_name, get_val, max_val))
            total_score += get_val
            max_score += max_val

        for a_id in self.advance_id_list:
            c_name = self.advance_cases[a_id]
            print("+++ TESTING advance case %s:" % c_name)
            ok = self.test_sample_case("advance", a_id)
            max_val = self.advance_case_scores[a_id]
            get_val = max_val if ok else 0
            print("---\t%s\t%d/%d" % (c_name, get_val, max_val))
            total_score += get_val
            max_score += max_val

        print("---\tTOTAL\t\t%d/%d" % (total_score, max_score))

        diff = open("{}/{}".format(self.output_dir, "diff.txt"), 'w')
        diff.write(self.diff_result)
        diff.close()

def main():
    parser = ArgumentParser()
    parser.add_argument("--scanner", help="scanner to grade", default="../src/scanner")
    parser.add_argument("--basic_case_id", help="basic case's ID", type=int, default=0)
    parser.add_argument("--advance_case_id", help="advance case's ID", type=int, default=0)
    args = parser.parse_args()

    g = Grader(scanner = args.scanner)
    g.get_case_id_list(args.basic_case_id, args.advance_case_id)
    g.run()

if __name__ == "__main__":
    main()