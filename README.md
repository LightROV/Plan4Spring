# Plan4Spring
The plan for spring festival term.
## TODO List
针对 LightROV ，至少有以下几个模块需要实现：

1.协议层
* UDP/TCP 异步通信及必要的传输认证
* 视频/音频流的转码、传输、解码
* 控制指令的确定（包括但不限于：转向、推进、升降）

2.嵌入式端
* 基础控制实现方案的确立
* 传感器的调试与数据滤波及目标数据的解算

## 一些基本约定与 Code Style
1.约定
* 网络框架统一使用 <b>Asio.Boost 1.66.0</b> (Docs: http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio.html Tutorial: http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial.html)
* 视频统一使用 4:3 宽高比进行传输，非此比例的摄像头数据需进行裁剪，网络视频分辨率不高于720P（都是为了简单方便）
* 图像处理也依方便起见直接上 OpenCV 2.*
* Python 使用 Python 3.6.* 版本
* 为了模块的可拓展性和编写简便起见，本项目不使用 C 代码，全部使用 C++ 实现。务必将 classes 设计合理。
* 