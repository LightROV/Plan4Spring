
#include <cstdlib>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "async_video_receiver.hpp"

ltrov::network::AsyncVideoReceiver* receiver;

void updateWindow() {
    while (1) {
        cv::Mat img = receiver->getImg();
        cv::resize(img, img, cv::Size(800, 600));
        cv::imshow("picture", img);
        if (cv::waitKey(20) == 'q') break;
    }
}

int main() {
    cv::namedWindow("picture", cv::WINDOW_AUTOSIZE);

    boost::asio::io_service io;
    try {
        receiver = new ltrov::network::AsyncVideoReceiver(io);
        receiver->notify();
        receiver->start();
        boost::thread ioRunThread(boost::bind(&boost::asio::io_service::run, &io));
        boost::thread thread(&updateWindow);
        
        boost::asio::io_service tIO;
        boost::asio::deadline_timer timer(tIO, boost::posix_time::seconds(10));
        timer.wait();
        
        receiver->stop();
        io.stop();
    } catch (std::exception& e) {
        using namespace std;
        cout << e.what() << endl;
    }
    
    return 0;
}