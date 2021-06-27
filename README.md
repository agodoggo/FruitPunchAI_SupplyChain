#	Supply Chain Project
This is a project for the EAISI Experience Lab, Supply Chain Demo.
This project is the software for an interactive electronic version of Willem van Jaarsveld and Remco Dijkman's Supply Chain Board Game.
The game includes the instructions for playing.
There are two sides for two players: a side assisted by AI, and one unassisted by AI.

Each side's interface is displayed on a Waveshare 10.1" LCD screen (https://www.waveshare.com/10.1inch-hdmi-lcd-b-with-case.htm).
Each side is controlled by a Raspbery Pi 4B on which the appropriate version of the Processing program is running.
This is in the processingCode folder. The AI side uses the processingCode/SupplyChain_SensorSide code, and the non-AI side uses the processingCode/SupplyChain_nonSensorSide.
The AI side can be distinguished from the non-AI side by the light-dependent resisters embedded in the different board areas for inserting the game pieces.

Software Instructions

Both raspberry Pis have the appropriate software (Processing) to run, as seen above.
The arduinos have their own appropriate software as well.
The ATMega Arduino on the non Raspberry Pi side of the Sensor Side runs using the arduinoCode/FinalVersion/SensorSide_Board1/SensorSide_Board1.ino
The ATMega Arduino on the Raspberry Pi side of the Sensor Side runs using the arduinoCode/FinalVersion/SensorSide_Board2/SensorSide_Board2.ino
The Arduino Uno on the non Raspberry Pi side of the Sensor Side runs using the arduinoCode/FinalVersion/nonSensorSide/nonSensorSide.ino

Hardware Instructions

General
Each area into which the playing pieces can be inserted has circuitry for controlling the LEDs and detecting the presence of a game piece.
The LEDs are driven by 5V USB power (from the wall) with a resistor attached, therefore it should turn on when plugged into a wall outlet.
The LDR piece detection circuit is a voltage divider circuit with 5V USB power (from the Arduino) and a 10K ohm resistor in series with the LDR.
The LDR comes first in the series, and the node between the LDR and 10K ohm is connected to a GPIO pin of the Arduino Mega.
The Arduino Mega can therefore tell which LDRs are covered or not, based on the voltage from the pin.
The voltage divider circuit is designed so that when the LDR is covered, the voltage drop across it drops below 1.5 V, the maximum voltage that can be considered digital LOW in an Arduino, and when the LDR is uncovered, the voltage drop across it is above 3.0 V, the minimum voltage to be considered digital HIGH (https://learn.sparkfun.com/tutorials/logic-levels/all).
Each digital pin on the Arduino can therefore tell the state of the LDR, covered is LOW, and uncovered is HIGH.
The digital pins that connect to each distinct area are below (the numbering of the board areas can be found in the instructions to the board game, please reach out to Willem van Jaarsveld or Remco Dijkman for a copy):

Sensor Side Board 1 (non Raspberry Pi Side) - Arduino Mega
  {24,25,26,27,28,29} - Board Area 1
  {14,15,16,17,18,19} - Board Area 0
  {2,3,4,5,6,7} - Board Area 3
  {48,49,50,51,52,53} - Board Area 16
  {8,9,10,11,12,13} - Board Area 10
  {30,31,32,33,34,35} - Board Area 5
  {A2,A3,A4,A5,A6,A7} - Board Area 7
  {42,43,44,45,46,47} - Board Area 14
  {A10,A11,A12,A13,A14,A15} - Board Area 2
  {36,37,38,39,40,41} - Board Area 6
  {A8,A9} - Board Area 4

Sensor Side Board 2 (Raspberry Pi Side) - Arduino Mega
  {22,24,26,28,30,32,34,36,38,40} - Board Area 8
  {4,5,6,7,8,9,10,11,12,13} - Board Area 15
  {23,25,27,29,31,33,35,37,39,41} - Board Area 9
  {42,44,46,48,50,52} - Board Area 13
  {A0,A1,A2,A3,A4,A5} - Board Area 12
  {A14,A15} - Board Area 11

Pins Controlling the LED arrows and the score count mechanism:

Non-Sensor Side
2 - LED Group E2 (Evolve 2)
3 - LED Group E1 (Evolve 1)
4 - LED Group P (Planning - Assembly)
5 - Stone Counter Signals (pullup 20k)
6 - LED controller (Demand Phase)
7 - IL arrow (International Logistics)

Sensor Side
23 (Board 1) - LED Group E2 (Evolve 2)
A8 (Board 2) - LED Group E1 (Evolve 1)
22 (Board 1) - LED Group P (Planning - Assembly)
3  (Board 2) - IL arrow (International Logistics)
A9  (Board 2) - Stone Counter Signals (pullup 20k)
2 - LED controller (Demand Phase)

Communication

The Raspberry Pis on both sides communicate with each other through their serial ports, using the GPIO serial pins on each Raspberry Pi.
The Arduinos and Raspberry Pis also communicate using their serial ports, the connection is done through USB.


Arrows Lighting

The arrows which light up are controlled by a MOSFET circuit.
The MOSFET acts as a switch to turn the LEDs connected on and off.
All of the arrows which must light up at once in the same phase are connected to the same MOSFET.

Miscellaneous

Both Raspberry Pis can be connected to from a PC using VNCShare.
On bootup, the Raspberry Pis and Arduinos should run the software immediately, allowing one to play.
The folder in which the code is located on each Raspberry Pi is in home/pi/documents.
The raspberry Pis can get out of sync if one player exits out of the game and the other doesn't, make sure that both players move through the phases together, and if one wishes to quit the other one must as well.
If you have any questions about the Arduino code or processing code or in general about the project, please contact me at agoston.walter@gmail.com or +17575534281.
I will do my best to help you troubleshoot an error or get set up with the board game.



Sincerely,

Ágoston Ottó Walter
Eindhoven 27 June, 2021
