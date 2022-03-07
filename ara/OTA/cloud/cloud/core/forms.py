from django import forms
from core.models import filehandler

class FileHandlerform(forms.ModelForm):

    file_upload=forms.FileField()

    class Meta():
        model = filehandler
        fields =('file_upload',)
