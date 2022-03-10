from fileinput import filename
from django.db import models
import os

def file_path(instance,filename):
   path="documents/"
   format=filename
   return os.path.join(path,format)


# Create your models here.

class FileHandler(models.Model):
   file_upload=models.FileField(upload_to=file_path,blank=True, null=True)
   file_name=models.CharField(null=False,max_length=500,default='')
  # file_version=models.FloatField(default=0)

   def __str__(self) :
      return self.file_name
