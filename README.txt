Usage:

1. #./superio -i Show all GPIO status and mode


2. superio -p [pin] -m [mode] -s [status]: Set pin x to mode and change status
	ex: Set pin 3 mode to 1 ,status to 0
		#./superio -p 3 -m 1 -s 0

3. superio -p [pin] -m [mode] : Set pin x to output or input 
	ex: Set pin 1 mode to input 
		#./superio -p 1 -m 0 