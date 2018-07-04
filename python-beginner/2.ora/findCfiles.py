'''
Reqirements:
    Find all *.c files
    copy into one directory
    archive
    script cli parameter

    \t -> tabulator
    \n -> newline
    \r -> carriage return
progressbar
print("Copy C files starting...\n")
for x in range(0, 100, 10):
    print("progress:" + str(x) + " %", end='\r', flush=True)
    time.sleep(0.5)
'''
import time
import sys
import subprocess
import os

def make_dir(path="backup"):
    currPath = os.path.abspath(path + "/")
    print(currPath)
    if os.path.exists(currPath) == True:
        return "EXISTS"
    else:
        args = ["mkdir", path]
        runProcess(args)
        return "CREATED"
    
def runProcess(args):
    handler = subprocess.Popen(args, stdout=subprocess.PIPE)
    handler.wait()
    return handler.stdout.readlines()

print("Starting copyC program")

if len(sys.argv) != 3:
    print("usage: findCfiles.py [search wildcard] [target directory]")
    sys.exit(1)

print(" Argument num: " , len(sys.argv) , "arguments: ", sys.argv[1:])
listam = ["alma", "korte", "narancs", "kivi", "dinnye"]
print(listam[2:5])

backupDir = sys.argv[2]
sourceDir = sys.argv[1]
args = ["ls",sourceDir]
print(args)
popen = subprocess.Popen(args, stdout=subprocess.PIPE)
popen.wait()
#print(popen.stdout.read())
files = []
x = 0
line = popen.stdout.readlines()
for l in line:
    print(l.rstrip())
    files.append(l.rstrip().decode("utf-8"))

print(files)

if len(files) == 0:
    print("no files found")
    sys.exit(0)

make_dir(backupDir)

for f in files:
    args = ["cp", sourceDir + "/" + f, backupDir + "/"]
    runProcess(args)

archiveName = "backups" + str(time.strftime("%Y-%m-%d_%H-%M")) + ".gz"

runProcess(["tar", "-zcvf", archiveName, backupDir])

print("done")