import xml.etree.ElementTree as ET

ToCPP = {
    "boolean": "bool",
    "uint8" : "uint8_t",
    "uint16" : "uint16_t",
    "uint32" : "uint32_t",
    "uint64" : "uint64_t",
    "sint8" : "sint8_t",
    "sint16" : "sint16_t",
    "sint32" : "sint32_t",
    "sint64" : "sint64_t",
    # (Not Sure if it's defined in Cpp or not)
    "float32" : "float32_t",
    "float64" : "float64_t"
}

class DataType:
    def __init__(self, AllTypes,root):
        self.Cat = root.find("CATEGORY").text
        self.Name = root.find("SHORT-NAME").text
        if self.Cat == "FIXED_LENGTH":
            self.Implementation = None
            self.Decleration = ToCPP[self.Name]

        elif self.Cat == "TYPE_REFERENCE":
            # Should I Use Ordinary Pointers or Shared Smart Pointers
            self.Implementation = None
            RefTypePath = root.find("SW-DATA-DEF-PROPS").find("SW-DATA-DEF-PROPS-VARIANTS").find("SW-DATA-DEF-PROPS-CONDITIONAL")
            if (RefTypePath.find("BASE-TYPE-REF")) is not None:
                Type = RefTypePath.find("BASE-TYPE-REF").text.split("/")[-1]
                self.Decleration = f'{AllTypes.Types["BaseTypes"][Type].Decleration} *'
            elif (RefTypePath.find("IMPLEMENTATION-DATA-TYPE-REF")) is not None:
                Type = RefTypePath.find("IMPLEMENTATION-DATA-TYPE-REF").text.split("/")[-1]
                self.Decleration = f'{AllTypes.Types["ImplementationDataTypes"][Type].Decleration} *'

        elif self.Cat == "VALUE":
            self.Implementation = None
            RefTypePath = root.find("SW-DATA-DEF-PROPS").find("SW-DATA-DEF-PROPS-VARIANTS").find("SW-DATA-DEF-PROPS-CONDITIONAL")
            if (RefTypePath.find("BASE-TYPE-REF")) is not None:
                Type = RefTypePath.find("BASE-TYPE-REF").text.split("/")[-1]
                self.Decleration = AllTypes.Types["BaseTypes"][Type].Decleration
            elif (RefTypePath.find("IMPLEMENTATION-DATA-TYPE-REF")) is not None:
                Type = RefTypePath.find("IMPLEMENTATION-DATA-TYPE-REF").text.split("/")[-1]
                self.Decleration = AllTypes.Types["ImplementationDataTypes"][Type].Decleration
        
        elif self.Cat == "VECTOR":
            self.Implementation = None
            self.Decleration = f'std::vector <{(DataType(AllTypes, root.find("SUB-ELEMENTS").find("IMPLEMENTATION-DATA-TYPE-ELEMENT"))).Decleration}>'
        
        elif self.Cat == "STRUCTURE":
            self.Decleration = self.Name
            elements = []
            ElementsPath = root.find("SUB-ELEMENTS")
            for elem in ElementsPath.findall("IMPLEMENTATION-DATA-TYPE-ELEMENT"):
                elements.append(DataType(AllTypes, elem))

            StringElements = ""
            for selem in elements:
                StringElements = StringElements + f'\t{selem.Decleration} {selem.Name};\n'
            self.Implementation = 'struct \n{' + f'{self.Name} \n{StringElements}' + '};'



class DataTypes:    
    def __init__(self, root):
        self.Types = {}
        self.Types["BaseTypes"] = {}
        self.Types["ImplementationDataTypes"] = {}
        for roottypenode in root.iter("AR-PACKAGE"):
            if  roottypenode.find("SHORT-NAME").text == "BaseTypes":
                BaseElements = roottypenode.find("ELEMENTS")
                for BaseType in BaseElements.findall("SW-BASE-TYPE"):
                    self.Types["BaseTypes"][BaseType.find("SHORT-NAME").text] = DataType(self, BaseType)

            elif  roottypenode.find("SHORT-NAME").text == "ImplementationDataTypes":
                ImpElements = roottypenode.find("ELEMENTS")
                for ImpType in ImpElements.findall("IMPLEMENTATION-DATA-TYPE"):
                    self.Types["ImplementationDataTypes"][ImpType.find("SHORT-NAME").text] = DataType(self, ImpType)
    def Resolve (self, String):
        MajorType = String.split("/")[-2]
        MinorType = String.split("/")[-1]
        return self.Types[MajorType][MinorType]
                

class Event:
    def __init__(self, root, AllTypes):
        self.EventName = root.find("SHORT-NAME").text
        self.DatatypePath = root.find("TYPE-TREF").text
        self.Datatype = AllTypes.Resolve(self.DatatypePath)
        self.ID = 1

    # Generate Based on Generation formate
    def GenerateSkeleton(self, TabsNum):
        Skeleton = "\t" * TabsNum + "class " + self.EventName + " : public ara::com::internal::SkeletonEvent\n" + "\t" * TabsNum + "{\n"
        TabsNum += 1
        Skeleton += "\t" * TabsNum + "public:\n"
        TabsNum += 1
        Skeleton += "\t" * TabsNum + "using SampleType = " + self.Datatype.Decleration + ";\n"
        TabsNum -= 2
        Skeleton += "\t" * TabsNum + "};\n"

        Skeleton += "\t" * TabsNum + f'{self.EventName} {self.EventName};\n'

        return Skeleton

    # Generate Based on Generation formate
    def GenerateProxy(self, TabsNum):
        Proxy = "\t" * TabsNum + "class " + self.EventName + " : public ara::com::internal::EventProxy\n" + "\t" * TabsNum + "{\n"
        TabsNum += 1
        Proxy += "\t" * TabsNum + "public:\n"
        TabsNum += 1
        Proxy += "\t" * TabsNum + "using SampleType = " + self.Datatype.Decleration + ";\n"
        Proxy +=  "\t" * TabsNum + self.EventName + f"BrakeEvent(ara::com::internal::Proxy::HandleType& Handle) : EventProxy(Handle, {self.ID})\n"
        Proxy +=  "\t" * TabsNum + "{}\n"
        TabsNum -= 2
        Proxy += "\t" * TabsNum + "};\n"

        return Proxy

 # (Find Field Formate in XML)
class Field:
    def __init__(self, root, AllTypes):
        self.FieldName = root.find("SHORT-NAME").text
        self.DatatypePath = root.find("TYPE-TREF").text
        self.Datatype = AllTypes.Resolve(self.DatatypePath)
        self.FNotifier = root.find("NOTIIER").text == True
        self.FSetter = root.find("SETTER").text == True
        self.FGetter = root.find("GETTER").text == True
        self.ID = 2
    
    # Generate Based on Generation formate
    def GenerateSkeleton(self, TabsNum):
        Skeleton = "\n\t" + self.FieldName + "\n\t{"
        Skeleton = Skeleton + "\n\t\t DataType:" + self.DatatypePath 
        Skeleton = Skeleton + "\n\t\t Getter:" + str (self.FGetter) 
        Skeleton = Skeleton + "\n\t\t Setter:" + str (self.FSetter)
        Skeleton = Skeleton + "\n\t\t Notifier:" + str (self.FNotifier) 
        Skeleton = Skeleton + "\n\t}\n"
        return Skeleton

    # Generate Based on Generation formate
    def GenerateProxy(self, TabsNum):
        Proxy = "\n\t" + self.FieldName + "\n\t{"
        Proxy = Proxy + "\n\t\t DataType:" + self.DatatypePath 
        Proxy = Proxy + "\n\t\t Getter:" + str(self.FGetter) 
        Proxy = Proxy + "\n\t\t Setter:" + str (self.FSetter)
        Proxy = Proxy + "\n\t\t Notifier:" + str (self.FNotifier) 
        Proxy = Proxy + "\n\t}\n"
        return Proxy

class Method:
    def __init__(self, root, AllTypes):
        self.MethodName = root.find("SHORT-NAME").text
        self.Arguments = []
        self.OneWay = True
        rootArgs = root.find("ARGUMENTS")
        self.ID = 3
        for rootArg in rootArgs.findall("ARGUMENT"):
            Temp = {}
            Temp["Name"] = rootArg.find("SHORT-NAME").text
            Temp["DatatypePath"] = rootArg.find("TYPE-TREF").text
            Temp["Datatype"] = AllTypes.Resolve(Temp["DatatypePath"])
            if rootArg.find("DIRECTION").text == "IN":
                Temp["IN"] = True
                Temp["OUT"] = False

            elif rootArg.find("DIRECTION").text == "OUT":
                self.OneWay = False
                Temp["IN"] = False
                Temp["OUT"] = True
            
            elif rootArg.find("DIRECTION").text == "INOUT":
                self.OneWay = False
                Temp["IN"] = True
                Temp["OUT"] = True
            self.Arguments.append(Temp)
    
    # Generate Based on Generation formate
    def GenerateSkeleton(self, TabsNum):
        Skeleton = "\n\t" + self.MethodName + "\n\t{"
        Skeleton = Skeleton + "\n\t\t OneWay:" + str (self.OneWay) 
        for arg in self.Arguments:
            Skeleton = Skeleton + "\n\t\t{"
            Skeleton = Skeleton + "\n\t\t\t Name: " + arg["Name"]
            Skeleton = Skeleton + "\n\t\t\t DataType: " + arg["DatatypePath"]
            Skeleton = Skeleton + "\n\t\t\t IN: " + str (arg["IN"])
            Skeleton = Skeleton + "\n\t\t\t OUT: " + str (arg["OUT"])
            Skeleton = Skeleton + "\n\t\t}"
        Skeleton = Skeleton + "\n\t}\n"
        return Skeleton

    # Generate Based on Generation formate
    def GenerateProxy(self, TabsNum):
        Proxy = "\t" * TabsNum + f"struct {self.MethodName}Output\n" + "\t" * TabsNum + "{\n"
        TabsNum += 1
        for arg in self.Arguments:
            if arg["OUT"] == True:
                Proxy += "\t" * TabsNum + arg["Datatype"].Decleration + " " + arg["Name"] + ";\n"
        TabsNum -= 1
        Proxy += "\t" * TabsNum + "};\n"

        Proxy += "\t" * TabsNum + f"class {self.MethodName} : public ara::com::internal::MethodProxy <{self.MethodName}Output>\n" + "\t" * TabsNum + "{\n"
        TabsNum += 1

        # PUBLIC #
        Proxy += "\t" * TabsNum + "public:\n"
        TabsNum += 1

        # CONSTRUCTOR #
        Proxy +=  "\t" * TabsNum + self.MethodName+ f"(ara::com::internal::Proxy::HandleType& Handle) : MethodProxy(Handle, {self.ID})\n" + "\t" * TabsNum + "{}\n"

        TabsNum -= 2
        Proxy += "\t" * TabsNum + "};\n"        

        return Proxy

class ServiceInterface:
    def __init__(self, root, Datatypes):
        # Root = SERVICE-INTERFACE
        self.rootServiceInterface = root
        self.TabsNum = 0
        self.DataTypes = Datatypes
        self.ServiceName = root.find("SHORT-NAME").text
        self.NameSpaces = []
        self.Events = []
        self.Methods = []
        self.Fields = []

        self.__Scan()
    
    def __Scan (self):
        # Scan Namespaces
        temprootNS = self.rootServiceInterface.find("NAMESPACES")
        for ns in temprootNS.findall("SYMBOL-PROPS"):
            self.NameSpaces.append(ns.find("SYMBOL").text)
        
        # Scan Events
        temprootE = self.rootServiceInterface.find("EVENTS")
        for e in temprootE.findall("VARIABLE-DATA-PROTOTYPE"):
            self.Events.append(Event(e, self.DataTypes))

        # Scan Fields (Find Field Formate in XML)
        temprootF = self.rootServiceInterface.find("FIELDS")
        for f in temprootF.findall("VARIABLE-DATA-PROTOTYPE"):
            self.Fields.append(Field(f, self.DataTypes))

        # Scan Methods
        temprootM = self.rootServiceInterface.find("METHODS")
        for m in temprootM.findall("CLIENT-SERVER-OPERATION"):
            self.Methods.append(Method(m, self.DataTypes))
    

    # Generate Based on Generation formate
    def GenerateSkeleton(self):
        rtn = []
        rtn.append(self.ServiceName)
        rtn.append(self.__Generate("Skeleton"))
        return rtn

    # Generate Based on Generation formate
    def GenerateProxy(self):
        rtn = []
        rtn.append(self.ServiceName)
        rtn.append(self.__Generate("Proxy"))
        return rtn

    def __Generate(self, Type):
        self.TabsNum = 0
        # GUARD #
        fname = ""
        for ns in self.NameSpaces:
            fname += f"_{ns.upper()}"

        fname += f"_{self.ServiceName.upper()}{Type.upper()}_HPP_"

        Temp = f"#ifndef {fname}\n"
        Temp += f"#define {fname}\n"

        # INCLUDES #
        Temp += f'#include "{Type}.hpp"\n#include "Event{Type}.hpp"\n#include "Method{Type}.hpp"\n#include "Field{Type}.hpp"\n'
 
        # NAMESPACES #
        for ns in self.NameSpaces:
            Temp += "\t" * self.TabsNum + "namespace " + ns + "\n" + "\t" * self.TabsNum + "{\n"
            self.TabsNum += 1

        # EVENTS NAMESPACE #
        if self.Events:
            Temp += "\t" * self.TabsNum + "namespace events\n" + "\t" * self.TabsNum + "{\n"
            self.TabsNum += 1
            if Type == "Proxy":
                for e in self.Events:
                    Temp += e.GenerateProxy(self.TabsNum)
            elif Type == "Skeleton":
                for e in self.Events:
                    Temp += e.GenerateSkeleton(self.TabsNum)

            self.TabsNum -= 1
            Temp += "\t" * self.TabsNum + "}\n"
        
        # FIELDS NAMESPACE #
        if self.Fields:
            Temp += "\t" * self.TabsNum + "namespace fields\n" + "\t" * self.TabsNum + "{\n"
            self.TabsNum += 1
            if Type == "Proxy":
                for f in self.Fields:
                    Temp += f.GenerateProxy(self.TabsNum)
            elif Type == "Skeleton":
                for f in self.Fields:
                    Temp += f.GenerateSkeleton(self.TabsNum)
            self.TabsNum -= 1
            Temp += "\t" * self.TabsNum + "}\n"
        
        # METHODS NAMESPACE #
        if self.Methods:
            Temp += "\t" * self.TabsNum + "namespace methods\n" + "\t" * self.TabsNum + "{\n"
            self.TabsNum += 1
            if Type == "Proxy":
                for m in self.Methods:
                    Temp += m.GenerateProxy(self.TabsNum)
            elif Type == "Skeleton":
                for m in self.Methods:
                    Temp += m.GenerateSkeleton(self.TabsNum)
            self.TabsNum -= 1
            Temp += "\t" * self.TabsNum + "}\n"


        # PROXY CLASS #
        Temp += "\t" * self.TabsNum + "class " + self.ServiceName + f"{Type} : public ara::com::internal::{Type}\n" + "\t" * self.TabsNum + "{\n"
        self.TabsNum += 1
        # PRIVATE #
        Temp += "\t" * self.TabsNum + "private:\n"
        self.TabsNum += 1


        self.TabsNum -= 1
        # PUBLIC #
        Temp += "\t" * self.TabsNum + "public:\n"
        self.TabsNum += 1

        # CONSTRUCTOR #
        Temp +=  "\t" * self.TabsNum + self.ServiceName + f"{Type}(HandleType &handle) : {Type} (handle)\n" + "\t" * self.TabsNum + "{\n"
        self.TabsNum += 1

        # EVENTS DEFINITION #
        if self.Events:
            for e in self.Events:
                Temp += "\t" * self.TabsNum + f"events::{e.EventName} {e.EventName}(handle);\n"
        
        # FIELDS DEFINITION #
        if self.Fields:
            for f in self.Fields:
                Temp += "\t" * self.TabsNum + f"fields::{f.FieldName} {f.FieldName}(handle);\n"
        
        # METHODS DEFINITION #
        if self.Methods:
            for m in self.Methods:
                Temp += "\t" * self.TabsNum + f"methods::{m.MethodName} {m.MethodName}(handle);\n"
        
        self.TabsNum -= 1
        Temp += "\t" * self.TabsNum + "}\n"

        self.TabsNum -= 2
        Temp += "\t" * self.TabsNum + "};\n"

        for ns in self.NameSpaces:
            self.TabsNum -= 1
            Temp += "\t" * self.TabsNum + "}\n"
        

        Temp += "#endif\n"
        return Temp        



class ComStructure:
    def __init__(self):
        self.CurrentXMLString = ""
        self.Name = ""
        self.ServiceInterfaces = {}
        self.DataTypes = {}
        self.Deployments = {}
        self.SoftwareCompounents = {}
        self.Mapping = {}
        self.Executables = {}
        self.Processes = {}
        self.Machines = {}
    
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

        elif fileType == "executables":
            if self.__CheckPName() == 1:
                return 1
            else:
                return self.__ScanE()

        elif fileType == "deployment":
            if self.__CheckPName() == 1:
                return 1
            else:
                return self.__ScanD()

        elif fileType == "processes":
            if self.__CheckPName() == 1:
                return 1
            else:
                return self.__ScanP()

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
        return 0

    def __ScanSI(self):
        print("Scanning Service Interfaces")


    def __ScanSC(self):
        print("Scanning Software Compounents")


    def __ScanE(self):
        print("Scanning Executables")


    def __ScanD(self):
        print("Scanning Deployments")


    def __ScanP(self):
        print("Scanning Processes")


    def __ScanMP(self):
        print("Scanning Mappings")


    def __CheckPName(self):
        root = ET.fromstring(self.xmlstring)
        # CHECK PROJECT NAME
        Tempnode = root.find("AR-PACKAGES")
        Tempnode = Tempnode.find("AR-PACKAGE")
        if self.Name != Tempnode.find("SHORT-NAME").text:
            return 1
        else:
            return 0

    def GenerateSkeleton(self):
        Skeleton = []
        for si in self.ServiceInterfaces:
            Skeleton.append(si.GenerateSkeleton())
        return Skeleton
        


    def GenerateProxy(self):
        Proxy = []
        for si in self.ServiceInterfaces:
            Proxy.append(si.GenerateProxy())
        return Proxy


