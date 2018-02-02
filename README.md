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

3.客户端
> 从上到下优先，能实现到哪就到哪：
* Linux ( gtk+3.* )
* Android
* Windows ( gtk+3.* )

## 一些基本约定与 Code Style
1.约定
* 网络框架统一使用 <b>Asio.Boost 1.58.0</b> (Docs: http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio.html Tutorial: http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial.html)
* 图像处理也依方便起见直接上 OpenCV 2.*
* Python 使用 Python 3.6.* 版本
* 为了模块的可拓展性和编写简便起见，本项目不使用 C 代码，全部使用 C++ 实现。务必将 classes 设计合理。
* 客户端 GUI 使用 gtk+3.* 实现，为了至少兼容 Windows 和 Linux，务必不要依赖系统专有库
* 视频统一使用 4:3 宽高比进行传输，非此比例的摄像头数据需进行裁剪，网络视频分辨率不高于720P（都是为了简单方便）
* 善用 namespace ，所有的类和常量都在父命名空间 ltrov 下，子命名空间暂且确定如下:

| namespace       | 用途 |
| :------------   | :----- |
| ltrov           | 根命名空间 |
| ltrov::control  | 电机的控制 |
| ltrow::network  | 潜水器的通信 |
| ltrow::sensor   | 传感器的数据获取及其数据解算 |
> 三级及更深层次命名空间的确定可根据情况自行命名，但为了本项目尽量简单，尽量不要定义四级或以上层次的命名空间。namespace 命名规则参照: 
http://google.github.io/styleguide/cppguide.html#Namespace_Names

2.Code Style
1. For Cpp
* Nameing 
1. 文件名内使用下划线连接，使用小写，比如: my_useful_class.cpp
2. class/enum/struct/typedef 使用首字母大写的驼峰命名法
3. 成员、方法使用首字母小写的驼峰命名法
```
// file: /network/network.hpp
namespace ltrov {
namespace network {

class NetworkTester { 
    static unsigned int connectedCount;

    bool isReady();
};

}  // namespace network
}  // namespace NetworkTester
```

* Header File
> 注意：一个独立模块的实现请严格分文件编写，并且严格将头文件声明和实现分离，比如 network 主模块的实现写在 network.cpp ，声明写在 network.hpp
```
#ifndef NETWORK_H_
#define NETWORK_H_

// ...

#endif  // NETWORK_H_
```
* Order of Includes
1. 当前实现对应的头文件 (optional)
2. C 标准库
3. C++ 标准库
4. 依赖的第三方项目的头文件
5. 本项目的头文件
```
/* example */
// 当前实现对应的头文件
#include "lightrov/control/control.hpp"
// C 标准库
#include <sys/types.h>
#include <unistd.h>
// C++ 标准库
#include <vector>
#include <hash_map>
// 依赖的第三方项目的头文件
#include <boost/asio.hpp>
// 本项目内的头文件
#include "lightrov/network/network.hpp"
```
* namespace
```
// 基本规范
// in .hpp file
namespace mynamespace {

// 所有的声明都在 namespace 的代码块内部
// 并且不需要缩进
class MyClass {
public:
    ...
    void Foo();
};

}  // namespace mynamespace
// 注意添加 namespace 的结束处注释，增加可读性


// in .cpp file
namespace mynamespace {

void MyClass::Foo() {
  ...
}

}  // namespace mynamespace
```