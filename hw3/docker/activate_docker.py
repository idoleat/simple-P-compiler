#! /usr/bin/env python3

# This dockerfile is intended to be used by students
import sys
import subprocess as sp
import pathlib
import argparse
from pathlib import Path
import os

try:
    assert sys.version_info >= (3, 6)
except AssertionError:
    print("Requires at least python 3.6")

COURSE_NAME = 'compiler-s20'

parser = argparse.ArgumentParser(
    description=f'Activate homework environment for {COURSE_NAME}')
parser.add_argument( '-t','--test-src-fld',
                    nargs=1,
                    default=[None],
                    help="Append different test folder")
parser.add_argument( '-u','--username',default="student",)
parser.add_argument( '--hostname', default=f'{COURSE_NAME}')
parser.add_argument( '--homedir', default='/home/student')
parser.add_argument( '-i','--imagename', default=f'{COURSE_NAME}-env')
parser.add_argument( 'command', default=[], nargs=argparse.REMAINDER, help="command to run in docker")


args = parser.parse_args()
test_src_fld = args.test_src_fld[0]
DOCKER_USER_NAME = args.username
DOCKER_HOST_NAME = args.hostname
DOCKER_IMG_NAME = args.imagename
dk_home = args.homedir

DOCKER_CMD = args.command

dirpath = os.path.dirname(os.path.abspath(__file__))

recurisive_docker_msg='''
    .
    .
   . ;.
    .;
     ;;.
   ;.;;
   ;;;;.
   ;;;;;
   ;;;;;
   ;;;;;
   ;;;;;    Don't activate environment twice QQ
   ;;;;;
 ..;;;;;..  You are already inside our docker environment, see?
  ':::::'
    ':`                              - SSLAB @NCTUCS
'''

def main():
    if "STATUS_DOCKER_ACTIVATED" in os.environ:
        print(recurisive_docker_msg)
        sys.exit(0)

    # print(f'dirpath :{dirpath}')
    cwd = os.getcwd()

    bash_his = Path(f'{dirpath}/.history/docker_bash_history')
    bash_his.parent.mkdir(exist_ok=True)
    bash_his.touch(exist_ok=True)

    if test_src_fld and not Path(test_src_fld).exists():
        raise FileNotFoundError(f"Folder: `{test_src_fld}` doesnt exist.")

    docker_options = [
        'docker', 'run',
        '--rm',
        '--hostname', DOCKER_HOST_NAME,
        '--cap-add=SYS_PTRACE',
        '-e', f'LOCAL_USER_ID={os.getuid()}',
        '-e', f'LOCAL_USER_GID={os.getgid()}',
        '-v', f'{os.getcwd()}:/home/{DOCKER_USER_NAME}',

        # bash history file
        '-v', f'{dirpath}/.history/docker_bash_history:/{dk_home}/.bash_history',
    ]
    if test_src_fld:
        docker_options.append(f'-v {os.path.abspath(test_src_fld)}:{dk_home}/test')
    if len(DOCKER_CMD) == 0:
        docker_options.append(f'-it')
    else:
        docker_options.extend(['-a', 'stderr'])
        docker_options.extend(['-a', 'stdout'])

    cmd_list = docker_options + [DOCKER_IMG_NAME] + DOCKER_CMD
    return os.WEXITSTATUS(os.system(' '.join(cmd_list)))

if __name__ == "__main__":
    exit_status = main()
    sys.exit(exit_status)