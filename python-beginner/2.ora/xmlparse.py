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

def InvestigateXmlFiles(XmlFile):
    print("Xml file processing: ", XmlFile)
    output = []
    stat = {'NumOfArtist' : 0}
    XmlTree = etree.parse(XmlFile)
    XmlRoot = XmlTree.getroot()
    print(XmlRoot.tag)
    for cd in XmlRoot:
        for content in cd:
            #print(content.text)
            if content.tag == "COMPANY":
                output.append(content.text)
        
    return output

if __name__ == "__main__":
    print("start xml parser")
    path = "."
    XmlFiles = GetXmlFiles(path)
    for Xml in XmlFiles:
        print(InvestigateXmlFiles(Xml))