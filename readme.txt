# PY32F002B TIM Fan Control Example

## 项目概述 (Project Overview)
本例程演示了如何使用 PY32F002B 微控制器生成 PWM 信号，并通过读取连接到 ADC 的电位器电压实时调整其中一路 PWM (TIM1_CH4) 的占空比，用于风扇转速控制或其他需要动态 PWM 输出的应用。

This example demonstrates how to use the PY32F002B microcontroller to generate PWM signals, and dynamically adjust the duty cycle of one PWM channel (TIM1_CH4) in real-time by reading the voltage from a connected potentiometer via ADC, for fan speed control or other applications requiring dynamic PWM output.

## 功能描述 (Function Description)
本例程配置 TIM1 输出 PWM 信号。其中，TIM1_CH4 (PA7) 的占空比会根据连接到 ADC_CHANNEL_1 (PA1) 的电位器电压进行动态调整。
PWM 周期为 25KHz (计算方式：24000000 / 96 / 10 = 25KHz)。

This routine configures TIM1 to output PWM signals. Specifically, the duty cycle of TIM1_CH4 (PA7) is dynamically adjusted based on the voltage from a potentiometer connected to ADC_CHANNEL_1 (PA1).
The PWM cycle is 10Hz (calculated as: 计算方式：24000000 / 96 / 10 = 25KHz).

## 注意事项 (Notes)
1.  **引脚分配 (Pin Assignment)**:
    - PA7 ------> TIM1_CH4 (PWM output, duty cycle controlled by ADC)
    - PA1 ------> ADC_CHANNEL_1 (ADC 输入，通常连接电位器用于 PWM 占空比控制)

## Keil MDK 项目结构 (Keil MDK Project Structure)
项目文件位于 `MDK-ARM/` 目录下。
打开 `MDK-ARM/Project.uvprojx` 即可在 Keil MDK 中加载项目。

Project files are located in the `MDK-ARM/` directory.
Open `MDK-ARM/Project.uvprojx` to load the project in Keil MDK.