import xml.etree.ElementTree as ET


class DeploymentParser:
    def _init_(self, xmlString):
        self.xmlString = xmlString
        print("Deployment Parser \n")
    
    def Parse(self):
        root = ET.fromstring(self.xmlString)
        # root = tree.getroot()
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
                    for field in service.findall(ns + "FIELD-DEPLOYMENTS/" + ns + "SOMEIP-FIELD-DEPLOYMENT"):
                        fieldName = field.find(ns + "SHORT-NAME").text
                        fieldPath = field.find(ns + "FIELD-REF").text
                        fieldGet = field.find(ns + "GET")
                        if fieldGet != None:
                            fieldGet = fieldGet.find(ns + "METHOD-ID").text
                        else:
                            fieldGet = "No Getter Method"

                        fieldSet = field.find(ns + "SET")
                        if fieldSet != None:
                            fieldSet = fieldSet.find(ns + "METHOD-ID").text
                        else:
                            fieldSet = "No Setter Method"

                        fieldNot = field.find(ns + "NOTIFY")
                        if fieldNot != None:
                            fieldNot = fieldNot.find(ns + "METHOD-ID").text
                        else:
                            fieldNot = "No Notifier Method"

                    serviceFields[fieldName] = [fieldGet,fieldSet, fieldNot]

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

    def _init_(self,id,name,methods,fields=None):
        serviceId = id
        serviceName = name
        serviceMethods = methods   
        serviceFields = fields