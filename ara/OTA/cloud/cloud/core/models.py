from fileinput import filename
from django.db import models
import os

#def file_path(instance,filename):
 #   path="\home"
  #  format='uploaded.'+filename
   # return os.path.join(path,format)


# Create your models here.

class filehandler(models.Model):
    fileupload=models.FileField(upload_to="")

  #  def __str__(self):
   #     return str(self.file_upload)
