/* 
 * 一个异步视频传输接收端的实现
 *  |- 基于 boost.asio 和 OpenCV 实现
 *   - 使用 UDP
 * 
 * created by itsl<itsl@foxmail.com>
 *  2018-2-6
 */

#include <ctime>
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifndef LIGHTROV_NETWORK_ASYNCVIDEORECIEVER_HPP_
#define LIGHTROV_NETWORK_ASYNCVIDEORECIEVER_HPP_

/* 
 * 一些配置
 */
#define SENDER_HOST         "192.168.1.107"
#define SENDER_PORT         7201
#define SERVER_PORT         7200
#define COMPRESS_QUALITY    60
#define PACK_SIZE           4096

#define REQ_FLAG            "2333"
#define CLOSE_FLAG          "stop"

using boost::asio::ip::udp;

namespace ltrov {
namespace network {

class AsyncVideoReceiver
{
public:
    AsyncVideoReceiver(boost::asio::io_service& ioService)
        : io(ioService),
        socket(io, localPort = udp::endpoint(udp::v4(), SERVER_PORT)) {
        // encodedBuf = nullptr;
        curImg = cv::Mat::zeros(cv::Size(200, 100), CV_8UC1);
        startRecieve = false;
    }

    void notify() {
        socket.send_to(boost::asio::buffer(REQ_FLAG), 
            udp::endpoint(boost::asio::ip::address_v4::from_string(SENDER_HOST), 
            SENDER_PORT));
    }

    void start() {
        socket.async_receive_from(
            boost::asio::buffer(recvBuf, recvBuf.size()), 
            remotePort, 
            boost::bind(&AsyncVideoReceiver::handleReceive, 
                this, 
                boost::asio::placeholders::error, 
                boost::asio::placeholders::bytes_transferred));
    }

    void stop() {
        socket.send_to(boost::asio::buffer(CLOSE_FLAG), 
            udp::endpoint(boost::asio::ip::address_v4::from_string(SENDER_HOST), 
            SENDER_PORT));
    }

    cv::Mat getImg() {
        return curImg;
    }

protected:
    // on recieve handler
    void handleReceive(const boost::system::error_code& error, 
        std::size_t size) {
        if (!error || error != boost::asio::error::message_size) {
            /* handle data */
            std::vector<uchar> vecStr(recvBuf.begin(), recvBuf.begin()+size);
            if (size == sizeof(int)) {
                startRecieve = true;
                if (encodedVec.size() != 0) {
                    using namespace std;
                    cout << "Pack Count" << PACK_COUNT << endl;
                    uchar *longBuf = &encodedVec[0];
                    longBuf[encodedVec.size()] = 0;
                    cv::Mat tmpMat(1, PACK_COUNT * PACK_SIZE, CV_8UC1, longBuf);
                    curImg = cv::imdecode(tmpMat, CV_LOAD_IMAGE_COLOR);
                    if (curImg.size().width != 0 && curImg.size().height != 0) {
                        std::cout << "decode success" << std::endl;
                    } else {
                        std::cout << "decode fail" << std::endl;
                    }
                    encodedVec.erase(encodedVec.begin(), encodedVec.end());
                }
                PACK_COUNT = (int)vecStr[0];
            } else if(startRecieve) {
                encodedVec.insert(encodedVec.end(), vecStr.begin(), vecStr.end());
            }
        }
        start();
    }

    boost::asio::io_service& io;
    boost::array<uchar, 65540> recvBuf;
    udp::socket socket;
    // 本地端口
    udp::endpoint localPort;
    // 远程端口
    udp::endpoint remotePort;
    cv::Mat curImg;
    char* encodedBuf;
    std::vector<uchar> encodedVec;
    bool startRecieve;
    int PACK_COUNT;
};

}  // namespace network
}  // namespace ltrov

#endif