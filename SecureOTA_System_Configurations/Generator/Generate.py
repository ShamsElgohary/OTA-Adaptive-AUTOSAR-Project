import xml.etree.ElementTree as ET          
from DataType import DataTypeParser
from Service_Interface import ServiceInfParser
from Deployment import DeploymentParser
from Mapping_SwCompounent import MappingParser, SWParser

class Generator:
    def __init__(self):
        self.CurrentXMLString = ""
        self.Name = ""
        self.Machines = {}
        self.ServiceInterfaces = {}
        self.DataTypes = {}
        self.Deployments = {}
        self.SoftwareCompounents = {}
        self.Mapping = {}
    
    def Scan (self, xmlstring, fileType):
        self.CurrentXMLString = xmlstring

        if fileType == "machines":
            return self.__ScanMC()

        elif fileType == "datatype":
            if self.__CheckPName() == 1:
                return 1
            else:
                return self.__ScanDT()

        elif fileType == "service_interfaces":
            if self.__CheckPName() == 1:
                return 1
            else:
                return self.__ScanSI()

        elif fileType == "software_compoenents":
            if self.__CheckPName() == 1:
                return 1
            else:
                return self.__ScanSC()

        elif fileType == "deployment":
            if self.__CheckPName() == 1:
                return 1
            else:
                return self.__ScanD()

        elif fileType == "mappings":
            if self.__CheckPName() == 1:
                return 1
            else:
                return self.__ScanMP()
        else:
            return 2

    def __ScanMC(self):
        print("Scanning Machines")
        root = ET.fromstring(self.CurrentXMLString)
        
        prelist = root.tag.split("}")
        pre = prelist[0] + "}"

        # FIND PROJECT NAME
        Tempnode = root.find(pre + "AR-PACKAGES")
        Tempnode = Tempnode.find(pre + "AR-PACKAGE")
        self.Name = Tempnode.find(pre + "SHORT-NAME").text

        found = False

        for Tempnode in root.iter(pre + "AR-PACKAGE"):
            print(Tempnode.find(pre + "SHORT-NAME").text)
            if Tempnode.find(pre + "SHORT-NAME").text == "machines":
                found = True
                RootElements = Tempnode.find(pre + "ELEMENTS")
                for Tempnode1 in RootElements.iter(pre + "MACHINE-DESIGN"):
                    self.Machines[Tempnode1.find(pre + "SHORT-NAME").text] = {}
                    RootConnectors = Tempnode1.find(pre + "COMMUNICATION-CONNECTORS")
                    for Tempnode2 in RootConnectors:
                        self.Machines[Tempnode1.find(pre + "SHORT-NAME").text][Tempnode2.find(pre + "SHORT-NAME").text] = Tempnode2.tag

                for Tempnode1 in RootElements.iter(pre + "MACHINE"):
                    self.Machines[Tempnode1.find(pre + "SHORT-NAME").text] = self.Machines[Tempnode1.find(pre + "MACHINE-DESIGN-REF").text.split("/")[-1]]      

        if found:
            return 0
        else:
            return 3

    
    def __ScanDT(self):
        print("Scanning DataTypes")
        DT = DataTypeParser(self.CurrentXMLString)
        DT.Parse()

    def __ScanSI(self):
        print("Scanning Service Interfaces")
        SI = ServiceInfParser(self.CurrentXMLString)
        SI.Parse()

    def __ScanSC(self):
        print("Scanning Software Compounents")
        SWC = SWParser(self.CurrentXMLString)
        SWC.Parse()

    def __ScanD(self):
        print("Scanning Deployments")
        Dep = DeploymentParser(self.CurrentXMLString)
        Dep.Parse()

    def __ScanMP(self):
        print("Scanning Mappings")
        Map = MappingParser(self.CurrentXMLString)
        Map.Parse()


    def __CheckPName(self):
        root = ET.fromstring(self.CurrentXMLString)
        
        prelist = root.tag.split("}")
        pre = prelist[0] + "}"

        # FIND PROJECT NAME
        Tempnode = root.find(pre + "AR-PACKAGES")
        Tempnode = Tempnode.find(pre + "AR-PACKAGE")

        if self.Name != Tempnode.find(pre + "SHORT-NAME").text:
            return 1
        else:
            return 0

    def GenerateSkeleton(self):
        Skeleton = "Generate Interface Skeleton"
        print("Generate Interface Skeleton")
        return Skeleton

    def GenerateProxy(self):
        Proxy = "Generate Interface Proxy"
        print("Generate Interface Proxy")
        return Proxy

    def GenerateManifest(self):
        Manifest = "Generate Interface Manifest"
        print("Generate Interface Manifest")
        return Manifest



