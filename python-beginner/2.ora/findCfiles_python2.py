import subprocess
import sys
import os
import time

'''
This program collect files and archive it 
for example, *.c files all C files etc...
Argument 1= rule of the files
Argument 2= folder name
'''




#sys.stderr.write('spam\n')

def make_dir(dirname):
	curr_path = os.path.abspath( dirname + "/")
	print(curr_path)
	if os.path.exists(curr_path) == True:
		return "EXISTS"
	else:
		args = ("mkdir", dirname)
		handler = subprocess.Popen(args, stdout=subprocess.PIPE)
		popen.wait()
		return "CREATED"


def get_filenames():
	args = ("ls", "*")
	popen = subprocess.Popen(args, stdout=subprocess.PIPE)
	popen.wait()
	temp2 = []
	while True:
		line = popen.stdout.readline()
		if line != '':
		#the real code does filtering here
			#print "test:", line.rstrip()
			for x in line.rstrip().split(" "):
				temp2.append(x)
		else:
			break
	return temp2



print "start main program"
print "print all files in directory"

if(len(sys.argv) != 3):
	print "usage: copy_c_files.py [search rule] [target directory]"
	sys.exit(1)

print "No of arguments:",len(sys.argv) , "arguments: ", sys.argv[1:] # 0. ertek a fajlnev(onmaga),!!!!!!!!!!!
print "***List of the files:"
for x in get_filenames():
	print x

backup_dir = sys.argv[2]


args = ("ls", sys.argv[1])
popen = subprocess.Popen(args, stdout=subprocess.PIPE)
popen.wait()
#output = popen.stdout.read()
#print output
temp2 = []
while True:
	line = popen.stdout.readline()
	if line != '':
		#print "test:", line.rstrip()
		for x in line.rstrip().split(" "):
			temp2.append(x)
	else:
		break

if(len(temp2) == 0):
	print "no files found with this rule, exiting..."
	sys.exit(1)

make_dir(backup_dir)
for x in temp2:
	args = ("cp", x, backup_dir + "/")
	popen = subprocess.Popen(args, stdout=subprocess.PIPE)
	popen.wait()


name = "c_files_backup" + str(time.strftime("%Y-%m-%d_%H-%M")) + ".gz"

popen = subprocess.Popen(["tar", "-zcvf", name , backup_dir], stdout=subprocess.PIPE)
popen.wait()

print "done..."
