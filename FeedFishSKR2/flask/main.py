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
global Red
global Green
global Blue
Mode = 0
delay = '0'
Red = 0
Green = 0
Blue = 0

@app.route("/data/<t>/<h>")																			#Get data From Client(Wemos)
def data(t,h):
	global Red
	global Green
	global Blue
	global Mode
	global delay
	socketio.emit('s2c_status',"Online")
	socketio.emit('s2c_humi',h)
	socketio.emit('s2c_temp',t)
	return ("%s,%s,%03d,%03d,%03d"%(Mode,delay,Red,Green,Blue))
	#return(c)
@socketio.on('c2s')																				#listen Data From Browser parth socketio "c2s" = cilent to server 
def C2S(data):
	global Mode
	sdata=json.loads(data)
	ComP = int(sdata['P'])
	print("solinoid %s"%(ComP))
	if (ComP == 1):
		Mode = 1
		print ("Solinoid_ON")
	if(ComP == 0):
		Mode = 0
		print ("Solinoid_OFF")
	if(ComP == 2):
		Mode = 2
		print ("Solinoid_ON+SEC")
	#return(c)
@socketio.on('c2s_led')																				#listen Data From Browser parth socketio "c2s" = cilent to server 
def C2S_led(data):
	global Red
	global Green
	global Blue
	sdata=json.loads(data)
	Red = float(sdata['R'])
	Green = float(sdata['G'])
	Blue = float(sdata['B'])
	print("Red%03d,Green%03d,Blue%03d"%(Red,Green,Blue))

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
@app.route("/admin3")
def ad3():
	return render_template('led.html')

@app.route("/switch")
def switch():
	return render_template('switch.html')
@app.route("/status")
def status():
	return render_template('status.html')
 

