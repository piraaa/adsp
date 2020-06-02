#coding: utf-8

import numpy as np
import matplotlib.pyplot as plt
import math

fs = 10000 #サンプリング周波数
fc = 2000  #カットオフ周波数

N = 21 #インパルス応答を計算する時の入力データ数

def main():

	#各変数の計算
	T = 1.0 / fs
	omega_d = 2.0 * math.pi * fc #ディジタルフィルタでほしいカットオフ角周波数
	omega_a = omega_c = 2.0 / T * math.tan(omega_d*T/2.0) #プリワーピングによって変換した角周波数
	fa = omega_c / (2*math.pi) #2π*fa = omega_c 単位:Hz
	alpha = omega_c * T
	a1 = (alpha-2) / (alpha+2)
	b0 = b1 = alpha / (alpha+2)
	print('omega_d =', omega_d)
	print('omega_c =', omega_c)
	print('fa =', fa)
	print('alpha =', alpha)
	print('a1 =', a1)
	print('b0 =', b0)
	print('b1 =', b1)

	#インパルスを入れた時の出力（インパルス応答）
	output = list()
	for i in np.arange(N):

		#入力データ（インパルス）を生成
		input = np.zeros(N)
		input[i] = 1

		#フィルタの計算
		x1, y1 = 0.0, 0.0
		for x0 in input:
			y0 = b0*x0 + b1*x1 - a1*y1
			x1, y1 = x0, y0
		output.append(y0)

	#インパルス応答のグラフ
	x = np.arange(0,N,1)
	y = output[::-1]
	plt.plot(x, y)
	plt.xlabel('Time [sample]')
	plt.ylabel('Impulse Response')
	plt.xlim(0,N,5)
	plt.show()

	for x in y:
		print(x)

	#振幅特性の計算
	freq = np.fft.fft(y)
	amp = [10 * math.log10(i.real**2 + i.imag**2) for i in freq]

	#振幅特性のグラフ
	x = np.arange(0,N,1)
	y = amp
	plt.plot(x, y)
	plt.xlabel('Freqency')
	plt.ylabel('Amplitude Spectrum [dB]')
	plt.xlim(0,N//2)
	plt.ylim(-40,0)
	plt.show()

if __name__ == '__main__':
	main()