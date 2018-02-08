//
// Created by itsl on 18-2-2.
//

#include <iostream>
#include <string>
#include <sstream>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/system/system_error.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define PORT            7200
#define SERVER          "192.168.1.107"
#define FRAME_WIDTH     400
#define FRAME_HEIGHT    300
#define FRAME_SIZE      (FRAME_WIDTH*FRAME_HEIGHT*3)

using boost::asio::ip::tcp;
using namespace std;

int main() {
    cv::namedWindow("picture", CV_WINDOW_AUTOSIZE);
    try {
        boost::asio::io_service io;
        tcp::resolver resolvler(io);
        boost::asio::ip::tcp::endpoint endpoint(
                boost::asio::ip::address::from_string(SERVER),
                PORT);
        tcp::socket socket(io);
        socket.connect(endpoint);
        int totalGet = 0;
        boost::array<uchar, 360000> buf;
        boost::system::error_code error;
        cv::Mat img;
        std::vector<uchar> vector_data;
        for (;;) {
            size_t len = socket.read_some(boost::asio::buffer(buf), error);
            std::vector<uchar> newAppend(buf.begin(), buf.begin()+len);
            vector_data.insert(vector_data.end(), newAppend.begin(), newAppend.end());
            if (vector_data.size() >= FRAME_SIZE) {
                vector<uchar> shotVec;
                shotVec.insert(shotVec.end(), 
                    vector_data.begin(), 
                    vector_data.begin() + FRAME_SIZE);
                cv::Mat data_mat(shotVec, true);
                img= data_mat.reshape(3, 300);
                cv::resize(img, img, cv::Size(800, 600));
                cv::imshow("picture", img);
                vector_data.erase(vector_data.begin(), 
                    vector_data.begin() + FRAME_SIZE);
            }
            if (cv::waitKey(1) == 'q') {
                break;
            }
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
