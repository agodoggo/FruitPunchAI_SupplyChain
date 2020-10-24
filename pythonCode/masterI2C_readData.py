#  Raspberry Pi Master for Arduino Slave
#  i2c_master_pi.py
#  Connects to Arduino via I2C
  
#  DroneBot Workshop 2019
#  https://dronebotworkshop.com

from smbus2 import SMBus

numBoards = 17 #Number of Boards in the game
boardAddr = [x for x in range (8,8+numBoards)] # board addresses 8 - 25
boardStoneCount = [0]*17
with SMBus(1) as bus: # indicates /dev/ic2-1
   print ("Enter 1 for ON or 0 for OFF")
   numb = 1
   while numb == 1:
      boardIndex=0
      getData = input(">>>>   ")
      if getData == 1:
         for addr in boardAddr:
            boardStoneCount[boardIndex] = bus.read_byte_data(addr, 0, 32) # read 32 bytes from address 'addr', offset 0
            print("Board Number ")
            print(boardIndex)
            print(": ")
            print(boardStoneCount[boardIndex])
            print("\n")
            boardIndex = boardIndex+1
      else:
         numb = 0
         
