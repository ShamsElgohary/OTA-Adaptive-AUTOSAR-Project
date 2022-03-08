from django import views
from django.urls import URLPattern, path
from core.views import Indexview
from core.views import download_file
from core.views import meta_data_send

app_name='core'
urlpatterns=[
    path('',Indexview.as_view(),name='index'),
    path('download/<str:value>/', download_file),
    path('meta_data_send', meta_data_send)

]
