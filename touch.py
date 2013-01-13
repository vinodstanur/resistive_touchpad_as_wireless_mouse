#!/usr/bin/ipython
import serial,time,re,os
from pymouse import PyMouse

m=PyMouse()
size=m.screen_size()
ser=serial.Serial()
ser.port='/dev/rfcomm2'
ser.baudrate=115200
ser.timeout=.1
ser.open()
prev_x=0
prev_y=0
speed = 1
count=0

right_click_debounce = 0
left_click_debounce = 0
wheel_up_debounce = 0
wheel_down_debounce = 0

xx = range(40)
yy = range(40)
xx_index=0
yy_index=0
for i in range(len(xx)):
	xx[i]=0
	yy[i]=0
	
while 1:
	a=ser.readline()
	cursor=re.findall('\d+',a)
	if len(cursor) == 2:
		x=int(cursor[1])
		y=int(cursor[0])
		print str(x) + ' ' + str(y)
		xx[xx_index]=x;
		yy[yy_index]=y;
		xx_index=xx_index+1
		yy_index=yy_index+1
		if xx_index==40:xx_index=0
		if yy_index==40:yy_index=0
		
		XX=sum(xx)/40
		YY=sum(yy)/40
		if x >720 and y > 730:
			right_click_debounce = right_click_debounce+1
			if right_click_debounce == 40:
				os.system("xdotool click 3")
				right_click_debounce = 0
		elif x < 300 and y > 730:
			left_click_debounce = left_click_debounce+1
			if left_click_debounce == 40:
				os.system("xdotool click 1")
				left_click_debounce = 0
		elif x > 820 and y < 400:
			wheel_up_debounce = wheel_up_debounce+1
			if wheel_up_debounce == 10:
				os.system("xdotool click 4")
				wheel_up_debounce = 0
		elif x > 820 and y > 400 and y < 730:
			wheel_down_debounce = wheel_down_debounce+1
			if wheel_down_debounce == 10:
				os.system("xdotool click 5")
				wheel_down_debounce = 0
		else:
			right_click_debounce = 0
			left_click_debounce = 0
			wheel_up_debounce = 0
			wheel_down_debounce = 0
			X=XX-prev_x
			Y=YY-prev_y
			if abs(X) <10 and abs(Y)<10:
				count=count+1
				if count==45:
					speed=1
				current_pos = m.position()
				m.move(current_pos[0]+(X*speed*2),current_pos[1]+(Y*2*speed))
			prev_x=XX
			prev_y=YY
	else:
		speed = 0
		count = 0
		prev_x=0
		prev_y=0
