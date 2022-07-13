from pickle import NONE
from Skel_Prxy_common import Datatypeslisting, unique, new_line, arg_struct, field_struct


def method_genrator(fd, method_name, method_id, service_id, in_args, out_args):
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
        fd.write("Output")
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
    if  len(out_args)!=0 and len(in_args)!=0:
        fd.write("                            process_method_call<")
        fd.write(method_output)
        fd.write(" , ")
        fd.write(method_input)
        fd.write(">")
        fd.write(" (")    
        fd.write("in , ")
        fd.write("out")
        fd.write(")")  
    elif len(in_args) == 0 and len(out_args) != 0:
        fd.write("                            process_method_call<")
        fd.write(method_output)
        fd.write(">")
        fd.write(" (")    
        fd.write("out")
        fd.write(")")  
    else:
        fd.write("                            process_method_call();")
    new_line(fd)
    fd.write("                            ara::com::AddMethodCall(")
    fd.write(method_id)
    fd.write(" , ")
    fd.write(method_name)
    fd.write(" , ara::com::MethodType::Proxy_Method, ")
    fd.write(service_id)
    fd.write(" , Cluster_Name);")
    new_line(fd)
    if len(out_args)!=0:
        fd.write("                            return out;")
    new_line(fd)
    fd.write("                        }")
    new_line(fd)
    fd.write("                    };")
    new_line(fd)    


def class_genrator(fd,service, service_id):
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
        

def field_helper(fd,service,decider,name,id, service_id):
    f_name=decider+name
    output_type=name+"Field"
    fd.write("                    class ")
    fd.write(f_name)
    fd.write(" : public ara::com::proxy::method::MethodBase")
    new_line(fd)
    fd.write("                    {")
    new_line(fd)
    fd.write("                    public:")
    new_line(fd)
    fd.write(f"                        {f_name}(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, ")
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
    fd.write(">(out);")
    new_line(fd)
    fd.write("                            ara::com::AddMethodCall(")
    fd.write(id)
    fd.write(", ")
    fd.write('"')
    fd.write(f_name)
    fd.write('", ')
    fd.write("ara::com::MethodType::Proxy_Method,")
    fd.write(service_id)
    fd.write(", Cluster_Name);")
    new_line(fd)
    fd.write("                            return out;")
    new_line(fd)
    fd.write("                        }")
    new_line(fd)
    fd.write("                    };")
    new_line(fd)
        

def field_genrator(fd,service, fields, serv_id):
    fd.write("                namespace fields")
    new_line(fd)
    fd.write("                {")
    new_line(fd)
    for f in service.field:
        if f.getter == "true":
            id = fields[f.name][0]
            decider="Get"
            field_helper(fd,service,decider,f.name,id, serv_id)

        if f.setter == "true":
            id = fields[f.name][1]
            decider="Set"
            field_helper(fd,service,decider,f.name,id, serv_id)
           
        if f.notifier == "true":
            id = fields[f.name][2]
            decider="Notify"
            field_helper(fd,service,decider,f.name,id, serv_id)
        
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
    fd.write("#include <array>")
    new_line(fd)
    fd.write('#include "serviceProxy.hpp"')
    new_line(fd)
    fd.write('#include "method.hpp"')
    new_line(fd)
    fd.write('#include "types.hpp"')
    new_line(fd)
    fd.write('using namespace std;')
    new_line(fd)

def proxy_generator(SI_parser, DataTypes, Deployment):
    for i in SI_parser.service_interface.keys():
        # creating proxy file 
        serv_id = Deployment[i][0]

        filename=i+"Proxy.hpp"
        f = open(filename, "w")
        includes(f)
        # accessing the Service interface 
        SI=SI_parser.service_interface[i]
        namespaces_genration(f,SI)

        # Shared Data Types Definition
        shared_list = []
        shared_str = ""

        for j in SI.shared_types:
            Datatypeslisting(j, shared_list, DataTypes)
            shared_list.append(j)

        shared_list = unique(shared_list)
        for j in shared_list:
            shared_str += DataTypes["shared_types"][j].Definition()
        f.write(shared_str)
        new_line(f)

        mm = SI.methods
        flds = SI.field
        #generate the Struct of the Methods Arguments
        for j in mm:
            arg_struct(f, j, DataTypes)

        for j in flds:
            field_struct(f, j, DataTypes)

        # Genrating the methods of the service interface    
        method_namespace(f)
        for j in mm:
            meth_id = None
            meth_id = Deployment[i][1][j.name]
            method_genrator(f, j.name, meth_id, serv_id, j.in_args, j.out_args)

        f.write("                }")
        new_line(f)
        # genrating the fields of the service interface    
        if len(SI.field)>0:
            fields = Deployment[i][2]
            field_genrator(f,SI,fields, serv_id)
        class_genrator(f, SI, serv_id)
        closing_namespaces(f,SI)
    f.close()
