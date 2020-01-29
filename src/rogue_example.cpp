#include <rogue/interfaces/stream/Master.h>
#include <rogue/interfaces/stream/Frame.h>
#include <rogue/interfaces/stream/FrameIterator.h>
#include <rogue/interfaces/stream/Fifo.h>
#include <iostream>

class MyCustomMaster : public rogue::interfaces::stream::Master {
   public:

      // Create a static class creator to return our custom class
      // wrapped with a shared pointer
      static std::shared_ptr<MyCustomMaster> create() {
         static std::shared_ptr<MyCustomMaster> ret =
            std::make_shared<MyCustomMaster>();
         return(ret);
      }

      // Standard class creator which is called by create
      MyCustomMaster() : rogue::interfaces::stream::Master() { }

      void myFrameGen() {
         rogue::interfaces::stream::FramePtr frame;
         rogue::interfaces::stream::FrameIterator it;
         uint32_t x;

         uint32_t  size;
         uint8_t * data;

         

         // First request an empty from from the primary slave
         // The first arg is the size, the second arg is a boolean
         // indicating if we can allow zero copy buffers, usually set to true

         // Here we request a frame capable of holding 100 bytes
         frame = reqFrame(100,true);

         // Unlike the python API we must now specify the new payload size
         frame->setPayload(20);

         // Here we get an iterator to the frame data
         it = frame->begin();

         // Set an incrementing value to the first 10 locations
         for (x=0; x < 10; x++) {
            *it = x;
            it++;
         }

         // Use std::copy to copy data from a data buffer
         // Here we copy 10 bytes starting a the current position of 10
         // Update the iterator
         it = std::copy(data, data+10, it);

         //Send frame
         sendFrame(frame);
      }
};

//Shared pointer alias
typedef std::shared_ptr<MyCustomMaster> MyCustomMasterPtr;

int main(int argc, char const *argv[])
{
    std::cout << "hi";

    //rogue::interfaces::stream::FifoPtr fifo = rogue::interfaces::stream::Fifo::create(100, 0, true);

    return 0;
}
