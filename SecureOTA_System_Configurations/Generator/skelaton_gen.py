from asyncore import write
from curses.ascii import SI
from doctest import master
from pickle import NONE
from xml.dom.minidom import Element
import xml.etree.ElementTree as ET
from xxlimited import new
from service_interface import ServiceInfParser



def new_line(fd):
    fd.write("\n")


def virtual_method_genrator(fd,service):
    for method in service.methods:
        method_output=method.name+"Output"
        length=len(method.in_args)
        counter=0
        fd.write("                    virtual std::future<")
        fd.write(method_output)
        fd.write("> ")
        fd.write(method.name)
        fd.write(" (")
        for arg in method.in_args:
            counter+=1
            fd.write(arg.type)
            fd.write(" ")
            fd.write(arg.name)
            if counter<length:
                fd.write(" , ")
        fd.write(")")     
        fd.write(" = 0;")   
        new_line(fd)
    for field in service.field:       
            if field.getter == "true":
                field_decider="Get"+field.name+"Field"
                name_decider= "Get"+field.name    
            if field.setter == "true":
                field_decider="Set"+field.name+"Field"
                name_decider= "Set"+field.name        
            if field.notifier == "true":
                field_decider="notifier"+field.name+"Field"
                name_decider= "notifier"+field.name    
            length=len(service.field)
            counter=0
            fd.write("                    virtual std::future<")
            fd.write(field_decider)
            fd.write("> ")
            fd.write(name_decider)
            fd.write(" (")
            fd.write(")")     
            fd.write(" = 0;")   
            new_line(fd)        




def handle_method(fd,service):
    new_line(fd)
    fd.write("                    void handleMethod() override")
    new_line(fd)
    fd.write("                    {")
    new_line(fd)
    new_line(fd)
    fd.write("                        int methodID;")
    new_line(fd)
    fd.write("                        string methodName;")
    new_line(fd)
    fd.write("                        stringstream payload = this->ptr2bindingProtocol->ReceiveMessage(methodID);")
    new_line(fd)
    fd.write("                    switch (methodID)")
    new_line(fd)
    fd.write("                    {")
    method_counter=1
    for method in service.methods:
        new_line(fd)
        length=len(method.in_args)
        counter=0
        method_output=method.name+"Output"
        method_input=method.name+"Input"
        input_var_ip="ip"
        output_var_op="F_op"
        # out_var="out"+str(counter)
        new_line(fd)
        fd.write("                        case ")
        fd.write(str(method_counter))
        method_counter+=1
        new_line(fd)
        fd.write("                        {")
        new_line(fd)
        #method name
        fd.write('                        methodName = "')
        fd.write(method.name)
        fd.write('" ;')
        new_line(fd)
        # deserialize input
        if len(method.in_args) !=0:
            fd.write("                        ")
            fd.write(method_input)
            fd.write(" ")
            fd.write(input_var_ip)
            fd.write(" ;")
            new_line(fd)
            fd.write("                        Deserializer2 D;")
            new_line(fd)
            fd.write("                        D.deserialize(payload, ")
            fd.write(input_var_ip)
            fd.write(");")

            # fd.write(method_input)
            # fd.write(");")
            new_line(fd)
        fd.write("                        std::future<")    
        fd.write(method_output)
        fd.write("> ")
        fd.write(output_var_op)
        fd.write(" = ")

        if len(method.in_args) ==0:
            fd.write(method.name)
            fd.write("()")
            new_line(fd)
        else:
            fd.write(method.name)
            fd.write("(")
            for arg in method.in_args:
                counter+=1
                fd.write("ip.")
                fd.write(arg.name)
                if counter<length:
                  fd.write(" , ")
            fd.write(") ;")
            new_line(fd)
            new_line(fd)
        new_line(fd)
        fd.write("                        Serializer2 S;")
        new_line(fd)
        fd.write("                        stringstream result;")
        new_line(fd)
        fd.write("                        ")
        fd.write(method_output)
        fd.write(" op = ")
        fd.write(output_var_op)
        fd.write(".get();")
        new_line(fd)
        fd.write("                        S.serialize(result, op);")
        new_line(fd)
        fd.write("                        this->ptr2bindingProtocol->SendResponse(")
        fd.write(str(method_counter))
        fd.write(", result);")
        new_line(fd)
        fd.write("                        break;")
        new_line(fd)
        fd.write("                        }")
        new_line(fd)
        
    for field in service.field:
        new_line(fd)
        name=""
        field_name=""
        if field.getter=="true":
            name="Get"+field.name
            field_name=name+"Field"
        elif field.setter=="true":
            name="Set"+field.name
            field_name=name+"Field"
        elif field.notifier=="true":
            name="notify"+field.name
            field_name=name+"Field"
        output_var_op="F_op"
        # out_var="out"+str(counter)
        fd.write("                        case ")
        fd.write(str(method_counter))
        method_counter+=1
        new_line(fd)
        fd.write("                        {")
        new_line(fd)
        #method name
        fd.write('                        methodName = "')
        fd.write(name)
        fd.write('" ;')
        new_line(fd)
        fd.write("                        std::future<")    
        fd.write(field_name)
        fd.write("> ")
        fd.write(output_var_op)
        fd.write(" = ")
        fd.write(name)
        fd.write("()")
        new_line(fd)
        fd.write("                        Serializer2 S;")
        new_line(fd)
        fd.write("                        stringstream result;")
        new_line(fd)
        fd.write("                        ")
        fd.write(method_output)
        fd.write(" op = ")
        fd.write(output_var_op)
        fd.write(".get();")
        new_line(fd)
        fd.write("                        S.serialize(result, op);")
        new_line(fd)
        fd.write("                        this->ptr2bindingProtocol->SendResponse(")
        fd.write(str(method_counter))
        fd.write(", result);")
        new_line(fd)
        fd.write("                        break;")
        new_line(fd)
        fd.write("                        }")
        new_line(fd)
    fd.write("                    }")



def class_genrator(fd,service):
    if service.ServiceInf_name=="UpdateRequest":
        service_id="1"
    elif service.ServiceInf_name=="PackageManager":    
        service_id="2"
    serice_sklaton_name=service.ServiceInf_name+"Skeleton"
    fd.write("                class ")
    fd.write(serice_sklaton_name)
    fd.write(" : public ara::com::skeleton::skeletonBase")
    new_line(fd)
    fd.write("                {")
    new_line(fd)
    fd.write("                public:")
    new_line(fd)
    fd.write("                    ")
    fd.write(serice_sklaton_name)
    fd.write('(ara::com::InstanceIdentifier I_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : skeletonBase(CUSTOMIZED_PROJECT_PATH + "executables/ucm/0.1/etc/service_manifest.json", ')
    fd.write(service_id)
    fd.write(", I_id, Cluster_Name, mode)")
    new_line(fd)
    fd.write("                    {")
    fd.write("}")
    new_line(fd)
    virtual_method_genrator(fd,service)
    handle_method(fd,service)
    new_line(fd)
    fd.write("                        ara::com::AddMethodCall(methodID, methodName, ara::com::MethodType::Skeleton_Method, ")
    fd.write(service_id)
    fd.write(", Cluster_Name);")
    new_line(fd)
    fd.write("                }")
    new_line(fd)
    fd.write("            };")
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
    new_line(fd)


def closing_namespaces(fd,service):
    for n in range (len(service.namespace)-1,-1,-1):
        space=n*"    "
        strr=space+"}"
        fd.write(strr)
        new_line(fd)


def includes(fd):
    fd.write("#pragma once")
    new_line(fd)
    fd.write('#include "types.hpp"')
    new_line(fd)
    fd.write('#include "serviceskeleton.hpp"')
    new_line(fd)
    fd.write('#include "serialization_simple.hpp"')
    new_line(fd)
    fd.write('using namespace std;')
    new_line(fd)


def skelaton_genrator():
    SI_parser=ServiceInfParser("service_interfaces.arxml")
    SI_parser.Parse()
    for i in SI_parser.service_interface.keys():
        # creating proxy file 
        filename=i+"Selaton.hpp"
        f = open(filename, "w")
        includes(f)
        # accessing the Service interface 
        SI=SI_parser.service_interface[i]
        namespaces_genration(f,SI)
        class_genrator(f,SI)
        closing_namespaces(f,SI)
    f.close()


skelaton_genrator()

