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
    std::cout << "hi" << "\n";

    MasterPtr src = ris::Master::create();                                     // Creation of Master & Slave object ptrs
    SlavePtr dst = ris::Slave::create();
    ris::FramePtr frame;
    ris::FrameIterator it;
    uint32_t x;

    *src >> dst;                                                               // Connecting Master to Slave


    std::cout << "Master Pointer Location: ";
    std::cout << src << "\n";
    std::cout << "Slave Pointer Location : ";
    std:: cout << dst << "\n";

    return 0;
}