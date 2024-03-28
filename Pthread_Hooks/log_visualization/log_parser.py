#!/usr/bin/python3

import pandas as pd
from tabulate import tabulate
import textwrap

import argparse

parser = argparse.ArgumentParser(description='Pthread Hooks Log Parser')
parser.add_argument('-f', "--file", metavar='LOG_FILE', dest="log_file",
                    help='path to the log file')

args = parser.parse_args()

if args.log_file is None:
    print("No log file specified")
    parser.print_help()
    quit()

f = open(args.log_file, 'r')

# read and parse the log file
logs = []
threads = []
for line in f:
    timestamp = int(line.split(' ')[1])
    thread_id = int(line.split(' ')[3].rstrip(':'))
    message = ' '.join(line.rstrip('\n').split(' ')[4:])
    logs.append({"timestamp":timestamp,"tid":thread_id,"message":message})

    if not (thread_id in threads):
        threads.append(thread_id)

threads = sorted(threads)
#print(logs)
#print(threads)

# constructed the list of logs by threads
header_row = ['timestamp'] + [ "th " + str(t) for t in threads]
rows = []
for l in logs:
    message = l["message"]
    timestamp = l["timestamp"]
    thread_id = l['tid']
    row = [timestamp]
    for t in threads:
        if thread_id == t:
            row.append('\n'.join(textwrap.wrap(message,width=20)))
            #row.append(message)
        else:
            row.append("x")
    rows.append(row)

#print(rows)

# convert to pandas dataframe 
df = pd.DataFrame(rows, columns=header_row)
df.sort_values('timestamp', inplace = True)
df.reset_index(drop = True, inplace = True)
pd.options.display.max_columns = len(df.columns)
pd.set_option('display.max_colwidth', 20)
print(tabulate(df,headers=list(df), tablefmt="grid"))
df.to_csv("log.csv")
df

    

    
