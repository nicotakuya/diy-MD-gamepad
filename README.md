# DIY MD Gamepad

## 概要
AVRマイコンで作るメガドライブ用のゲームパッドです。
6ボタンに対応しています。

## Files
Atmel Studio 7.0用プロジェクトファイルです。

main.c : ソースです。

mdpad_schematics.png : 回路図。

mdpad_timing.png : タイミングチャート。

## Parts list
 U1：AVRマイコン ATmega168/328P

 X1：発振子16MHz
 
 SW1-4：レバー	 	 
 
 SW5-12：ボタン	  
 
 CN1：Dsub9pin	 	 
 
 CN2：2x3ピンヘッダ	 	 
 
 CN3：2x8ピンソケット	  
 
 CN4：2x8ピンヘッダ	 	 
 
 C1：コンデンサ 0.1μF
 
 C2：コンデンサ 100μF	 	 
 
 R1：抵抗 10k ohm
