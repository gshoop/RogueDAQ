#include <rogue/interfaces/stream/Master.h>
#include <rogue/interfaces/stream/Slave.h>
#include <rogue/interfaces/stream/Frame.h>
#include <rogue/interfaces/stream/FrameIterator.h>
#include <rogue/interfaces/stream/Fifo.h>
#include <iostream>

namespace ris = rogue::interfaces::stream;                                     // Creating namespace for stream interface

//Shared pointer aliases
typedef std::shared_ptr<rogue::interfaces::stream::Master> MasterPtr;
typedef std::shared_ptr<rogue::interfaces::stream::Slave> SlavePtr;


int main(int argc, char const *argv[])
{
    std::cout << "*************** ROGUE CPP EXAMPLES ******************" << "\n\n\n";

    MasterPtr src = ris::Master::create();                                     // Creation of Master & Slave object ptrs
    SlavePtr dst = ris::Slave::create();
    ris::FramePtr frame;
    ris::FrameIterator it;
    uint32_t x;
    uint32_t size;
    uint64_t data64;
    uint32_t data32;
    uint8_t data8;
    uint8_t * data;
    *src >> dst;                                                               // Connecting Master to Slave

    // Output addresses for Master & Slave Pointers
    std::cout << "Master Pointer Location: ";
    std::cout << src << "\n";
    std::cout << "Slave Pointer Location : ";
    std::cout << dst << "\n";

    std::cout << "Master Slave Count: " << src->slaveCount() << "\n";

    // We will request an empty frame from the primary slave (dst)
    frame = src->reqFrame(100,true);                                           // First param is min. size of frame & second param is true for zero copy frame
                                                                               // setting false allows master to send the same frame multiple times
    frame->setPayload(100);                                                    // Set payload size

    it = frame->begin();

    while ( it != frame->end()) {
       size = it.remBuffer();                                                  // remBuffer() gets the remaining bytes in the current buffer

       it = std::copy(data, data+size, it);
       data += size;
    }
    
    src->sendFrame(frame);                                                     // sendFrame() sends the passed frame to all current slaves
                                                                               // if zeroCopy after sendFrame(arg) returns arg frame will be emptied
   
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
   //  for (x=0; x<10; x++) {
   //     *it = x;
   //     it++;
   //  }

    //it = std::copy(data, data+10,it);

    //src->sendFrame(frame);
   // ************************************************************

    return 0;
}