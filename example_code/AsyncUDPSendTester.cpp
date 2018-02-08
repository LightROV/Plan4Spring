#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "async_video_sender.hpp"

int main(int argc, char* args[]) {
    boost::asio::io_service io;
    AsyncVideoSender* sender = new AsyncVideoSender(io, 0);
    sender->setFrameSize(1200, 900);
    sender->start();
    io.run();
    return 0;
}