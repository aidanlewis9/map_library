#!/usr/bin/env python2.7

import os
import sys
import commands

i = 1
load_factors = [.5, .75, .9, 1.0, 2.0, 4.0, 8.0, 16.0]

headers = "|\tNITEMS\t|\tALPHA\t|\tTIME\t\t|\tSPACE\t\t|\n"
lines = "---------------------------------------------------------------------------------\n"
body = headers + lines

while i <= 10000000:
  for load_factor in load_factors:
    trash, data = commands.getstatusoutput("shuf -i1-100000000 -n {} | ./measure ./freq -l {} > /dev/null".format(i, load_factor))
    formatted = data.split('\t');
    time = formatted[0].split(' ')[0];
    memory = formatted[1].split(' ')[0];
    body += "|\t{}\t|\t{}\t|\t{}\t|\t{}\t|\n".format(i, load_factor, time, memory)
  i *= 10;
print(body);
