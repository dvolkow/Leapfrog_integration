#! /usr/bin/env python3
import os
import itertools

symm    = [" -s ", ""]
bits    = [" -b 80 ", ""]
prec = [" -d 8 ", " -p 0.000001 "]
hamout  = [" -e h.dat ", ""]
outf    = [" -o out.dat ", ""]

N       = 4

PREFIX_RUNNED = "leapfrog demo 2 4 -t 10"

options = itertools.product(symm, hamout, outf, prec, bits)

print("===============================")
print("Hello from Leapfrog smoke test!")
print("===============================")

i = 1
fails   = 0
success = 0
for opt in options:
    print("\nTest #", i, ": ", PREFIX_RUNNED + ''.join(opt))
    ret = os.system(PREFIX_RUNNED + ''.join(opt))
    print("[FAIL]" if ret != 0 else "[OK]")
    if ret != 0:
        fails += 1
    else:
        success += 1
    i += 1

print("\nSuccess: ", success, " |",  str(float(success) / (i - 1) * 100), "%")
print("Fail:    ", fails, " |",  str(float(fails) / (i - 1) * 100), "%")

os.system("rm *.dat")
