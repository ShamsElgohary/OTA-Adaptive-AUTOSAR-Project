from typing import Mapping
import xml.etree.ElementTree as ET

class MappingParser:
    def __init__(self, xmlString):
        self.xmlString = xmlString
        self.valuesPorts = []
        self.valuesSWC = []
        self.values = []
        self.root = None
        print("Mapping Parser")

    def Parse(self):
        self.root = ET.fromstring(self.xmlString)

        for child in self.root.findall('.//{http://autosar.org/schema/r4.0}SOMEIP-SERVICE-INSTANCE-TO-MACHINE-MAPPING'):
            data = []
            name = child.find('.//{http://autosar.org/schema/r4.0}SHORT-NAME')
            port = child.find('.//{http://autosar.org/schema/r4.0}TCP-PORT')
            path = child.find('.//{http://autosar.org/schema/r4.0}SERVICE-INSTANCE-REF').text
            tem = path.rfind("/")
            D_name = path[tem+1:]
            data.append(name.text)
            data.append(port.text)
            data.append(path)
            data.append(D_name)
            self.valuesPorts.append(data)

        for child in self.root.findall('.//{http://autosar.org/schema/r4.0}SERVICE-INSTANCE-TO-PORT-PROTOTYPE-MAPPING'):
            data = []
            name = child.find('.//{http://autosar.org/schema/r4.0}SHORT-NAME')
            swcPath = child.find('.//{http://autosar.org/schema/r4.0}TARGET-PORT-PROTOTYPE-REF')
            instancePath = child.find('.//{http://autosar.org/schema/r4.0}SERVICE-INSTANCE-REF')
            
            path = swcPath.text
            tem = path.rfind("/")
            P_name = path[tem+1:]
            path = path[0:tem]
            tem = path.rfind("/")
            Swc_name = path[tem+1:]
            data.append(name.text)
            data.append(swcPath.text)
            data.append(instancePath.text)
            data.append(Swc_name)
            data.append(P_name)
            
            self.valuesSWC.append(data)

        for valPort in self.valuesPorts:
            for valSWC in self.valuesSWC:
                if valPort[2] == valSWC[2]:
                    temp = []
                    temp.append(valPort[3])
                    temp.append(valPort[1])
                    temp.append(valSWC[3])
                    temp.append(valSWC[4])
                    self.values.append(temp)
        

        print(self.values)
        return self.values
                




class SWParser:
    def __init__(self, xmlString):
        self.xmlString = xmlString
        self.SWC = []
        print("SW Parser")

    def Parse(self):
        root = ET.fromstring(self.xmlString)
        Mapping = []
        for child in root.findall('.//{http://autosar.org/schema/r4.0}ADAPTIVE-APPLICATION-SW-COMPONENT-TYPE'):
            myMap = []
            name = child.find('.//{http://autosar.org/schema/r4.0}SHORT-NAME')
            p = child.findall('.//{http://autosar.org/schema/r4.0}P-PORT-PROTOTYPE')
            if p:
                for i in p:
                    name_p = i.find('.//{http://autosar.org/schema/r4.0}SHORT-NAME')
                    temp = []
                    temp.append("P-PORT name")
                    temp.append(name_p.text)
                    myMap.append(temp)

            R = child.findall('.//{http://autosar.org/schema/r4.0}R-PORT-PROTOTYPE')
            if R:
                for i in R:
                    name_r = i.find('.//{http://autosar.org/schema/r4.0}SHORT-NAME')
                    temp = []
                    temp.append("R-PORT name")
                    temp.append(name_r.text)
                    myMap.append(temp)
            tempDect = []
            tempDect.append(name.text)
            tempDect.append(myMap)
            self.SWC .append(tempDect)
        print (self.SWC)
        return self.SWC
        