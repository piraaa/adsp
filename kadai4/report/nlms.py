#
# nlms.py
# Created by pira on 2018/01/19.
#

#coding: utf-8

# 手計算の結果をグラフにプロットするよ

import numpy as np
import matplotlib.pyplot as plt
import math

x = [0.0,1.8,2.1]
y = [0.0,3.5,3.9]

plt.plot(x,y,marker='o')
plt.plot(3,3,marker='o') #wn
plt.xlim(0,4,1)
plt.ylim(0,4,1)
plt.xticks([0,1,2,3,4])
plt.yticks([0,1,2,3,4])
plt.xlabel('x')
plt.ylabel('y')
plt.show()