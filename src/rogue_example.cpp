#include <rogue/interfaces/stream/Master.h>
#include <rogue/interfaces/stream/Slave.h>
#include <rogue/interfaces/stream/Frame.h>
#include <rogue/interfaces/stream/FrameIterator.h>
#include <rogue/interfaces/stream/Fifo.h>
#include <iostream>

namespace ris = rogue::interfaces::stream;

//Shared pointer alias
typedef std::shared_ptr<rogue::interfaces::stream::Master> MasterPtr;
typedef std::shared_ptr<rogue::interfaces::stream::Slave> SlavePtr;


int main(int argc, char const *argv[])
{
    std::cout << "hi" << "\n";

    MasterPtr src = ris::Master::create();
    SlavePtr dst = ris::Slave::create();

    *src >> dst;

    std::cout << dst << "\n";


    return 0;
}