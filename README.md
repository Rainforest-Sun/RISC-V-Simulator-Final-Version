# **RISC-V Simulator**
***
## **简介**
- SJTU rainforest
- 2021-2022学年暑期小学期，PPCA课程项目
- **使用C++模拟五级流水**
***
## **五级流水设计示意图**
![五级流水架构设计图](https://github.com/Rainforest-Sun/RISC-V-Simulator/blob/master/Pipeline%20Design.png)

***
### **缩写说明**
> 缩写方式继承于各位助教
#### 五级流水各个阶段
| **缩写** | **全称** |
| :--- | :----------------- |
| **IF**  | Instruction Fetch  |
| **ID**  | Instruction Decode |
| **EX**  | Execute            |
| **MEM** | Memory Access      |
| **WB**  | Write Back         |
#### 实现过程中涉及的名词
| **缩写** | **全称** | **说明** |
| :--- | :-------------------- | :----------------------- |
| **PC**  | Program Counter      | 待执行的下一条指令地址 |
| *IA*  | Instruction Address  | 当前指令地址 |
| **INS** | Insturction          | 指令 |
| **RS**  | Register Source      | 读取值的寄存器序号 |
| *RV*  | Register Value       | 获取的寄存器值 |
| **IMM** | Immediate            | 立即数 |
| **RD**  | Register Destination | 将写入的寄存器序号 |
| *CR*  | Calculation Result   | ALU计算结果 |
| *VAL* | Value                | 将写入寄存器的值 |
| *TA*  | Target Address       | 跳转或分支指令的目标地址 |
| *INS TYPE* | Instruction Type | 指令的类型 |

**注：粗体内容为通用缩写。*
***
## 文件结构
- global.hpp
    - `pc`、`clk`以及一些全局变量
    - `INSTRUCTION`结构体
    - `sext`符号位扩展函数
    - `get_bit`取无符号数的若干位函数
- buffer.hpp
    - 由于C++模拟五级流水时必须给5个 Stage 人为指定一个顺序，为保证任何顺序都是可行的，使用了4个 Buffer 供时钟上升沿读入数据的暂时存储
    - 4个 Buffer 供时钟下降沿数据的存储
- instruction.hpp
    - 37个指令模拟
- Memory.hpp
    - 定义了`Memory`类
    - `Memory`构造函数将16进制数据转为10进制并读入模拟的内存 mem 中
    - 从内存中 Load 和 Store 的成员函数
- Register.hpp
    - 定义了`Register`类
    - 从寄存器中 Load 和 Store 的成员函数
    - 每个时钟周期结束后让`reg[0]`保持为0
- stage.hpp
    - 定义了`StageIF`、`StageID`、`StageEX`、`StageMEM`、`StageWB`五个类
    - 五个类中分别有五个指令函数`do_IF`、`do_ID`、`do_EX`、`do_MEM`、`do_WB`以及他们的封装函数`IF`、`ID`、`EX`、`MEM`、`WB`
- forwarding.hpp
    - `forwarding1(ins)`为从 MEM/WB Buffer 向该条指令 ins 的 forwarding
        - `forwarding1_1(ins)`为只有 RV1 可能发生 forwarding 的情况
        - `forwarding1_2(ins)`为 RV1 、RV2 都可能发生 forwarding 的情况
    - `forwarding2(ins)`为从 EX/MEM Buffer 向该条指令 ins 的 forwarding
        - `forwarding2_1(ins)`为只有 RV1 可能发生 forwarding 的情况
        - `forwarding2_2(ins)`为 RV1 、RV2 都可能发生 forwarding 的情况
- predictor.hpp
    - `succ`预测成功次数、`tot`预测总次数、`bht` Branch History Table、`pht` Pattern History Table、`btb` Branch Target Buffer
    - `init_predictor`初始化 BHT 、PHT 以及 BTB
    - `predictPC`根据 branch 指令的 PC 提供预测，并跳转 PC
    - `update`用预测结果更新 BHT、PHT、BTB以及预测成功率
- cpu.hpp
    - 定义了`CPU`类
    - 时钟上升沿时的同步读入函数`syn()`
    - `run_order()`函数按周期模拟 CPU 运行
    - 使用了`randon_shuffle()`函数，每个时钟周期内，5个 Stage 运行的顺序都是随机的，模拟硬件并行
- main.cpp
    - `main`函数
***
## forwarding
### 共实现了三种forwarding
- EX/MEM Buffer-->EX
- MEM/WB Buffer-->EX
- MEM/WB Buffer-->ID
***
## 分支预测
- 二级自适应分支预测
    - 在`IF`阶段进行分支预测，以`PC`作为预测依据
    - 通过大小为`256`的`BHT`获取分支历史记录
    - 通过大小为`256×16`的`PHT`预测是否跳转
    - 通过大小为`256`的`BTB`获取预测的跳转地址
- 成功率

| **测试点** | **成功率** | **预测成功次数** | **总预测次数** | **总时钟周期** |
| :-----: | :----: | :----------: | :------: | :----: |
| **array_test1**    | 54.55% | 12 | 22 | 255 |
| **array_test2**    | 50.00% | 13 | 26 | 292 |
| **basicopt1**      | 99.19% | 153889 | 155139 | 638560 |
| **bulgarian**      | 94.59% | 67625 | 71493 | 413863 |
| **expr**           | 72.07% | 80 | 111 | 639 |
| **gcd**            | 62.50% | 75 | 120 | 713 |
| **hanoi**          | 98.28% | 17157 | 17457 | 203154 |
| **lvalue2**        | 66.67% | 4 | 6 | 70 |
| **magic**          | 82.72% | 56141 | 67869 | 600860 |
| **manyarguments**  | 80.00% | 8 | 10 | 80 |
| **multiarray**     | 53.09% | 86 | 162 | 1960 |
| **naive**          | /      | 0 | 0 | 32 |
| **pi**             | 85.35% | 34100754 | 39956380 | 126166429 |
| **qsort**          | 94.54% | 189130 | 200045 | 1545411 |
| **queens**         | 80.97% | 62439 | 77116 | 634542 |
| **statement_test** | 63.37% | 128 | 202 | 1372 |
| **superloop**      | 95.89% | 417136 | 435027 | 595754 |
| **tak**            | 78.54% | 47623 | 60639 | 1842941 |