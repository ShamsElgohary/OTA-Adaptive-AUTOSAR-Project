import xml.etree.ElementTree as ET

class DeploymentParser:
    def __init__(self, xmlString):
        self.xmlString = xmlString
        print("Deployment Parser")
    
    def Parse(self):
        Deployment = {}
        return Deployment