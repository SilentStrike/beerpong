#!/usr/bin/python
from packetizer import Packetizer
import time

def main():
	p = Packetizer('/dev/ttyUSB0')
	packetizer_code = 0	
	pwm = 0
	packet = [packetizer_code , pwm]
	while (1):
		packet[1] = pwm
		p.send_packet(packet)
		time.sleep(1)
		pwm += 1
		if(pwm == 100):
			break;

if __name__ == "__main__":
	main()
