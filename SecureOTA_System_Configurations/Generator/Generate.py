from poplib import POP3_PORT
import xml.etree.ElementTree as ET          
from DataType import DataTypeParser
from Service_Interface import ServiceInfParser
from proxy_gen import proxy_generator
from skeleton_gen import skeleton_generator
from Deployment import DeploymentParser
from Mapping_SwCompounent import MappingParser, SWParser
import json

class Generator:
    def __init__(self):
        self.CurrentXMLString = ""
        self.Name = ""
        self.Machines = {}
        self.DataTypes = {}
        self.Deployments = {}
        self.Deployments_Manifest = []
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
        self.SI = ServiceInfParser(self.CurrentXMLString)
        self.SI.Parse()

    def __ScanSC(self):
        print("Scanning Software Compounents")
        SWC = SWParser(self.CurrentXMLString)
        self.SoftwareCompounents = SWC.Parse()

    def __ScanD(self):
        print("Scanning Deployments")
        Dep = DeploymentParser(self.CurrentXMLString)
        self.Deployments, self.Deployments_Manifest = Dep.Parse()

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
        print("Generate Interface Skeleton")
        skeleton_generator(self.SI, self.DataTypes, self.Deployments)

    def GenerateProxy(self):
        print("Generate Interface Proxy")
        proxy_generator(self.SI, self.DataTypes, self.Deployments)

    def GenerateManifest(self):
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
        for swc in self.SoftwareCompounents:
            name=swc[0]
            #######PROVIDED#########
            port=[]
            p_list=[]
            list1=[]
            list2=[]
            provided_list=Manifests[name]['P_PORT']
            for list in provided_list:
                dictionary={}
                dictionary["port"] = list[1]
                dictionary["service_id"]=list[2]
                dictionary["instance_id"]=list[3]
                dictionary["type"]="SOME/IP"
                dictionary["ipv4"]="127.0.0.1"
                list1.append(dictionary)
                print("list 1",list1)
            #######REQUIRED#########
            required_list=Manifests[name]['R_PORT']
            for list in required_list:
                dictionary2={}
                dictionary2["service_id"]=list[1]
                dictionary2["instance_id"]=list[2]
                dictionary2["type"]="SOME/IP"
                list2.append(dictionary2)


            datajson = {
                    "ap_service_instances":{
                    "required_ap_service_instances":list2,
                    "provided_ap_service_instances":list1
                    }
                }
            with open("executables/"+name+"/0.1/etc/"+" service_manifest2.json", 'w') as f:
                json.dump(datajson, f, 
                                    indent=4,  
                                    separators=(',',': '))
                f.close()
