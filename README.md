Simple Morse code trainer device
================================

A small box for improving Morse code copy under difficult conditions, or so I hope!  Wiring assumptions:

1. Speaker on digital pin D13.  I put a variable resistor in series to control the volume.
2. Potentiometer on Analog pin A0 for controlling the code speed.

I built this using an Arduino Nano in a small plastic box, in which the power is a 9V battery with a small switch to deactivate.  It sends random length strings of between 3 and 5 random characters with a longish pause between them.  It's quite simplistic. 