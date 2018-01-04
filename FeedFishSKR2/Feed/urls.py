from django.conf.urls import url
from . import views
from django.contrib.auth import views as auth_views



urlpatterns = [
	url(r'^$', views.home, name='home'),
	url(r'cal/?$', views.cal, name='cal'),
	url(r'vol/(?P<type>\d+)/(?P<ifish>\d+)/?$', views.vol_food, name='vol_food'),
]