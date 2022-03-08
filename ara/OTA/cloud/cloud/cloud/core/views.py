from logging import FileHandler
from multiprocessing import get_context
import re
from django.shortcuts import redirect, render
from django.template import context
from django.views.generic import TemplateView
from .models import FileHandler, file_path
from .forms import FileHandlerform
import mimetypes
from django.http import HttpResponse
import os

# Create your views here.


class Indexview(TemplateView):
    template_name='core/index.html'

    def get_context_data(self, **kwargs):
        context=super().get_context_data(**kwargs)
        get_files=FileHandler.objects.all()
        context={'form':FileHandlerform,'get_files':get_files}
        return context

    def post(self,request,**kwargs):
        context= {}
        if request.method == 'POST':
            form=FileHandlerform(request.POST or None ,request.FILES or None)
            if form.is_valid():
                obj=form.save(commit=False)
                obj.save()

                return redirect('core:index')

            else:
                context['form']=form
        else:
            context['form']= form

        return redirect('core:index')




def meta_data_send(request):
    data=FileHandler.objects.all()
    print(data)
    f = open("demofile2.txt", "w")
    print("*******************")
    for x in data:
        print(x)
        f.write(str(x))
        f.write('\n')
    f.close()
    filename ="demofile2.txt"
    filepath="demofile2.txt"
    path = open(filepath, 'r')
    # Set the mime type
    mime_type, _ = mimetypes.guess_type(filepath)
    # Set the return value of the HttpResponse
    response = HttpResponse(path, content_type=mime_type)
    # Set the HTTP header for sending to browser
    response['Content-Disposition'] = "attachment; filename=%s" % filename
    # Return the response value
    return response






def download_file(request,value):
    # Define Django project base directory
    BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    # Define text file name
    filename = value
    # Define the full file path
    filepath = BASE_DIR+'/cloud/media/documents/'+ filename
    # Open the file for reading content
    path = open(filepath, 'rb')
    # Set the mime type
    mime_type, _ = mimetypes.guess_type(filepath)
    # Set the return value of the HttpResponse
    response = HttpResponse(path, content_type=mime_type)
    # Set the HTTP header for sending to browser
    response['Content-Disposition'] = "attachment; filename=%s" % filename
    # Return the response value
    return response