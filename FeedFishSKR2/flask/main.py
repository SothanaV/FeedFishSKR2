from flask import Flask, render_template, request, jsonify
from flask_socketio import SocketIO, send, emit
from datetime import datetime,timedelta
import json
#from StringIO import StringIO
import sqlite3

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)
##Defind Start Value##
#io = StringIO()

global Mode
global delay
Mode = "0"
delay = "0"

@app.route("/data/<c>")																			#Get data From Client(Wemos)
def data(c):
	global Mode
	global delay
	print(c)
	return "%s,%s"%(Mode,delay)
	#return(c)
@socketio.on('c2s')																				#listen Data From Browser parth socketio "c2s" = cilent to server 
def C2S(data):
	global Mode
	sdata=json.loads(data)
	ComP = int(sdata['P'])
	print("solinoid %s"%(ComP))
	if (ComP == 1):
		Mode = "1"
		print ("Solinoid_ON")
	if(ComP == 0):
		Mode = "0"
		print ("Solinoid_OFF")
	if(ComP == 2):
		Mode = "2"
		print ("Solinoid_ON+SEC")
@socketio.on('c2s_d')
def delay(dt):
	global delay
	delay = dt
	print(dt)
	#return(dt)
@app.route("/admin")
def ad():
	return render_template('UrbanControl.html')
@app.route("/admin2")
def ad2():
	return render_template('secFeed.html')
@app.route("/switch")
def switch():
	return render_template('switch.html')
 

