from django import views
from django.urls import URLPattern, path
from core.views import Indexview
from core.views import download_file

app_name='core'
urlpatterns=[
    path('',Indexview.as_view(),name='index'),
    path('download/', download_file)

]
