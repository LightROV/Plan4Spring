/* 
 * 一个异步UDP视频传输发送端的实现
 * 
 * created by itsl<itsl@foxmail.com>
 */

#ifndef LIGHTROV_NETWORK_ASYNCVIDEOSENDER_CPP_
#define LIGHTROV_NETWORK_ASYNCVIDEOSENDER_CPP_

#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define SERVER_PORT     7201
#define REQ_FLAG        "2333"
#define CLOSE_FLAG      "stop"
#define ENCODE_QUALITY  60
#define PACK_SIZE       4096

using boost::asio::ip::udp;

class AsyncVideoSender
{
protected:
    const int VIDEO_ID;
    enum ACTION{NOT_ACTION, ACTION_REQUEST, ACTION_CLOSE};
    bool running;
    std::vector < int > compressionParams;
    boost::asio::io_service& io;
    boost::array<uchar, PACK_SIZE> receiveBuffer;
    boost::array<uchar, PACK_SIZE> sendBuffer;
    boost::thread imgStreamThread;
    udp::socket socket;
    udp::endpoint localPort;
    udp::endpoint remotePort;
    cv::VideoCapture cap;

    enum ACTION getAction() {
        std::stringstream ss;
        std::string code;
        for (int i = 0; i < 4; ++i)
            ss << receiveBuffer[i];
        ss >> code;
        if (code == REQ_FLAG)
            return ACTION_REQUEST;
        else if (code == CLOSE_FLAG)
            return ACTION_CLOSE;
        else 
            return NOT_ACTION;
    }

    void sendImg() {
        cv::Mat frame;
        while(true)
        if (running) {
            cap >> frame;
            std::vector<uchar> encoded;
            cv::imencode(".jpg", frame, encoded, compressionParams);
            int packCounts = 1 + (encoded.size()-1) / PACK_SIZE;
            sendBuffer[0] = packCounts;
            using namespace std;
            socket.send_to(boost::asio::buffer(sendBuffer, sizeof(int)), remotePort);
            for (int i = 0; i < packCounts; ++i) {
                std::vector<uchar> sendVec;
                if (i == packCounts-1){
                    sendVec.insert(sendVec.begin(), 
                        encoded.begin() + i*PACK_SIZE, 
                        encoded.end());
                } else {
                    sendVec.insert(sendVec.begin(), 
                        encoded.begin() + i*PACK_SIZE, 
                        encoded.begin() + (i+1)*PACK_SIZE);
                }
                socket.send_to(boost::asio::buffer(sendVec, PACK_SIZE), remotePort);
            }
            cv::waitKey(1);
        }
    }

    void handleReceive(const boost::system::error_code error, 
        std::size_t size) {
        if (!error && size != 0) {
            start();
            using namespace std;
            enum ACTION action = getAction();
            if (action == ACTION_REQUEST) {
                running = true;
                cout << "[" << remotePort.address() 
                    << ":" << remotePort.port() << "]" 
                    << " Stream Open" << endl;
            } else if (action == ACTION_CLOSE) {
                cout << "[" << remotePort.address() 
                    << ":" << remotePort.port() << "]" 
                    << " Stream Close" << endl;
            }
        }
    }

    void handleSender(boost::system::error_code& error,
        std::size_t size) {

    }

public:
    AsyncVideoSender(boost::asio::io_service& ioService, int videoId = 0)
        :io(ioService),
        socket(ioService, localPort = udp::endpoint(udp::v4(), SERVER_PORT)),
        VIDEO_ID(videoId) {
        cap.open(VIDEO_ID);
        compressionParams.push_back(CV_IMWRITE_JPEG_QUALITY);
        compressionParams.push_back(ENCODE_QUALITY);
        running = false;
        imgStreamThread = boost::thread(boost::bind(&AsyncVideoSender::sendImg, this));
    }

    void setFrameSize(int width, int height) {
        cap.release();
        cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
        cap.open(VIDEO_ID);
    }

    void start() {
        boost::system::error_code ignored_error;
        socket.async_receive_from(
            boost::asio::buffer(receiveBuffer, 128), 
            remotePort,
            boost::bind(&AsyncVideoSender::handleReceive, 
                this, 
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
                )
            );
    }

};

#endif
