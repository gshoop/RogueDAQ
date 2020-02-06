#include <rogue/interfaces/stream/Master.h>
#include <rogue/interfaces/stream/Slave.h>
#include <rogue/interfaces/stream/Frame.h>
#include <rogue/interfaces/stream/FrameIterator.h>
#include <rogue/interfaces/stream/Fifo.h>
#include <rogue/interfaces/stream/Buffer.h>
#include <rogue/interfaces/stream/FrameLock.h>
#include <iostream>
#include <vector>

namespace ris = rogue::interfaces::stream;                                     // Creating namespace for stream interface

//Shared pointer aliases
typedef std::shared_ptr<ris::Master> MasterPtr;
typedef std::shared_ptr<ris::Slave> SlavePtr;
typedef std::shared_ptr<ris::Fifo> FifoPtr;


int main(int argc, char const *argv[])
{
    std::cout << "*************** ROGUE CPP EXAMPLES ******************" << "\n\n\n";

    MasterPtr src  = ris::Master::create();                                     // Creation of Master & Slave object ptrs
    SlavePtr  dst  = ris::Slave::create();
    MasterPtr srcf = ris::Master::create();
    SlavePtr  dstf = ris::Slave::create();
    FifoPtr   fifo = ris::Fifo::create(100,0,true);
    dst->setDebug(100,"Primary Slave");                                        // Set debug option for slave. Outputs frame data when sent from master
    dstf->setDebug(100, "Slave");
    ris::FramePtr frame;
    ris::FrameIterator it;
    uint32_t x;
    //uint32_t size;
    //uint64_t data64;
    //uint32_t data32;
    //uint8_t data8;
    //uint8_t * data;

    *src >> dst;                                                               // Connecting Master to Slave

    // Output addresses for Master & Slave Pointers
    std::cout << "Master Pointer Location: ";
    std::cout << src << "\n";
    std::cout << "Slave Pointer Location : ";
    std::cout << dst << "\n";

    std::cout << "Master Slave Count: " << src->slaveCount() << "\n\n";

    // We will request an empty frame from the primary slave (dst)
    frame = src->reqFrame(100,true);                                           // First param is min. size of frame & second param is true for zero copy frame
                                                                               // setting false allows master to send the same frame multiple times
    frame->setPayload(10);                                                     // Set payload size

    it = frame->begin();

    for (x=0; x<10; x++) {
       *it = x*2;
       it++;
    }

    // ********************************************************
    // Another method to fill frame
    // while ( it != frame->end()) {
    //    size = it.remBuffer();                                                  // remBuffer() gets the remaining bytes in the current buffer
       
    //    it = std::copy(data, data+size, it);
    //    data += size;
    // }
    // *******************************************************************

    std::cout << "FRAME ATTRIBUTES *********************************\n";
    std::cout << "Size        : " << frame->getSize() << "\n";
    std::cout << "Available   : " << frame->getAvailable() << "\n";
    std::cout << "Payload     : " << frame->getPayload() << "\n";
    std::cout << "Buffer Count: " << frame->bufferCount() << "\n\n";

    std::cout << "Primary Slave Debug ***************************************************\n";     
    src->sendFrame(frame);                                                     // sendFrame() sends the passed frame to all current slaves
                                                                               // if zeroCopy after sendFrame(arg) returns arg frame will be emptied
    
    std::cout << "\nFRAME SENT **************************************\n\n";
    // std::cout << "Size        : " << frame->getSize() << "\n";
    // std::cout << "Available   : " << frame->getAvailable() << "\n";
    // std::cout << "Payload     : " << frame->getPayload() << "\n";
    // std::cout << "Buffer Count: " << frame->bufferCount() << "\n\n";

    std::cout << "Slave Attributes **********************************\n";
    std::cout << "Frame Count: " << dst->getFrameCount() << "\n";
    std::cout << "Byte Count : " << dst->getByteCount() << "\n\n";

    // ***********************************************************
    // REQUESTING ANOTHER FRAME
    // frame = src->reqFrame(100,true);
    // frame->setPayload(13);

    // it = frame->begin();
   
    // toFrame(it, 8, &data64);                                                 // Following methods should write bit data with byte offsets corresponding
                                                                               // to the second paramater
    // toFrame(it, 4, &data32);
   
    // toFrame(it, 1, &data8);
    // ***********************************************************

    // ***********************************************************
    // FIRST METHOD FOR SENDING FRAME WITH DATA
    // for (x=0; x<10; x++) {
    //    *it = x;
    //    it++;
    // }

    //it = std::copy(data, data+10,it);

    //src->sendFrame(frame);
    // ************************************************************

    // RECEIVING FRAMES
    ris::FrameLockPtr lock = frame->lock();
    std::vector<uint32_t> data(frame->getPayload());
    std::vector<uint32_t>::iterator vit = data.begin();
    
    it = frame->begin();
    // Accessing the frame data using the iterator. For loop to iterate through the iterator and de-reference
    // it to retreive the data.
    // for (x=0; x < 10; x++) {
    //     printf("Location %i = 0x%x\n", x, *it);
    //     it++;
    // }
    // std::cout << "\n\n";
    // std::copy(frame->begin(),frame->end(),std::ostream_iterator<uint32_t>(std::cout<< " "));

    std::copy(frame->begin(),frame->end(), vit);

    for (vit = data.begin(); vit != data.end(); vit++) {
        printf("0x%x ", *vit);
    }
    std::cout<< "\n";

    // ****************************** FIFO DEMONSTRATION *****************************************

    *(*fifo << srcf) >> dstf;                                                    // Connecting the fifo to the source and then onto the destination.


    return 0;
}