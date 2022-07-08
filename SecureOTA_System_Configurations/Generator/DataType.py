import xml.etree.ElementTree as ET


class DataTypeParser:
    def __init__(self, xmlString):
        self.xmlString = xmlString
        print("DataType Parser")

    def Parse(self):
        DataType = {}
        return DataType


########################################################################

# ToCPP = {
#     "boolean": "bool",
#     "uint8" : "uint8_t",
#     "uint16" : "uint16_t",
#     "uint32" : "uint32_t",
#     "uint64" : "uint64_t",
#     "sint8" : "sint8_t",
#     "sint16" : "sint16_t",
#     "sint32" : "sint32_t",
#     "sint64" : "sint64_t",
#     # (Not Sure if it's defined in Cpp or not)
#     "float32" : "float32_t",
#     "float64" : "float64_t"
# }

# class DataType:
#     def __init__(self, AllTypes,root):
#         self.Cat = root.find("CATEGORY").text
#         self.Name = root.find("SHORT-NAME").text
#         if self.Cat == "FIXED_LENGTH":
#             self.Implementation = None
#             self.Decleration = ToCPP[self.Name]

#         elif self.Cat == "TYPE_REFERENCE":
#             # Should I Use Ordinary Pointers or Shared Smart Pointers
#             self.Implementation = None
#             RefTypePath = root.find("SW-DATA-DEF-PROPS").find("SW-DATA-DEF-PROPS-VARIANTS").find("SW-DATA-DEF-PROPS-CONDITIONAL")
#             if (RefTypePath.find("BASE-TYPE-REF")) is not None:
#                 Type = RefTypePath.find("BASE-TYPE-REF").text.split("/")[-1]
#                 self.Decleration = f'{AllTypes.Types["BaseTypes"][Type].Decleration} *'
#             elif (RefTypePath.find("IMPLEMENTATION-DATA-TYPE-REF")) is not None:
#                 Type = RefTypePath.find("IMPLEMENTATION-DATA-TYPE-REF").text.split("/")[-1]
#                 self.Decleration = f'{AllTypes.Types["ImplementationDataTypes"][Type].Decleration} *'

#         elif self.Cat == "VALUE":
#             self.Implementation = None
#             RefTypePath = root.find("SW-DATA-DEF-PROPS").find("SW-DATA-DEF-PROPS-VARIANTS").find("SW-DATA-DEF-PROPS-CONDITIONAL")
#             if (RefTypePath.find("BASE-TYPE-REF")) is not None:
#                 Type = RefTypePath.find("BASE-TYPE-REF").text.split("/")[-1]
#                 self.Decleration = AllTypes.Types["BaseTypes"][Type].Decleration
#             elif (RefTypePath.find("IMPLEMENTATION-DATA-TYPE-REF")) is not None:
#                 Type = RefTypePath.find("IMPLEMENTATION-DATA-TYPE-REF").text.split("/")[-1]
#                 self.Decleration = AllTypes.Types["ImplementationDataTypes"][Type].Decleration
        
#         elif self.Cat == "VECTOR":
#             self.Implementation = None
#             self.Decleration = f'std::vector <{(DataType(AllTypes, root.find("SUB-ELEMENTS").find("IMPLEMENTATION-DATA-TYPE-ELEMENT"))).Decleration}>'
        
#         elif self.Cat == "STRUCTURE":
#             self.Decleration = self.Name
#             elements = []
#             ElementsPath = root.find("SUB-ELEMENTS")
#             for elem in ElementsPath.findall("IMPLEMENTATION-DATA-TYPE-ELEMENT"):
#                 elements.append(DataType(AllTypes, elem))

#             StringElements = ""
#             for selem in elements:
#                 StringElements = StringElements + f'\t{selem.Decleration} {selem.Name};\n'
#             self.Implementation = 'struct \n{' + f'{self.Name} \n{StringElements}' + '};'



# class DataTypes:    
#     def __init__(self, root):
#         self.Types = {}
#         self.Types["BaseTypes"] = {}
#         self.Types["ImplementationDataTypes"] = {}
#         for roottypenode in root.iter("AR-PACKAGE"):
#             if  roottypenode.find("SHORT-NAME").text == "BaseTypes":
#                 BaseElements = roottypenode.find("ELEMENTS")
#                 for BaseType in BaseElements.findall("SW-BASE-TYPE"):
#                     self.Types["BaseTypes"][BaseType.find("SHORT-NAME").text] = DataType(self, BaseType)

#             elif  roottypenode.find("SHORT-NAME").text == "ImplementationDataTypes":
#                 ImpElements = roottypenode.find("ELEMENTS")
#                 for ImpType in ImpElements.findall("IMPLEMENTATION-DATA-TYPE"):
#                     self.Types["ImplementationDataTypes"][ImpType.find("SHORT-NAME").text] = DataType(self, ImpType)
#     def Resolve (self, String):
#         MajorType = String.split("/")[-2]
#         MinorType = String.split("/")[-1]
#         return self.Types[MajorType][MinorType]