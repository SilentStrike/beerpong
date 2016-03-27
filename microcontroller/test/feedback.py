#!/usr/bin/python
from packetizer import Packetizer
import time

def main():
	p = Packetizer('/dev/rfcomm0')

	factor = 256.0/15000000.0/16
	while (1):
		packet = p.get_packet();
		print packet
		raw = packet[1] << 8 | packet[2]
		print raw
		if (raw != 0):
			print 1.0/(factor*raw)
		else:
			print "0"


if __name__ == "__main__":
	main()
