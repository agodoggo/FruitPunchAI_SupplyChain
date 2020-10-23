#  Raspberry Pi Master for Arduino Slave
#  i2c_master_pi.py
#  Connects to Arduino via I2C
  
#  DroneBot Workshop 2019
#  https://dronebotworkshop.com

from smbus2 import SMBus

numBoards = 17 #Number of Boards in the game
boardAddr = [x for x in range (0,numBoards)] # board addresses 0 - 16
boardStoneCount = [0]*17
with SMBus(1) as bus: # indicates /dev/ic2-1
   while True:
      boardIndex=0
      for addr in boardAddr:
         boardStoneCount[boardIndex] = bus.read_byte_data(addr, 0, 32) # read 32 bytes from address 'addr', offset 0
         print("Board Number ")
         print(boardIndex)
         print(": ")
         print(boardStoneCount[boardIndex])
         print("\n")
         boardIndex = boardIndex+1
