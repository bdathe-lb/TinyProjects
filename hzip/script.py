#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Huffman 字节频率可视化脚本
读取标准输入，每行格式：byte : frequency
绘制柱状图显示频率分布
"""

import sys
import matplotlib.pyplot as plt  # 用于绘图

# 1. 准备存储字节值和频率的列表
bytes_list = []   # 存放字节值（0~255）
freq_list = []    # 存放对应频率

# 2. 逐行读取标准输入
for line in sys.stdin:
    line = line.strip()  # 去掉首尾空格和换行
    if not line:
        continue  # 空行跳过
    # 每行格式是 "byte : frequency"
    try:
        byte_str, freq_str = line.split(":")
        byte_val = int(byte_str.strip())   # 字节值
        freq_val = int(freq_str.strip())   # 出现次数
        bytes_list.append(byte_val)
        freq_list.append(freq_val)
    except ValueError:
        print(f"忽略无法解析的行: {line}", file=sys.stderr)
        continue

# 3. 绘制柱状图
plt.figure(figsize=(12, 6))        # 设置图像大小
plt.bar(bytes_list, freq_list)      # 绘制柱状图，x=字节值，y=频率
plt.xlabel("Byte (0-255)")         # x轴标签
plt.ylabel("Frequency")            # y轴标签
plt.title("Byte Frequency Distribution")  # 图表标题
plt.xticks(range(0, 256, 16))      # x轴每16个字节显示一次刻度
plt.grid(axis='y', linestyle='--', alpha=0.5)  # 添加y轴网格，便于观察

# 4. 显示图形
plt.tight_layout()  # 自动调整布局，避免标签重叠
plt.show()
