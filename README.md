# 【瀚文】HelloWord-Smart Keyboard

![hw1](5.Docs/2.Images/hw1.jpg)

> `瀚文` 智能键盘是一把我为自己使用需求设计的 **多功能**、**模块化** 机械键盘。
>
> 键盘使用模块化设计，左侧的**多功能场景交互模块**可以替换成各种自定义组件，默认使用的是一个带电子墨水屏以及FOC力反馈旋钮的`Dynamic组件`；键盘使用我自己开发的基于ARM Cortex-M芯片的键盘固件以及模块固件；键盘本体使用移位寄存器方式实现优化的按键扫描电路；模块以及键盘本体可以单独使用，也可以通过串口协议进行相互通信和调用。
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

* 添加键盘硬件所有设计原理图文件（电路还有些bug没修复，如视频里的飞线，会晚些等新版PCB打样收到验证没问题后更新）
* 添加键盘固件源代码
* 添加Dynamic组件源代码

### 1.1 项目文件说明：

#### 1.1.1 Hardware

Hardware文件夹内是瀚文键盘里面用到的所有电路的原理图和PCB文件，目前提供的是[立创EDA专业版](https://oshwhub.com/pengzhihui/helloword-smart-keyboard)格式的源文件以及Gerber格式的光绘文件用于提供给厂家进行直接加工。

一共有如下几块板子：

- **HelloWord-Keyboard**：主键盘的PCB，控制器为STM32F103，可以配合底座单独使用，提供常规按键输入功能，带全按键独立RGB灯。
- **HelloWord-Ctrl**：左侧Dynamic组件的PCB，控制器为STM32F405，可以配合底座单独使用，提供FOC力反馈旋钮、电子墨水屏显示、OLED显示、RGB灯等功能。
- **HelloWord-Connector**：主键盘用于连接底座的触点PCB，通过FFC排线与键盘PCB连接。
- **HelloWord-Connector-Ctrl**：Dynamic组件用于连接底座的触点PCB，通过FFC排线与Dynamic组件PCB连接。
- **HelloWord-Encoder**：磁编码器PCB，用于对无刷电机进行位置反馈，需要配合一个径向充磁的永磁铁工作。
- **HelloWord-Hub1**：底座上扩展出的两个额外USB-A接口转接PCB，通过FFC排线和TypeC接口板连接。
- **HelloWord-Hub2**：底座上扩展出的两个额外USB-A接口的母座PCB，预留的是USB3.0母座和引脚，但是目前只使用到了2.0接口，未来可以升级到USB3.0的HUB。
- **HelloWord-TypeC**：底座上用于连接电脑的TypeC接口PCB，板载了电源充电管理芯片，以及USB-HUB芯片，通过FFC排线连接其余模块。
- **HelloWord-OLED**：Dynamic组件上OLED屏幕的最小驱动电路以及转接板。
- **HelloWord-TouchBar**：可选的电容触摸条模块PCB，使用6按键电容触摸芯片组成一个线性感应阵列，通过FFC排线和主键盘PCB连接。

#### 1.1.2 Firmware

Firmware中提供了上面所有板子的固件源码，以及**预编译好的bin固件**可以直接烧录，主要包含以下两个工程：

* **HelloWord-Keyboard-fw**：主键盘的固件，主要实现了基于硬件SPI和移位寄存器的高速按键扫描、基于硬件SPI&DMA的总线式RGB灯控制、HID高速设备键盘枚举&报文协议实现、非易失存储配置、多层按键映射等功能。
* **HelloWord-Dynamic-fw：**Dynamic组件的固件，主要实现了基于FOC的电机控制代码、可配置触感封装类、电子墨水屏驱动、OLED驱动、USB全速复合设备枚举和通信协议、RGB灯控制等功能。

工程都是基于STM32HAL实现的，所以提供了对应的`.ioc`文件，可以自行用STM32CubeMX打开，生成对应的keil或者STM32IDE工程文件。当让也可以像我一样，用CLion或者STM32CubeIDE进行编译和下载。

`_Release`文件夹里是预编译好的bin文件，可以用**ST-Link Utillity**或者STM32CubeProgrammer之类的软件直接下载到芯片。

关于固件的实现细节后文有讲解。

> 把CLion变成STM32的IDE的方法参考我之前发的一篇教程：[配置CLion用于STM32开发【优雅の嵌入式开发】](https://zhuanlan.zhihu.com/p/145801160) 。

#### 1.1.3 Software

Software中提供了一些用于和键盘交互的电脑端上位机软件，包括视频里演示的傻瓜化修改墨水屏图片的上位机软件，以及后续会逐步补充用于**修改键位的图形化软件**给**模块添加APP**的应用商店软件，这些还在开发中。

#### 1.1.4 Tools

Tools主要是提供一些三方的工具软件，比如**STM32 ST-LINK Utility** 、用于安装驱动的**zadig**等等。

#### 1.1.5 3D Model

文件夹里是键盘用到的所有结构件的3D模型文件，可以用于3D打印。

#### 1.1.6 Docs

相关的参考文件，包括芯片的Datasheet等。

## 2. 硬件架构说明

**关于结构设计？**

瀚文的结构包括三大部分：**扩展坞底座**、**键盘输入模块**和**可替换的多功能交互模块**，键盘输入模块和可替换的多功能交互模块通过若干接触式触点连接在所述扩展坞底座顶部：

![hw2](5.Docs/2.Images/hw2.jpg)

而键盘本体也是一个标准的客制化键盘层叠结构设计，包含减震棉、PCBA、定位板、轴下垫等：

![hw2](5.Docs/2.Images/hw3.jpg)

键盘的结构设计主要是Xikii根据S98修改而来，为75键布局，有其他布局需要的同学可以自行修改PCB和固件适配。

> 关于视频中展示的结构件，由于是Xikii的方案所以我也不好擅自把源文件放出，而且原始版本结构是用于CNC机加工的，成本会比较高。
>
> 所以我也让Xikii帮忙设计一套简化版可用于3D的结构件，并开源出来放到仓库。

**关于芯片选型？**

- 键盘主控选用的芯片是STM32F103CBT6，实际上C8T6也可以，不过考虑到未来固件功能的扩展性，Flash大一倍的CBT6性价比更高。这块由于我固件基本都是使用HAL库实现的，因此实际上也可以把主控替换成STM32系列的任何一款芯片，需要支芯片有2个SPI硬件接口分别用于按键扫描以及RGB灯驱动，以及一个全速USB接口即可。
- Dynamic组件主控的STM32F4，这个是因为我手边这款芯片比较多，理论上可以替换为性价比更高的F1系列的，只要芯片具备一个高级定时器用于PWM生成、2个硬件SPI接口用于编码器和电子墨水屏通信、一个I2C接口用于OLED驱动，以及一个全速USB接口即可。
- 电机的磁编码器芯片我是使用AS5047P，也是一款很常用性能很好的磁编码芯片，不过成本略高，我只是因为手边有所以选了这款，也是可以修改为其他更便宜的型号的比如MT6816等，当然也需要修改固件驱动代码。
- 按键扫描用的移位寄存器使用的74HC165，国产芯片零售的话大概0.5元一片，一片可以驱动8个按键，按照你自己需要的按键数目修改串联的寄存器芯片即可。进口的165比如TI原装的要比国产贵一些，性能也会稍微好点，不过由于本项目中按键扫描频率4MHz已经完全够用了，因此哪怕国产的16MHz芯片也绰绰有余了。
- 电容触摸板使用的是一个6通道电容触摸按键芯片XW06A实现的，这个对于PCB感应盘的设计有一定要求，仓库已经提供了设计好的PCB。对于该芯片的读取方式，其实和普通按键没有区别，所以本方案中也是使用74HC165进行扫描读取的。
- 电机FOC驱动电路完全是从我的Ctrl驱动器中移植过来的，使用FD8288Q作为栅极驱动器，无需电流传感器。

**关于烧录方式？**

使用JLink、STLink之类的调试器烧录，我在PCB和外壳上都预留了SWD调试口。对于没有硬件开发经验的同学，我晚点也会放出一个Bootloader，可以直接通过USB口进行固件升级。

**关于电机选型？**

我使用的是一个2204的二手电机，不过这一款电机目前好像不太好买，大家可以选取类似尺寸的无刷电机替换，参数方面需要KV值低一些，最好200左右。电机需要手动在转子上安装一个径向充磁的永磁体用于编码器定位，不同型号的电机需要对FOC参数进行一些调整。

![hw2](5.Docs/2.Images/hw4.jpg)

## 3. 软件架构说明

## 4. SDK设计 & 二次开发

待补充。



> 感谢以下项目：
>
> [Lexikos/AutoHotkey_L: AutoHotkey - macro-creation and automation-oriented scripting utility for Windows. (github.com)](https://github.com/Lexikos/AutoHotkey_L)
>
> [olikraus/u8g2: U8glib library for monochrome displays, version 2 (github.com)](https://github.com/olikraus/u8g2)
>
> [simplefoc/Arduino FOC for BLDC  (github.com)](https://github.com/simplefoc/Arduino-FOC)

