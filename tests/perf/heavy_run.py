#! /usr/bin/env python3
import os

BODY_COUNT = 100
TIME       = 200


os.system("time leapfrog demo 2 " + str(BODY_COUNT) + " -t " + str(TIME) + " -s -o res.dat -d 8 -b 80")
