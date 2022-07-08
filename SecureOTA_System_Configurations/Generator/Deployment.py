import xml.etree.ElementTree as ET

class DeploymentParser:
    def __init__(self, xmlString):
        self.xmlString = xmlString
        print("Deployment Parser \n")
    
    def Parse(self):
        tree = ET.parse(self.xmlString)
        root = tree.getroot()
        ns = (root.tag.split("}"))[0] + "}"
        
        Deployments = {}
        serviceInterfaces=[]
        serviceMethods={}

        for node in root.iter(ns + "AR-PACKAGE"):
            #print(node.find(ns + "SHORT-NAME").text)
            if node.find(ns + "SHORT-NAME").text == "deployments":

                RootElements = node.find(ns + "ELEMENTS")
                # RootElements = RootElements.find(ns + "SOMEIP-SERVICE-INTERFACE-DEPLOYMENT")
                # RootElements = RootElements.find(ns + "SHORT-NAME")

                for service in RootElements.iter(ns + "SOMEIP-SERVICE-INTERFACE-DEPLOYMENT"):
                    #GET SERVICE INTERFACE NAME AND ID
                    serviceName = (service.find(ns + "SHORT-NAME").text).split('_')[0]
                    serviceInterfaces += serviceName
                    serviceId = service.find(ns + "SERVICE-INTERFACE-ID").text

                    print(serviceName, serviceId)  

                    # GET METHOD IDS
                    for method in service.findall(ns + "METHOD-DEPLOYMENTS/" + ns + "SOMEIP-METHOD-DEPLOYMENT"):
                        methodName = method.find(ns + "SHORT-NAME").text
                        methodPath = method.find(ns + "METHOD-REF").text
                        methodId = method.find(ns + "METHOD-ID").text
                        # Dictionary key = methodID , Values are methodName and methodPath
                        serviceMethods[methodId] = [methodName,methodPath]
                        print(methodId, methodName, methodPath)
                        

                    # GET FIELD
                    #for method in service.findall(ns + "METHOD-DEPLOYMENTS/" + ns + "SOMEIP-METHOD-DEPLOYMENT"):

                    serviceInstance = DeploymentInfo(serviceId, serviceName, serviceMethods)
                    Deployments[serviceId] = [serviceInstance]



        #print(serviceMethods)                 


        return Deployments



class DeploymentInfo:

    serviceId = 0
    serviceName = ''
    serviceMethods = {}
    serviceFields = {}
    #serviceEvents = {}

    def __init__(self,id,name,methods,fields=None):
        serviceId = id
        serviceName = name
        serviceMethods = methods   
        serviceFields = fields  


dep = DeploymentParser("/home/shams/Github/OTA-Adaptive-AUTOSAR-Project/SecureOTA_System_Configurations/deployment.arxml")

dep.Parse()