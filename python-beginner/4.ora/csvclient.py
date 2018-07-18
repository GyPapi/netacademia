import urllib3
import csv
import random
import json
'''
http = urllib3.PoolManager()

r = http.request('GET', 'http://api.ipify.org')

print(r.data)

'''
def generateData():
    data = {"dht22": [], "lm35" : []}
    for x in range(0, 10):
        data['dht22'].append(random.randint(10,20))
        data['lm35'].append(random.randint(10,20))
    print(data)
    return data


def createCsv(data):
    with open('datas.csv', 'w', newline='') as csvfile:
        cswrite = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
        cswrite.writerow(["lm35"]+ ["dht22"])
        for x in range(0, len(data['dht22'])):
            cswrite.writerow([data['dht22'][x], data['lm35'][x]])

def readCsv():
    with open('datas.csv', newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=' ', quotechar='|')
        for row in reader:
            print(', '.join(row))

class httpApi(object):
    def __init__(self, address):
        self.http = urllib3.PoolManager()
        self.address = address

    def PutCsv(self, path, filepath):
        with open(filepath) as fp:
            data = fp.read()
        r = self.http.request('POST',self.address + path,fields={'file': ("asd"+filepath, data, 'text/plain')})
        #json.loads(r.data.decode('utf-8'))
        print(r.data.decode('utf-8'))
    
    def GetFile(self, path):
        print("start getting")
        r = self.http.request('GET', self.address + path, preload_content=False)
        print(r.data)
        return r



if __name__ == '__main__':
    print("generatingcsv starting...")
    random.seed(556326)
    data = generateData()
    createCsv(data)
    #readCsv()
    conn = httpApi('127.0.0.1:5000')
    conn.GetFile('/uploads/sad.txt')
    conn.PutCsv("/", 'datas.csv')
