import xml.etree.ElementTree as ET

pre = ""
compu_methods = []

class DataTypeParser:
    def __init__(self, xmlString):
        self.xmlString = xmlString
        print("DataType Parser")
        self.DataTypes = {"std_types": [], "shared_types" : []}

    def Parse(self):
        global pre
        root = ET.fromstring(self.xmlString)
        prelist = root.tag.split("}")
        pre = prelist[0] + "}"

        # FIND PROJECT NAME
        Tempnode = root.find(pre + "AR-PACKAGES")
        Tempnode = Tempnode.find(pre + "AR-PACKAGE")
        Tempnode = Tempnode.find(pre + "AR-PACKAGES")
        Dif_DTypes =  Tempnode.getchildren()
        for Temp in Dif_DTypes:
            if Temp.find(pre + "SHORT-NAME").text == "std_types":
                self.STD_Parse(Temp)

            elif Temp.find(pre + "SHORT-NAME").text == "shared_types":
                self.Shared_Parse(Temp)

            elif Temp.find(pre + "SHORT-NAME").text == "compu_methods":
                self.Compu_Parse(Temp)
                        
            else:
                print ("Unkown DataType Found")
        
        for Temp in self.DataTypes["shared_types"]:
            print(Temp.Definition())

        return self.DataTypes

    def STD_Parse(self, root):
        global pre
        Temp = root.find(pre + "ELEMENTS")
        Elements = Temp.getchildren()
        for Element in Elements:
            Temp = DataType(Element)
            self.DataTypes["std_types"].append(Temp)
    
    def Shared_Parse(self, root):
        global pre
        Temp = root.find(pre + "ELEMENTS")
        Elements = Temp.getchildren()
        for Element in Elements:
            Temp = DataType(Element)
            self.DataTypes["shared_types"].append(Temp)

    def Compu_Parse(self, root):
        global pre
        global compu_methods
        Temp = root.find(pre + "ELEMENTS")
        Elements = Temp.getchildren()
        for Element in Elements:
            Temp = CompuType(Element)
            compu_methods.append(Temp)

class DataType:
    def __init__(self,root):
        global pre
        self.root = root
        self.Cat = root.find(pre + "CATEGORY").text
        self.S_Name = root.find(pre + "SHORT-NAME").text
        self.subType = None
        self.ArrSize = None
        self.subTypePath = None
        self.enumPath = None
        self.enum = None

    def Definition (self):
        Def = ""
        if (self.Cat == "VALUE"):
            Def = ""
        
        elif (self.Cat == "STRING"):
            Def = "using " + self.S_Name + " = std::string;\n"

        elif (self.Cat == "VECTOR" or self.Cat == "ARRAY" ):
            Temp = self.root.find(pre + "TEMPLATE-ARGUMENTS")
            Temp = Temp.find(pre + "CPP-TEMPLATE-ARGUMENT")
            self.subTypePath= Temp.find(pre + "TEMPLATE-TYPE-REF").text
            temp = self.subTypePath.rfind("/")
            self.subType = self.subTypePath[temp+1:]

            if (self.Cat == "ARRAY"):
                self.ArrSize = self.root.find(pre + "ARRAY-SIZE").text
                Def = "using " + self.S_Name + " = std::array<" + self.subType + ", " + self.ArrSize + ">;\n"

            if (self.Cat == "VECTOR"):
                Def = "using " + self.S_Name + " = std::vector<" + self.subType + ">;\n"

        elif (self.Cat == "TEXTTABLE"):
            global compu_methods
            Temp = self.root.find(pre + "SW-DATA-DEF-PROPS")
            Temp = Temp.find(pre + "SW-DATA-DEF-PROPS-VARIANTS")
            Temp = Temp.find(pre + "SW-DATA-DEF-PROPS-CONDITIONAL")
            self.enumPath= Temp.find(pre + "COMPU-METHOD-REF").text
            temp = self.enumPath.rfind("/")
            self.enum = self.enumPath[temp+1:]
            for compu_method in compu_methods:
                if (compu_method.S_Name == self.enum):
                    Def = f"enum class {self.S_Name} : uint8_t \n"
                    Def += "{\n"
                    for value in compu_method.Values:
                        Def += f"{value[0]} = {value[1]}U, \n"
                    Def += "};\n"
        elif (self.Cat == "STRUCT"):
            Def = "STRUCT"

        else:
            Def = "Unknown Type"

        return Def


    def Instantation (self, Name = "No_Name"):
        Def = self.S_Name + " " + Name + ";\n"

        return Def

class CompuType:
    def __init__(self,root):
        global pre
        self.S_Name = root.find(pre + "SHORT-NAME").text
        Temp = root.find(pre + "COMPU-INTERNAL-TO-PHYS")
        Temp = Temp.find(pre + "COMPU-SCALES")
        self.Values = []
        EnumValues = Temp.getchildren()
        for EnumValue in EnumValues:
            Temp = EnumValue.find(pre + "COMPU-CONST")
            V_name = Temp.find(pre + "VT").text
            V_num = EnumValue.find(pre + "UPPER-LIMIT").text
            V = [V_name , V_num]
            self.Values.append(V)

            