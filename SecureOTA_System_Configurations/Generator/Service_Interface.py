from asyncore import write
from doctest import master
from pickle import NONE
from xml.dom.minidom import Element
import xml.etree.ElementTree as ET
from xxlimited import new
pre = ""
machine_name = ""

class argument:
    def __init__(self, name, path ,id,direction):
        self.name = name
        self.path=path
        self.id=id
        self.direction=direction
        type_location=path.find("/",5)+1
        self.type=path[type_location:]  
 


class Method:
    def __init__(self,name,id):
        self.name = name
        self.id=id
        self.in_args=[]
        self.out_args=[]
        # print("method initialized")
        
    def add_argument(self,arg):
        if arg.direction=="IN":
            self.in_args.append(arg)
        elif arg.direction=="OUT":
            self.out_args.append(arg)   
    


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
        self.shared_types = []
    
    def add_method(self,method):
        self.methods.append(method)
        # print("method added")
    
    def add_field(self,field):
        self.field.append(field)
        # print("field added")
    
    def add_namespace(self,namespace):
        self.namespace.append(namespace)
    

class ServiceInfParser:
    def __init__(self,xmlString):
        self.xmlString = xmlString
        self.service_interface={}

    def add_Service(self,name,service):
        self.service_interface[name]=service
        # print("Service Inf added to map successfulllyyyyy")
    
    def Parse(self):
        global pre
        root = ET.fromstring(self.xmlString)
        prelist = root.tag.split("}")
        pre = prelist[0] + "}"

        Tempnode = root.find(pre + "AR-PACKAGES")
        Tempnode = Tempnode.find(pre + "AR-PACKAGE")
        machinnode = Tempnode.find(pre + "SHORT-NAME")
        machine_name = machinnode.text
        Tempnode = Tempnode.find(pre + "AR-PACKAGES")
        d=Tempnode.getchildren()
        for x in d:
            # print(x.tag)
            # print(x.attrib)
            if x.find(pre + "SHORT-NAME").text=="service_interfaces":
                interface_p=x
        # print(interface_p.tag) 
        Elements=interface_p.find(pre+"ELEMENTS")
        # print(Elements.tag)
        nodes=Elements.getchildren()
        for SI in nodes:
            id=SI.attrib
            name=SI.find(pre+"SHORT-NAME").text
            s=ServiceInf(name,id)
            # print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
            # print(s.ServiceInf_name,s.ServiceInf_id)
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

                    pathlist = path.split("/")
                    if (pathlist[-2] == "shared_types"):
                        s.shared_types.append(pathlist[-1])

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
                # print(m_new.name,m_new.id)
                args=i.find(pre+"ARGUMENTS")
                if args!=None:
                    sss=args.getchildren()
                    for x in sss:
                        # print(x.attrib)
                        arg_id=x.attrib
                        arg_name=x.find(pre+"SHORT-NAME").text
                        arg_path=x.find(pre+"TYPE-TREF").text

                        pathlist = arg_path.split("/")
                        if (pathlist[-2] == "shared_types"):
                            s.shared_types.append(pathlist[-1])

                        arg_dir=x.find(pre+"DIRECTION").text
                        a=argument(arg_name,arg_path,arg_id,arg_dir)
                        m_new.add_argument(a)
                s.add_method(m_new)
                # print("#####################")
            self.add_Service(s.ServiceInf_name,s)

def new_line(fd):
    fd.write("\n")

def Proxy_method_genrator(fd,method_name,method_id,in_args,out_args):
    method_input=method_name+"Input"
    method_output=method_name+"Output"
    fd.write("                    class ")
    fd.write(method_name)
    fd.write(" : public ara::com::proxy::method::MethodBase")
    new_line(fd)
    fd.write("                    {")
    new_line(fd)
    fd.write("                    public:")
    new_line(fd)
    fd.write("                        ")
    fd.write(method_name)
    fd.write("(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, ")
    fd.write(method_id)
    fd.write(") {}")
    new_line(fd)
    if(len(out_args)!=0):
        fd.write("                        ")
        fd.write(method_name)
        fd.write("output")
    else:
        fd.write("                        void ")
    fd.write(" operator()(")
    length=len(in_args)
    counter=0
    for i in in_args:
        counter+=1
        fd.write(i.type)
        fd.write(" ")
        fd.write(i.name)
        if counter<length:
            fd.write(" , ")
    fd.write(")")
    new_line(fd)
    fd.write("                            {")
    new_line(fd)
    if len(in_args)!=0:
        fd.write("                            ")
        fd.write(method_input)
        fd.write(" in;")
        new_line(fd)
        for i in in_args:
            fd.write("                            in.")
            fd.write(i.name)
            fd.write(" = ")
            fd.write(i.name)
            fd.write(" ; ")
            new_line(fd)
    if len(out_args)!=0:
        fd.write("                            ")
        fd.write(method_output)
        fd.write(" out; ")
        new_line(fd)
    if  len(out_args)!=0 or  len(in_args)!=0:
        fd.write("                            process_method_call<")
        if len(out_args)!=0:
            fd.write(method_output)
        if len(in_args)!=0:
            fd.write(" , ")
            fd.write(method_input)
        fd.write(">")
        fd.write(" (")    
        if len(in_args)!=0:
            fd.write("in , ")
        if len(out_args)!=0:
            fd.write("out")
        fd.write(")")    
    else:
        fd.write("                            process_method_call();")
    new_line(fd)
    fd.write("                            ara::com::AddMethodCall(")
    fd.write("1 , ")
    fd.write(method_name)
    fd.write(" , ara::com::MethodType::Proxy_Method, ")
    fd.write("1")
    fd.write(" , Cluster_Name);")
    new_line(fd)
    if len(out_args)!=0:
        fd.write("                            return out;")
    new_line(fd)
    fd.write("                        }")
    new_line(fd)
    fd.write("                    };")
    new_line(fd)    
    