'''
    XML fájlok beolvasása könyvtárból
    értelmezés, feldolgozás (minden részletre)
    XML fájlonként "kibányásszuk" az előadókat majd megszámoljuk és átlagoljuk a CD árát
    majd exportáljuk egy XML fájlba

'''

from lxml import etree
import os
import time

def GetXmlFiles(path="."):
    result = []
    for files in os.listdir(path):
        if files.find(".xml") > -1:
            result.append(path + "/" + files)
    print("result: %s" % result)
    return result

def CalcMean(list):
    result = 0
    for x in list:
        result += x
    return (result/len(list))

def ExportStatToXml(xml, stat):
    root = etree.Element("XmlStatForFiles")
    '''
    root.append(etree.Element("myChild1"))
    root[0].text = "asd"
    print(etree.tostring(root))
    '''
    for result in stat:
        root.append(etree.Element("XmlFile", 
                                    filename=result[0]['XmlName'], 
                                    numofartists=str(len(result[0]['Artists'])),
                                    meanofprices=str(CalcMean(result[1]))))
    output = etree.tostring(root, pretty_print=True).decode('ascii')
    fh = open(xml, 'w')
    fh.write(output)
    fh.close

def InvestigateXmlFiles(XmlFile):
    print("Xml file processing: ", XmlFile)
    Prices = []
    stat = {'Artists' : [], 'XmlName' : ""}
    stat['XmlName'] = XmlFile
    XmlTree = etree.parse(XmlFile)
    XmlRoot = XmlTree.getroot()
    print(XmlRoot.tag)
    for cd in XmlRoot:
        for content in cd:
            #print(content.text)
            if content.tag == "PRICE":
                Prices.append(float(content.text))
            if content.tag == "ARTIST":
                stat['Artists'].append(content.text)
        
    return (stat, Prices)

if __name__ == "__main__":
    print("start xml parser")
    path = "."
    XmlFiles = GetXmlFiles(path)
    XmlStats = []
    '''
    dates = ["2018-07-20", "07-20-2018", "20-07-2018", "2018"]
    outputDatetime = []
    for date in dates:
        try:
            outputDatetime.append(time.strptime(date, "%Y-%m-%d"))
        except ValueError:
            try:
                outputDatetime.append(time.strptime(date, "%d-%m-%Y"))
            except ValueError:
                try:
                    outputDatetime.append(time.strptime(date, "%m-%d-%Y"))
                except ValueError:
                    print("lol")
    print(outputDatetime)
    '''
    for Xml in XmlFiles:
        XmlStats.append(InvestigateXmlFiles(Xml))
    ExportStatToXml("xmlexport.xml", XmlStats)