#!/usr/bin/python
from packetizer import Packetizer
import time

def main():
	p = Packetizer('/dev/ttyUSB0')

	factor = 256.0/15000000.0/16
	while (1):
		packet = p.get_packet();
		print packet
		raw = packet[1] << 8 | packet[2]
		print raw
		print 1.0/(factor*raw)


if __name__ == "__main__":
	main()
