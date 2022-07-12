from asyncore import write
from doctest import master
from pickle import NONE
from xml.dom.minidom import Element
import xml.etree.ElementTree as ET
from xxlimited import new
from Service_Interface import ServiceInfParser


def new_line(fd):
    fd.write("\n")


def method_genrator(fd,method_name,method_id,service_id,in_args,out_args):
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
    fd.write("                        {")
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


def class_genrator(fd,service):
    if service.ServiceInf_name=="UpdateRequest":
        service_id="1"
    elif service.ServiceInf_name=="PackageManager":    
        service_id="2"
    service_proxy_name=service.ServiceInf_name+"proxy"
    fd.write("                class ")
    fd.write(service_proxy_name)
    fd.write(" : public ara::com::proxy::ProxyBase")
    new_line(fd)
    fd.write("                {")
    new_line(fd)
    fd.write("                public:")
    new_line(fd)
    fd.write("                    ")
    fd.write(service_proxy_name)
    fd.write("(HandleType handle) : ProxyBase(handle), ")
    counter=0
    length=len(service.methods)
    for i in service.methods:
        counter+=1
        fd.write(i.name)
        fd.write("(handle.ptr2bindingProtocol) ")
        if counter<length:
            fd.write(", ")
    new_line(fd)
    fd.write("                    {")
    new_line(fd)
    fd.write("                    }")
    new_line(fd)
    fd.write("                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService()")
    new_line(fd)
    fd.write("                    {")
    new_line(fd)
    fd.write('                        return ara::com::proxy::ProxyBase::FindService(CUSTOMIZED_PROJECT_PATH + "executables/ucm/0.1/etc/service_manifest.json", Cluster_Name, ')
    fd.write(service_id)
    fd.write(');')
    new_line(fd)
    fd.write("                    }")
    new_line(fd)
    fd.write("                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(ara::com::InstanceIdentifier InstanceID)")
    new_line(fd)
    fd.write("                    {")
    new_line(fd)
    fd.write('                        return ara::com::proxy::ProxyBase::FindService(CUSTOMIZED_PROJECT_PATH + "executables/ucm/0.1/etc/service_manifest.json", Cluster_Name, ')
    fd.write(service_id)
    fd.write(', InstanceID);')
    new_line(fd)
    fd.write("                    }")
    new_line(fd)
    for m in service.methods:
        fd.write("                    methods::")
        fd.write(m.name)
        fd.write(" ")
        fd.write(m.name)
        fd.write(";")
        new_line(fd)
    for f in service.field:
        if f.setter == "true":
            fd.write("                    fields::")
            f_name="Set"+f.name
            fd.write(f.name)
            fd.write(" ")
            fd.write(f_name)
            fd.write(";")
            new_line(fd)
        if f.getter == "true":
            fd.write("                    fields::")
            f_name="Get"+f.name
            fd.write(f_name)
            fd.write(" ")
            fd.write(f_name)
            fd.write(";")
            new_line(fd)    
        if f.notifier == "true":
            fd.write("                    fields::")
            f_name="notifier"+f.name
            fd.write(f_name)
            fd.write(" ")
            fd.write(f_name)
            fd.write(";")
            new_line(fd)
    fd.write("                };")
    new_line(fd)    
        

def field_helper(fd,service,decider,name,id):
            f_name=decider+name
            output_type=decider+name
            fd.write("                    class ")
            fd.write(f_name)
            fd.write(" : public ara::com::proxy::method::MethodBase")
            new_line(fd)
            fd.write("                    {")
            new_line(fd)
            fd.write("                    public:")
            new_line(fd)
            fd.write("                        GetCurrentStatus(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, ")
            fd.write(id)
            fd.write(") {}")
            new_line(fd)
            fd.write("                        ")
            fd.write(f_name)
            fd.write(" operator()()")
            new_line(fd)
            fd.write("                        {")
            new_line(fd)
            fd.write("                            ")
            fd.write(output_type)
            fd.write(" out ;")
            new_line(fd)
            fd.write("                            process_method_call<")
            fd.write(output_type)
            fd.write(")>(out);")
            new_line(fd)
            fd.write("                            ara::com::AddMethodCall(")
            fd.write(id)
            fd.write(", ")
            fd.write('"')
            fd.write(f_name)
            fd.write('", ')
            fd.write("ara::com::MethodType::Proxy_Method,")
            if service.ServiceInf_name=="UpdateRequest":
                service_id="1"
            elif service.ServiceInf_name=="PackageManager":    
                service_id="2"
            fd.write(service_id)
            fd.write(", Cluster_Name);")
            new_line(fd)
            fd.write("                            return out;")
            new_line(fd)
            fd.write("                        }")
            new_line(fd)
            fd.write("                    };")
            new_line(fd)
        

def field_genrator(fd,service,id):
    fd.write("                namespace fields")
    new_line(fd)
    fd.write("                {")
    new_line(fd)
    for f in service.field:
        if f.getter == "true":
            decider="Get"
            field_helper(fd,service,decider,f.name,id)

        if f.setter == "true":
            decider="Set"
            field_helper(fd,service,decider,f.name,id)
           
        if f.notifier == "true":
            decider="Notify"
            field_helper(fd,service,decider,f.name,id)
        
        new_line(fd)
    fd.write("                }")
    new_line(fd)


def method_namespace(fd):
    fd.write("                namespace methods ")
    new_line(fd)
    fd.write("                { ")
    new_line(fd)


def namespaces_genration(fd,service):
    space=""
    for name_space in service.namespace:
        n=space+"namespace "
        b=space+"{"
        fd.write(n)
        fd.write(name_space)
        new_line(fd)
        fd.write(b)
        new_line(fd)
        space=space+"    "
    fd.write("            namespace proxy")
    new_line(fd)
    fd.write("            {")
    new_line(fd)


def closing_namespaces(fd,service):
    for n in range (len(service.namespace),-1,-1):
        space=n*"    "
        strr=space+"}"
        fd.write(strr)
        new_line(fd)


def includes(fd):
    fd.write("#pragma once")
    new_line(fd)
    fd.write("#include <vector>")
    new_line(fd)
    fd.write('#include "serviceProxy.hpp"')
    new_line(fd)
    fd.write('#include "method.hpp"')
    new_line(fd)
    fd.write('#include "types.hpp"')
    new_line(fd)
    fd.write('using namespace std;')
    new_line(fd)


def proxy_genrator():
    SI_parser=ServiceInfParser("service_interfaces.arxml")
    SI_parser.Parse()
    for i in SI_parser.service_interface.keys():
        # creating proxy file 
        filename=i+"proxy.hpp"
        f = open(filename, "w")
        includes(f)
        # accessing the Service interface 
        SI=SI_parser.service_interface[i]
        namespaces_genration(f,SI)

        #genrating the methods of the service interface    
        method_namespace(f)
        mm=SI.methods
        for j in mm:
            method_genrator(f,j.name,"1",3,j.in_args,j.out_args)
        f.write("                }")
        new_line(f)
        #genrating the fields of the service interface    
        if len(SI.field)>0:
            field_genrator(f,SI,"10")
        class_genrator(f,SI)
        closing_namespaces(f,SI)
    f.close()


proxy_genrator()