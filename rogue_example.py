import pyrogue
import rogue.interfaces.stream as pystream

def newLine():
    print('\n')


print('*****************************************')
print('******* TEST USAGE OF ROGUE *************')
print('*****************************************')
newLine()

print('**** CREATE POINTER TO MASTER OBJECT ****')
myMaster = pystream.Master()
print(myMaster)
newLine()

print('************** INITIAL ******************')
print('Slave Count: ' + str(myMaster._slaveCount()))       # Returns the number of slaves connected to master
newLine()

print('***** CREATE POINTER TO SLAVE OBJECT *****')
mySlave = pystream.Slave()
print(mySlave)
newLine()

print('************** INITIAL ******************')
print('Frame Count: ' + str(mySlave.getFrameCount()))      # Returns the number of frames received
print('Byte Count : ' + str(mySlave.getByteCount()))       # Returns the number of bytes received
newLine()

myMaster >> mySlave                                        # Connects master to slave with >> operation
                                                           # Alternatively can connect with pyrogue.streamConnect(myMaster,mySlave)

# Create a few more slave object pointers
slave1 = pystream.Slave()
slave2 = pystream.Slave()
slave3 = pystream.Slave()

myMaster >> slave1
myMaster >> slave2
myMaster >> slave3

print('*************** Slave1 ******************')
print(slave1)
newLine()
print('*************** Slave2 ******************')
print(slave2)
newLine()
print('*************** Slave3 ******************')
print(slave3)
newLine()

print('Slave Count: ' + str(myMaster._slaveCount()))
newLine()

frame = myMaster._reqFrame(100,True)                       # Requesting frame of size 100 with zero-copy enabled

print('*************** Frame *******************')
print('Frame Size     : ' + str(frame.getSize()))          # Total size of frame
print('Remaining Space: ' + str(frame.getAvailable()))     # Remaining space in frame for payload
print('Payload Size   : ' + str(frame.getPayload()))       # Payload size of frame
newLine()

ba1 = bytearray([i for i in range(10)])
frame.write(ba1,0)                                         # Writes the byte array to offset of 0
                                                           # Location 0-9 are filled
ba2 = bytearray([i*2 for i in range(10)])
frame.write(ba2,50)                                        # Writes the new byte array to offset of 50
                                                           # Locations 50-59 are filled
                                                           # Locations 10-49 are not known

myMaster._sendFrame(frame)

print('************** Slave ********************')
print('Frame Count: ' + str(mySlave.getFrameCount()))
print('Byte Count : ' + str(mySlave.getByteCount()))
newLine()

print('********** Byte Array Sent to Slave ***********')
print(ba1 + ba2)
newLine()

print('*************** Frame *******************')
print('Frame Size     : ' + str(frame.getSize()))          # Total size of frame
print('Remaining Space: ' + str(frame.getAvailable()))     # Remaining space in frame for payload
print('Payload Size   : ' + str(frame.getPayload()))       # Payload size of frame
newLine()
with frame.lock():
    size = frame.getPayload()

    fullData = bytearray(size)                                 # Create empty array using size of the payload

    frame.read(fullData,0)                                     # Read data from frame starting at an offset of 0
                                                               # and save it into fullData bytearray
    print('************* Full Payload Data ****************')
    print(fullData)
    newLine()

    partialData1 = bytearray(10)                               # Create 2 empty arrays with size of 10  
    partialData2 = bytearray(10) 

    frame.read(partialData1,0)                                 # Read data from offset of 0 (reading the payload data passed
                                                               # from original bytearray (ba)) and write into partialData bytearray
    frame.read(partialData2,50)                                # Reading the last 10 from the payload that was written from the 
                                                               # second byte array
print('************ Original Payload Data **************')
print(partialData1 + partialData2)
newLine()


src = pystream.Master()
dst = pystream.Slave()
fifo = pystream.Fifo(100,0,True)

src >> fifo >> dst                                         # Connect fifo to master and slave to master
                                                           # Fifo will be the primary slave and be the last to
                                                           # receieve data

print('****************** Fifo ************************')
print(fifo)

