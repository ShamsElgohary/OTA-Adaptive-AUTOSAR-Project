from doctest import master
from pickle import NONE
from xml.dom.minidom import Element
import xml.etree.ElementTree as ET
pre = ""
machine_name=""
class argument:
    def __init__(self, name, path ,id,direction):
        self.name = name
        self.path=path
        self.id=id
        self.direction=direction
        print("argument initilalized")



class Method:
    def __init__(self,name,id):
        self.name = name
        self.id=id
        self.args=[]
        print("method initialized")
        
    def add_argument(self,arg):
        self.args.append(arg)
        print("arg added")


class Field:
    def __init__(self,name,id,path,getter,notifier,setter):
        self.path=path
        self.name=name
        self.id=id
        self.getter=getter
        self.notifier=notifier
        self.setter=setter



class ServiceInf:
    def __init__(self,name,id):
        self.methods=[]
        self.namespace=[]
        self.ServiceInf_name=name
        self.ServiceInf_id=id
        self.field=[]
    
    def add_method(self,method):
        self.methods.append(method)
        print("method added")
    
    def add_field(self,field):
        self.field.append(field)
        print("field added")
    
    def add_namespace(self,namespace):
        self.namespace.append(namespace)
        print("namespace added")
    
    def appear():
        print("iam heereeee")
    
    # def add_namespace(self,node):
    #     nss=node.getchildren()
    #     for i in nss:
    #         nspace=i.find(pre+"SHORT-NAME")
    #         s.add_namespace(nspace)

class ServiceInfParser:
    def __init__(self,xml_str):
        self.service_interface={}
        self.tree = ET.fromstring(xml_str)

    def add_Service(self,name,service):
        self.service_interface[name]=service
        print("Service Inf added to map successfulllyyyyy")
    
    def Parse(self):
        root=self.tree
        prelist = root.tag.split("}")
        pre = prelist[0] + "}"
        Tempnode = root.find(pre + "AR-PACKAGES")
        Tempnode = Tempnode.find(pre + "AR-PACKAGE")
        machinnode = Tempnode.find(pre + "SHORT-NAME")
        machine_name=machinnode.text
        Tempnode = Tempnode.find(pre + "AR-PACKAGES")
        d=Tempnode.getchildren()
        for x in d:
            # print(x.tag)
            # print(x.attrib)
            if x.find(pre + "SHORT-NAME").text=="service_interfaces":
                interface_p=x
        print(interface_p.tag) 
        Elements=interface_p.find(pre+"ELEMENTS")
        # print(Elements.tag)
        nodes=Elements.getchildren()
        for SI in nodes:
            id=SI.attrib
            name=SI.find(pre+"SHORT-NAME").text
            s=ServiceInf(name,id)
            print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
            print(s.ServiceInf_name,s.ServiceInf_id)
            nspaces_=SI.find(pre+"NAMESPACES")
            nss=nspaces_.getchildren()
            for i in nss:
                nspace=i.find(pre+"SHORT-NAME")
                s.add_namespace(nspace.text)
            # s.add_namespace(nspaces_)
            f=SI.find(pre+"FIELDS")
            if(f!=None):
                f=f.getchildren()
                for field in f:
                    id=field.attrib
                    name=field.find(pre+"SHORT-NAME").text
                    path=field.find(pre+"TYPE-TREF").text
                    getter=field.find(pre+"HAS-GETTER").text
                    setter=field.find(pre+"HAS-SETTER").text
                    notifier=field.find(pre+"HAS-NOTIFIER").text
                    new_field=Field(name,id,path,getter,notifier,setter)
                    s.add_field(new_field)
            m=SI.find(pre+"METHODS").getchildren()
            for i in m:
                id=i.attrib
                name=i.find(pre+"SHORT-NAME").text
                m_new=Method(name,id)
                print(m_new.name,m_new.id)
                args=i.find(pre+"ARGUMENTS")
                if args!=None:
                    sss=args.getchildren()
                    for x in sss:
                        # print(x.attrib)
                        arg_id=x.attrib
                        arg_name=x.find(pre+"SHORT-NAME").text
                        arg_path=x.find(pre+"TYPE-TREF").text
                        arg_dir=x.find(pre+"DIRECTION").text
                        a=argument(arg_name,arg_path,arg_id,arg_dir)
                        m_new.add_argument(a)
                s.add_method(m_new)
                # print("#####################")
            self.add_Service(s.ServiceInf_name,s)
        print("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")


# SI_parser=ServiceInfParser("service_interfaces.arxml")
# SI_parser.Parse()
# p=SI_parser.service_interface["UpdateRequest"]
# n=p.ServiceInf_name
# print (n)
# for namespace__ in p.namespace:
#     print("namespace : ", namespace__)  
# for i in p.methods:
#     print("method name: ",i.name)
#     for j in i.args:
#         print("argument name : ",j.name)
#         print("argument direction : ",j.direction)
#     print("&&&&&&&&&&&&&&&&&&&&&&&&&&")    
    
# for f in p.field:
#     print("notifier name:",f.name)       
#     print("notifier id:",f.id)        
#     print("notifier setter:",f.setter)        
#     print("notifier getter:",f.getter)        
#     print("notifier notifier:",f.notifier) 
# print("IIIIIIIIIIIIIIIIIIIIIIIIIIIIII")
# p=SI_parser.service_interface["PackageManager"]
# n=p.ServiceInf_name
# print (n)
# for namespace__ in p.namespace:
#     print("namespace : ", namespace__)   
# for i in p.methods:
#     print("method name: ",i.name)
#     for j in i.args:
#         print("argument name : ",j.name)
#         print("argument direction : ",j.direction)
 
#     print("&&&&&&&&&&&&&&&&&&&&&&&&&&")    
    
# for f in p.field:
#     print("notifier name:",f.name)       
#     print("notifier id:",f.id)        
#     print("notifier setter:",f.setter)        
#     print("notifier getter:",f.getter)        
#     print("notifier notifier:",f.notifier)        
#     print("notifier path:",f.path)        

