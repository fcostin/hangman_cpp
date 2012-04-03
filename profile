#! /usr/bin/env python
import subprocess
import sys

def main():
    arg_string = ' '.join(sys.argv[1:])
    params = {
        'CPUPROFILE' : './hangman.prof',
        'CPUPROFILE_FREQUENCY' : 10000,
    }
    param_string = ' '.join(['%s=%s' % (str(k), str(v)) for (k, v) in params.iteritems()])
    command = 'env %s ./a.out %s' % (param_string, arg_string)
    print 'running "%s"' % command
    p = subprocess.Popen(command, shell = True)
    p.wait()
    sys.exit(p.returncode)

if __name__ == '__main__':
    main()
