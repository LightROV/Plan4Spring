# 控制指令以及回传指令


## 大前提
务必确定好统一的规则以区分于其他数据，比如可以对指令集使用统一的前后缀

## 控制指令
```
/* 
 * 控制指令写在下面
 * 务必注意确定统一可靠的规则
 */
#ifndef CONTROL_INSTRUCTION_SET_
#define CONTROL_INSTRUCTION_SET_

/* example */

// 请求获取视频数据
#define REQUEST_VIDEO_GET   0x6666

/* example end */

// 注意，例子中并没有考虑控制指令的唯一性，最终确定时要符合大前提

#endif
```

## 回传指令
```
/* 
 * 回传指令写在下面
 * 务必注意确定统一可靠的规则
 * 
 * 回传指令要设定标志段和数据段，分别用于标示数据类型和数据内容
 */

#ifndef RESPONSE_INSTRUCTION_SET_
#define RESPONSE_INSTRUCTION_SET_

#endif
```