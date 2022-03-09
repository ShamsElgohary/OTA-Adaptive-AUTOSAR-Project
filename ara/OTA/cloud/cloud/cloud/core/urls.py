from django import views
from . import views 
from django.urls import URLPattern, path
from django.contrib.auth.views import LogoutView
from core.views import Indexview
from core.views import download_file
from core.views import meta_data_send
from core.views import *

app_name='core'
urlpatterns=[
    path('',views.login_page, name="login"),
    path('index',Indexview.as_view(),name='index'),
    path('download/<str:value>/', download_file),
    path('meta_data_send', meta_data_send),
    path('logout/',views.logout_view, name="logout"),
]
