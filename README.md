
# Cortex M3

[![sites](docs/mcuyun.png)](http://www.mcuyun.com)

Cortex-M3是ARM 32位内核，采用了哈佛结构，拥有独立的指令总线和数据总线

Cortex M3采用了Tail-Chaining中断技术，完全基于硬件进行中断处理，Cortex-M3首次在内核上集成了嵌套向量中断控制器（NVIC）。Cortex-M3的中断延迟只有12个时钟周期(ARM7需要24-42个周期)，在实际应用中可减少70%中断

Cortex-M3还使用尾链技术，使得背靠背（back-to-back）中断的响应只需要6个时钟周期(ARM7需要大于30个周期)



---


---

## 说明

本文档用于索引Cortex M3工程

