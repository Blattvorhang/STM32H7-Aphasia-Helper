# STM32H7 Aphasia Helper
失语症患者辅助设备，全国大学生嵌入式芯片与系统设计竞赛，嵌入式人工智能赛道。

## Table of Contents
- [Features](#features)
- [Product](#product)
- [X-Cube-AI](#x-cube-ai)
- [Hardware](#hardware)
  - [Connection](#connection)
- [PCB](#pcb)
- [3D Model](#3d-model)
- [Pins Configuration](#pins-configuration)
- [UI](#ui)
- [Project Configuration](#project-configuration)
- [Encoding](#encoding)
- [References](#references)

## Features
屏幕上有三个选项，通过左右倾斜设备，陀螺仪会读取角度，切换不同的带选项。患者通过拂过（遮挡后松开）红外传感器，进行确认选项。相比手机等设备的点触，不容易选错选项，对于难以精准控制自己肢体动作的患者更加友好，操作的鲁棒性更强。并且本身握持的设备，进行倾斜设备和手指拂动的操作，更加简便。

进行确认选项后，设备震动给予患者“选择成功”的反馈，利于患者适应操作流程。

选择“吃饭/喝水、上厕所”选项时，会播放特定的音乐呼叫家属，屏幕界面有对应显示。家属或患者可以再次轻拂过红外传感器，关闭音乐。

而选择“找东西”选项时，设备调用设备底部的摄像头，用户抬起设备像“望远镜”一样找东西，屏幕显示画面与内置的CUBE-AI物体识别结果，辅助患者寻找辨别东西。找到目标时，同样可以呼叫家属。

交互简易，易于患者与家属的理解，可大大促进双方的交流，免于说话交流的不便。

## Product
![](./imgs/product.png)

## X-Cube-AI
![](./imgs/AI.png)

## Hardware
![](./imgs/peripherals.png)
- STM32H7A3ZI-Q (MCU)
- OV7670 (Camera): 320x240, 30fps, RGB565
- ST7735 (2.8" TFT LCD): 240x320, RGB565
- ATK-MPU6050 (Gyroscope)
- LD3320 (Voice Recognition) (Not used)
- Passive Buzzer
- Infra-red Sensor
- Vibration Motor
- Lithium Battery (5V, Micro USB)

### Connection
![](./imgs/connection.png)

## PCB
![](./imgs/schematic.png)
![](./imgs/PCB.svg)

## 3D Model
![](./imgs/3d_modeling.png)

## Pins Configuration
![](./imgs/pins.png)

## UI
- Home 

![](./imgs/UI/choose_eat.jpg)

- Calling

![](./imgs/UI/call_eat.jpg)

## Project Configuration
- Target: Use MicroLIB
- Debug: ST-Link Debugger

## Encoding
- GB2312

## References
- [Hardware SPI Register](https://www.armbbs.cn/forum.php?mod=viewthread&tid=117991)
- [RGB565 to RGB888](https://stackoverflow.com/questions/2442576/how-does-one-convert-16-bit-rgb565-to-24-bit-rgb888)
- [X-Cube-AI MCUNet](https://github.com/mit-han-lab/mcunet)
