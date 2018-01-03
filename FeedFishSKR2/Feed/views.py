from django.shortcuts import render,HttpResponse,redirect
from django.contrib.auth.decorators import login_required
# Create your views here.
def vol_food(request,type,ifish):
	fish = int(ifish)
	if(type=='1'):
		volfood = (4*fish)/100
	if(type=='2'):
		volfood = (8*fish)/100 
	return HttpResponse( volfood )

@login_required
def home(request):
	return render(request, 'first.html')

