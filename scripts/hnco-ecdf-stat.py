#!/usr/bin/env python

from __future__ import print_function

import argparse
import json
import subprocess
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
        for f in self.plan['functions']:
            self.run_function(f)

    def run_function(self, f):
        id = f['id']
        print('Function {}'.format(id))
        path = '{}/{}'.format(self.path_results, id)
        for a in self.plan['algorithms']:
            self.run_algorithm(a, f)

    def run_algorithm(self, a, f):
        f_id = f['id']
        a_id = a['id']
        path = '{}/{}/{}'.format(self.path_results, f_id, a_id)
        if a.get('deterministic', False):
            pass
        else:
            for i in range(self.plan['num_runs']):
                pass

if __name__ == '__main__':
    application = Application()
    exit(application.main())
