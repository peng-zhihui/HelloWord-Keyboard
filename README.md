# HelloWord-Smart Keyboard

![hw1](5.Docs/2.Images/hw1.jpg)

> `瀚文` 智能键盘是一把我为自己使用需求设计的 **多功能**、**模块化** 机械键盘。
>
> 键盘使用模块化设计，左侧的**多功能场景交互模块**可以替换成各种自定义组件，默认使用的是一个带电子墨水屏以及FOC力反馈旋钮的`Dynamic组件`；键盘使用我自己开发的基于ARM Cortex-M芯片的键盘固件以及模块固件；键盘本体使用移位寄存器方式实现按键扫描电路；模块以及键盘本体可以单独使用，也可以通过串口协议进行相互通信和调用。
>
> **本仓库开源的资料内容包括：**
>
> * 瀚文本体硬件设计的10块PCB设计源文件，提供立创EDA专业版的文件格式
> * 外壳设计结构文件
> * 键盘本体的固件源码（已相对完善）
> * Dynamic组件的固件源码（已完成框架，更多APP扩展WIP）
> * 键盘二次开发SDK（开发中）
>
> **键盘功能演示可以参考：**
>
> * [【自制】我做了一把 模 块 化 机 械 键 盘 !【软核】_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV19V4y1J7Hx)
>
> * [I Made A Customized Modular Keyboard ! - YouTube](https://www.youtube.com/watch?v=mGShD9ZER1c)

**注意：Issues里面是讨论项目开发相关话题的，不要在里面发无意义的消息，不然watch了仓库的人都会收到通知邮件会给别人造成困扰的！！！灌水可以在仓库的Discuss里讨论！**

---

## 1. 项目说明

### 1.0 更新说明：

**22.7.31更新：**

* 添加键盘硬件所有设计原理图和PCB文件
* 添加键盘固件源代码
* 添加Dynamic组件源代码

### 1.1 项目文件说明：

#### 1.1.1 Hardware

Hardware文件夹内是瀚文键盘里面用到的所有电路的原理图和PCB文件，目前提供的是**立创EDA专业版**格式的源文件以及Gerber格式的光绘文件用于提供给厂家进行直接加工。

一共有如下几块板子：

- 

#### 1.1.2 Firmware

Firmware中提供了上面所有板子的固件源码，主要包含以下两个工程：



#### 1.1.3 Software

Software中提供了视频里演示的上位机软件ElectronStudio的Unity工程源码，以及SDK库和SDK源文件，库的层级关系由下至上分别是：



> SDK的使用方法见后文。

此外文件夹里还包含了BotDriver也就是ElectronBot连接电脑需要安装的USB驱动文件，安装方法是在设备管理器里右键更新驱动，然后选择自定义目录导航到这个文件夹即可安装。

> 安装过程需要禁用Windows的强制驱动签名（没钱给M$买签名），禁用方法根据你的操作系统版本有不同，可以百度一下。

#### 1.1.4 CAD-Model

文件夹中包含了ElectronBot的结构设计图纸，`.step`通用格式可以在所有软件里打开编辑；同时为了满足有的同学想要Fusion360里面的原始工程，我也共享了`.f3d`格式源文件，源文件里包含了完整的编辑建模时间轴。



#### 1.1.5 Docs

相关的参考文件，包括芯片的Datasheet等。

此目录下还有个_LargeFiles文件夹，是软件过程项目中依赖的一些库（比如OpenCV）因为DLL比较大超过了100MB，GitHub需要开启LFS才能提交，而我不想开启LFS，因此单独拿出来压缩了提交到仓库，编译工程的时候需要把这些库放到对应工程目录下，具体的路径见 `_path.txt`。





## 2. 硬件设计

## 3. 固件设计

## 4. SDK设计 & 二次开发



> 感谢以下项目：
>
> [opencv/opencv: Open Source Computer Vision Library (github.com)](https://github.com/opencv/opencv)
>
> [Lexikos/AutoHotkey_L: AutoHotkey - macro-creation and automation-oriented scripting utility for Windows. (github.com)](https://github.com/Lexikos/AutoHotkey_L)
>
> [olikraus/u8g2: U8glib library for monochrome displays, version 2 (github.com)](https://github.com/olikraus/u8g2)
>
> [simplefoc/Arduino FOC for BLDC  (github.com)](https://github.com/simplefoc/Arduino-FOC)

