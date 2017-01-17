 #!/usr/bin/env python
          
from datetime import datetime      
import time
import serial
import locale
import os
def datetime_from_utc_to_local(utc_datetime):
    now_timestamp = time.time()
    offset = datetime.fromtimestamp(now_timestamp) - datetime.utcfromtimestamp(now_timestamp)
    return utc_datetime + offset
locale.setlocale(locale.LC_TIME,'de_DE.UTF-8')
ser = serial.Serial(
              
               port='/dev/ttyACM0',
               baudrate = 9600,timeout=5 )
try:
	ser.open()
except Exception:
	ser.close()
	time.sleep(5)
	ser.open()
print("port open")
time.sleep(3)# auf arduino warten
print(ser.readline())
timeout=60*60*10 # alle 10 Minuten Uhrzeit mit arduino syncen
try:
	while 1:
		
		# time conversion utc to locale
		loc=datetime_from_utc_to_local(datetime.utcnow())
		print(datetime.now())
		locTimeEpoch=int(loc.strftime("%s"))+(60*60)  
		temp='T'+repr(locTimeEpoch)[0:10]
		#temp='T'+os.popen('date +%s').read()
		#temp='T'+repr(int(time.time()))
		ser.write(temp)
		print(temp)
		while time.time() < locTimeEpoch+timeout:
			readA=ser.readline()
			if readA!="":
				print(readA)
except KeyboardInterrupt:
	ser.close()
