import xml.etree.ElementTree as ET


class ServiceInfParser:
    def __init__(self, xmlString):
        self.xmlString = xmlString
        print("Service Inf Parser")
    
    def Parse(self):
        ServiceInf = {}
        return ServiceInf











#####################################################################

class Event:
    def __init__(self, root, AllTypes):
        self.EventName = root.find("SHORT-NAME").text
        self.DatatypePath = root.find("TYPE-TREF").text
        self.Datatype = AllTypes.Resolve(self.DatatypePath)
        self.ID = 1

    # Generate Based on Generation formate
    def GenerateSkeleton(self, TabsNum):
        Skeleton = "Generate Event Skeleton"
        return Skeleton

    # Generate Based on Generation formate
    def GenerateProxy(self, TabsNum):
        Proxy = "Generate Event Proxy"
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
        Skeleton = "Generate Field Skeleton"
        return Skeleton

    # Generate Based on Generation formate
    def GenerateProxy(self, TabsNum):
        Proxy = "Generate Field Proxy"
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
        Proxy = "Generate Method Proxy"
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