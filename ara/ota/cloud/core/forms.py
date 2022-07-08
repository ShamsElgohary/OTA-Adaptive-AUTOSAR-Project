from email.policy import default
from django import forms
from core.models import FileHandler

class FileHandlerform(forms.ModelForm):
    file_upload=forms.FileField(required=False)
    file_name=forms.CharField(widget=forms.TextInput(attrs={'placeholder': 'Write file name'}))

    class Meta():
        model = FileHandler
        fields =('file_upload','file_name',)