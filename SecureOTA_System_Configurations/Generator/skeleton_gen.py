from Skel_Prxy_common import Datatypeslisting, unique, new_line, arg_struct, field_struct


def virtual_method_genrator(fd,service):
    for method in service.methods:
        method_output=method.name+"Output"
        length=len(method.in_args)
        counter=0
        if len(method.out_args) > 0:
            fd.write("                    virtual std::future<")
            fd.write(method_output)
            fd.write("> ")
        else:
            fd.write("                    virtual void ")
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
        field_decider=field.name+"Field"  
        if field.getter == "true":
            name_decider= "Get"+field.name 
               
        if field.setter == "true":
            name_decider= "Set"+field.name  

        if field.notifier == "true":
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




def handle_method(fd,service, Deployment):
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
    for method in service.methods:
        method_counter = Deployment[1][method.name]
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
        fd.write(f"{str(method_counter)}:")
        new_line(fd)
        fd.write("                        {")
        new_line(fd)
        fd.write('                        methodName = "')
        fd.write(method.name)
        fd.write('" ;')
        new_line(fd)
        # deserialize input
        if len(method.out_args) !=0:
            if len(method.in_args) != 0:
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
                new_line(fd)
                fd.write("                        std::future<")    
                fd.write(method_output)
                fd.write("> ")
                fd.write(output_var_op)
                fd.write(" = ")
                fd.write(method.name)
                fd.write("(")
                for arg in method.in_args:
                    counter+=1
                    fd.write("ip.")
                    fd.write(arg.name)
                    if counter<length:
                        fd.write(" , ")
                fd.write(") ;")
            elif len(method.in_args) == 0:
                fd.write("                        std::future<")    
                fd.write(method_output)
                fd.write("> ")
                fd.write(output_var_op)
                fd.write(" = ")
                fd.write(method.name)
                fd.write("();")

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


        elif len(method.out_args) == 0:
            fd.write(f"                        {method.name}();")

        new_line(fd)
        fd.write("                        break;")
        new_line(fd)
        fd.write("                        }")
        new_line(fd)
        
    for field in service.field:
        new_line(fd)
        name=""
        
        field_name=field.name+"Field"
        if field.getter=="true":
            method_counter = Deployment[2][field.name][0]
            name="Get"+field.name

        elif field.setter=="true":
            method_counter = Deployment[2][field.name][1]
            name="Set"+field.name

        elif field.notifier=="true":
            method_counter = Deployment[2][field.name][2]
            name="notify"+field.name

        output_var_op="F_op"
        # out_var="out"+str(counter)
        fd.write("                        case ")
        fd.write(f"{str(method_counter)}:")
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
        fd.write("();")
        new_line(fd)
        fd.write("                        Serializer2 S;")
        new_line(fd)
        fd.write("                        stringstream result;")
        new_line(fd)
        fd.write("                        ")
        fd.write(field_name)
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



def class_genrator(fd,service, service_id, Deployment, DataTypes, SWname):
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
    fd.write(f'(ara::com::InstanceIdentifier I_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : skeletonBase(CUSTOMIZED_PROJECT_PATH + "executables/{SWname}/0.1/etc/service_manifest.json", ')
    fd.write(service_id)
    fd.write(", I_id, Cluster_Name, mode)")
    new_line(fd)
    fd.write("                    {")
    fd.write("}")
    new_line(fd)
    mm = service.methods
    flds = service.field
    #generate the Struct of the Methods Arguments
    for j in mm:
        arg_struct(fd, j, DataTypes)
    for j in flds:
        field_struct(fd, j, DataTypes)
        
    virtual_method_genrator(fd,service)
    handle_method(fd,service, Deployment)
    new_line(fd)
    if SWname == "ota" or SWname == "ucm" or SWname == "sm":
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


def skeleton_generator(SI_parser, DataTypes, Deployment, Deployment_Manifest, Sw_Comps):
    for i in SI_parser.service_interface.keys():
        SkeletonNeeded = []
        for inst in Deployment_Manifest:
            if inst[0] == f"{i}_provided_instance":
                SkeletonNeeded.append(inst)
        if len (SkeletonNeeded) == 0:
            continue
        for Sw_Comp in Sw_Comps:
            SWname = Sw_Comp[0]
            for port in Sw_Comp[1]:
                if port[0] == 'P-PORT name' and port[2] == i:
                    # creating proxy file 
                    # creating proxy file 
                    serv_id = Deployment[i][0]

                    SI=SI_parser.service_interface[i]
                    
                    # creating proxy file 
                    filename="SecureOTA_System_Configurations/GeneratedFiles/"+i+"Skeleton.hpp"
                    f = open(filename, "w")
                    includes(f)
                    namespaces_genration(f,SI)

                    # accessing the Service interface 
                    InfDeployment = Deployment[i]
                    class_genrator(f,SI, serv_id, InfDeployment, DataTypes, SWname)
                    closing_namespaces(f,SI)
                    f.close()


