import xml.etree.ElementTree as ET


class DeploymentParser:
    def __init__(self, xmlString):
        self.xmlString = xmlString
        print("Deployment Parser \n")

    def Parse(self):
        root = ET.fromstring(self.xmlString)

        # tree = ET.parse(self.xmlString)
        # root = tree.getroot()

        ns = (root.tag.split("}"))[0] + "}"

        Deployments = {}
        Instances=[]
        serviceInterfaces = []
        ServiceInstances = []
        R_ServiceInstances=[]
        for node in root.iter(ns + "AR-PACKAGE"):

            if node.find(ns + "SHORT-NAME").text == "deployments":

                RootElements = node.find(ns + "ELEMENTS")

                for service in RootElements.iter(ns + "SOMEIP-SERVICE-INTERFACE-DEPLOYMENT"):
                    # GET SERVICE INTERFACE NAME AND ID
                    serviceName = (service.find(ns + "SHORT-NAME").text).split("_")[0]
                    serviceInterfaces += serviceName
                    serviceId = service.find(ns + "SERVICE-INTERFACE-ID").text

                    # GET METHOD IDS
                    serviceMethods = {}
                    for method in service.findall(ns + "METHOD-DEPLOYMENTS/" + ns + "SOMEIP-METHOD-DEPLOYMENT"):
                        methodName = method.find(ns + "SHORT-NAME").text
                        # methodPath = method.find(ns + "METHOD-REF").text
                        methodId = method.find(ns + "METHOD-ID").text
                        # Dictionary key = methodID , Values are methodName and methodPath
                        # serviceMethods[methodName] = [methodId, methodPath]
                        serviceMethods[methodName] = methodId
                    # print(serviceMethods)

                    # GET FIELD IDs
                    serviceFields = {}
                    for field in service.findall(ns + "FIELD-DEPLOYMENTS/" + ns + "SOMEIP-FIELD-DEPLOYMENT"):
                        # FIELD NAME
                        fieldName = field.find(ns + "SHORT-NAME").text

                        # FIELD GETTERS
                        fieldGet = field.find(ns + "GET")
                        if fieldGet != None:
                            fieldGet = fieldGet.find(ns + "METHOD-ID").text
                        else:
                            fieldGet = "No Getter Method"

                        # FIELD SETTERS
                        fieldSet = field.find(ns + "SET")
                        if fieldSet != None:
                            fieldSet = fieldSet.find(ns + "METHOD-ID").text
                        else:
                            fieldSet = "No Setter Method"

                        # FIELD NOTIFIERS
                        fieldNot = field.find(ns + "NOTIFY")
                        if fieldNot != None:
                            fieldNot = fieldNot.find(ns + "METHOD-ID").text
                        else:
                            fieldNot = "No Notifier Method"

                        serviceFields[fieldName] = [fieldGet, fieldSet, fieldNot]
                    Deployments[serviceName] = [serviceId, serviceMethods, serviceFields]     
            # SERVICE INSTANCES
            if node.find(ns + "SHORT-NAME").text == "Instances":
                RootElements = node.find(ns + "ELEMENTS")
                # provided instances parser
                for instance in RootElements.findall(ns + "PROVIDED-SOMEIP-SERVICE-INSTANCE"):
                    InstName = (instance.find(ns + "SHORT-NAME").text)
                    Dep_ref = (instance.find(ns + "SERVICE-INTERFACE-DEPLOYMENT-REF").text)
                    Dep_Name = Dep_ref.split("/")[-1]
                    InstanceName = Dep_Name.split("_")[0]
                    instanceId = int(instance.find(ns + "SERVICE-INSTANCE-ID").text)

                    serviceId = int(Deployments[InstanceName][0])
                    ServiceInstances = [InstName, serviceId, instanceId]
                    Instances.append(ServiceInstances)
                #required instances parser
                for instance in RootElements.findall(ns + "REQUIRED-SOMEIP-SERVICE-INSTANCE"):
                    R_InstName = (instance.find(ns + "SHORT-NAME").text)
                    R_Dep_ref = (instance.find(ns + "SERVICE-INTERFACE-DEPLOYMENT-REF").text)
                    R_Dep_Name = R_Dep_ref.split("/")[-1]
                    R_InstanceName = R_Dep_Name.split("_")[0]
                    R_instanceId = int(instance.find(ns + "REQUIRED-SERVICE-INSTANCE-ID").text)

                    R_serviceId = int(Deployments[R_InstanceName][0])
                    R_ServiceInstances = [R_InstName, R_serviceId, R_instanceId]
                    Instances.append(R_ServiceInstances)
        # print(Deployments) 
        return Deployments, Instances

