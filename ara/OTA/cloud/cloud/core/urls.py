from django.urls import URLPattern, path
from core.views import Indexview
app_name='core'
urlpatterns=[
    path('/home',Indexview.as_view(),name='index'),
]
