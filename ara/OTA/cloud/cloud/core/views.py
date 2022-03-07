from logging import FileHandler
from multiprocessing import get_context
import re
from django.shortcuts import redirect, render
from django.template import context
from django.views.generic import TemplateView
from core.models import filehandler
from core.forms import FileHandlerform
# Create your views here.


class Indexview(TemplateView):
    template_name='core/index.html'

    def get_context_data(self, **kwargs):
        context=super().get_context_data(**kwargs)
        context={'form',FileHandlerform}

    def post(self,request,**kwargs):
        context={}
        if request.method == 'post':
            form=FileHandlerform(request.post,request.FILES)
        
            if form.is_valid():
                FileHandler.objects.get_or_create(file_upload=form.cleaned_data.get('file_upload'))
                return redirect('core:index')
            else:
                context['form']=form
        else:
            context['form']=form
        
        return redirect('core:index')