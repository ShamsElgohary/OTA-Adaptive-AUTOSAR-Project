import xml.etree.ElementTree as ET          
from DataType import DataTypeParser
from Service_Interface import ServiceInfParser
from Deployment import DeploymentParser
from Mapping_SwCompounent import MappingParser, SWParser
import json

class Generator:
    def __init__(self):
        self.CurrentXMLString = ""
        self.Name = ""
        self.Machines = {}
        self.ServiceInterfaces = {}
        self.DataTypes = {}
        self.Deployments = {}
        self.Deployments_Manifest = [
                                ["PackageManager_provided_instance", 2 ,1],
                                ["PackageManager_required_instance", 2 ,1], 
                                ["UpdateRequest_provided_instance", 1 ,1], 
                                ["UpdateRequest_requested_instance", 1 ,1]
                            ]
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
        self.DataTypes = DT.Parse()

    def __ScanSI(self):
        print("Scanning Service Interfaces")
        SI = ServiceInfParser(self.CurrentXMLString)
        self.ServiceInterfaces = SI.Parse()

    def __ScanSC(self):
        print("Scanning Software Compounents")
        SWC = SWParser(self.CurrentXMLString)
        self.SoftwareCompounents = SWC.Parse()

    def __ScanD(self):
        print("Scanning Deployments")
        Dep = DeploymentParser(self.CurrentXMLString)
        self.Deployments = Dep.Parse()

    def __ScanMP(self):
        print("Scanning Mappings")
        Map = MappingParser(self.CurrentXMLString)
        self.Mapping = Map.Parse()


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

        for Map in self.Mapping:
            for Inst in self.Deployments_Manifest:
                if Inst[0] == Map[0]:
                    Map.append(Inst[1])
                    Map.append(Inst[2])
                    
        Manifests = {}
        for SoftwareCompounent in self.SoftwareCompounents:
            Swc_name = SoftwareCompounent[0]
            Manifests[Swc_name] = {"P_PORT":[], "R_PORT" : []}
            for Port in SoftwareCompounent[1]:
                P_name = Port[1]
                for Map in self.Mapping:
                    if Map[2] == Swc_name and Map[3] == P_name:
                        if Port[0] == "P-PORT name":
                            temp = []
                            temp.append(Port[1])
                            temp.append(Map[1])
                            temp.append(Map[4])
                            temp.append(Map[5])
                            Manifests[Swc_name]["P_PORT"].append(temp)
                        elif Port[0] == "R-PORT name":
                            temp = []
                            temp.append(Port[1])
                            temp.append(Map[4])
                            temp.append(Map[5])
                            Manifests[Swc_name]["R_PORT"].append(temp)
        
        
        

        print(Manifests)
        return Manifests

    [
        ["PackageManager_provided_instance", 2 ,1],
        ["PackageManager_required_instance", 2 ,1], 
        ["UpdateRequest_provided_instance", 1 ,1], 
        ["UpdateRequest_requested_instance", 1 ,1]
    ]

    {
        'ucm_swc': 
        {
            'P_PORT': [['package_manager', '2253', 2, 1]], 
            'R_PORT': [['update_request', 1, 1]]
        }, 
        'ota_swc': 
        {
            'P_PORT': [], 
            'R_PORT': [['package_manager', 2, 1]]
        }, 
        'sm_swc': 
        {
            'P_PORT': [['update_request', '2522', 1, 1]], 'R_PORT': []
        }
    }