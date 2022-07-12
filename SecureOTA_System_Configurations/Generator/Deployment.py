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
                    serviceName = (service.find(ns + "SHORT-NAME").text).split('_')[0]
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
                    return Deployments
            # SERVICE INSTANCES
            if node.find(ns + "SHORT-NAME").text == "Instances":
                RootElements = node.find(ns + "ELEMENTS")
                # provided instances parser
                for instance in RootElements.findall(ns + "PROVIDED-SOMEIP-SERVICE-INSTANCE"):
                    InstanceName = (instance.find(ns + "SHORT-NAME").text)
                    InstanceName = InstanceName.split('_')[0]
                    instanceId = instance.find(ns + "SERVICE-INSTANCE-ID").text

                    # Path = instance.find(ns + "SERVICE-INTERFACE-DEPLOYMENT-REF").text
                    serviceId = Deployments[InstanceName][0]
                    ServiceInstances.append([InstanceName, serviceId, instanceId])
                    #required instances parser
                    for instance in RootElements.findall(ns + "REQUIRED-SOMEIP-SERVICE-INSTANCE"):
                        R_InstanceName = (instance.find(ns + "SHORT-NAME").text)
                        R_InstanceName = R_InstanceName.split('_')[0]
                        R_instanceId = instance.find(ns + "SERVICE-INSTANCE-ID").text

                        # Path = instance.find(ns + "SERVICE-INTERFACE-DEPLOYMENT-REF").text
                        R_serviceId = Deployments[R_InstanceName][0]
                        R_ServiceInstances.append([R_InstanceName, serviceId, R_instanceId])
                        Instances.append(ServiceInstances)
                        Instances.append(R_ServiceInstances)
        #        print(ServiceInstances)

        #        print(Deployments)


# class DeploymentInfo:

#     serviceId = 0
#     serviceName = ''
#     serviceMethods = {}
#     serviceFields = {}
#     #serviceEvents = {}

#     def __init__(self,id,name,methods,fields=None):
#         self.serviceId = id
#         self.serviceName = name
#         self.serviceMethods = methods
#         self.serviceFields = fields


# dep = DeploymentParser("/home/shams/Github/OTA-Adaptive-AUTOSAR-Project/SecureOTA_System_Configurations/deployment.arxml")
# dep.Parse()