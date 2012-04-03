#! /usr/bin/env python
import subprocess
import sys

def main():
    args = ' '.join(sys.argv[1:])
    p = subprocess.Popen('env CPUPROFILE=./hangman.prof ./a.out %s' % args, shell = True)
    p.wait()
    sys.exit(p.returncode)

if __name__ == '__main__':
    main()
