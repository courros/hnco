#!/usr/bin/env python

from __future__ import print_function

import argparse
import json
import os
import os.path
import subprocess
import sys
import traceback

class Application(object):

    def __init__(self):
        parser = argparse.ArgumentParser()
        parser.add_argument("--plan", type=str, help="JSON file which describes the experiment", default='plan.json')
        self.args = parser.parse_args()

    def main(self):
        try:
            self.load(self.args.plan)
            self.run_all()
        except:
            traceback.print_exc()
            return 1

    def load(self, path):
        with open(path) as file:
            self.plan = json.load(file)

    def run_all(self):
        self.path_results = self.plan['results']
        if not os.path.isdir(self.path_results):
            os.mkdir(self.path_results)
            print('Created directory {}'.format((self.path_results)))
        for f in self.plan['functions']:
            self.run_function(f)

    def run_function(self, f):
        id = f['id']
        print('Function {}'.format(id))
        path = '{}/{}'.format(self.path_results, id)
        if not os.path.isdir(path):
            os.mkdir(path)
            print('Created directory {}'.format((path)))
        for a in self.plan['algorithms']:
            self.run_algorithm(a, f)

    def run_algorithm(self, a, f):
        f_id = f['id']
        a_id = a['id']
        path = '{}/{}/{}'.format(self.path_results, f_id, a_id)
        if not os.path.isdir(path):
            os.mkdir(path)
            print('Created directory {}'.format((path)))
        sys.stdout.write('    Algorithm {}:'.format(a_id))
        if a.get('deterministic', False):
            self.run_algorithm_once(a, f, 0)
        else:
            for i in range(self.plan['num_runs']):
                self.run_algorithm_once(a, f, i)
        print('')

    def run_algorithm_once(self, a, f, i):
        f_id = f['id']
        a_id = a['id']
        path = '{}/{}/{}/log.{}'.format(self.path_results, f_id, a_id, i)
        file_out = open(path, 'w')
        file_err = open('log.err', 'a')
        args = '{} {} {} {}'.format(a.get('exec', self.plan['exec']), self.plan['opt'], f['opt'], a['opt']).split()
        status = subprocess.call(args, stdout=file_out, stderr=file_err)
        if status == 0:
            sys.stdout.write('.')
        else:
            sys.stdout.write('X')

if __name__ == '__main__':
    application = Application()
    exit(application.main())
