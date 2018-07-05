Loker AMV

Flowchart

![Flowchart](flowchart_loker-amv.jpg)

kodingan terdiri 2 bagian
- mainboard, untuk arduino mega
- secondboard, untuk arduino nano

kodingan loker-amv ga dipake

list pin

- vcc semua ditaroh di 5v

- 3.3v nya rfid taro di 3.3v

- gnd semua ditaroh di gnd

- rtc
	- D di SDA (20)
	- C di SCL (21)

- lcd
	- SDA di SDA (20)
	- SCL di SCL (21)

- mfrc 522
	- SDA di 10
	- SCK di 52
	- MOSI di 51
	- MISO di 50
	- RST di 9

- sd card adapter (pinnya taroh di arduino nano)
	- MOSO 11
	- MISO 12
	- SCK 13
	- CS 4
	
- relay di 23

- buzzer di 21
	
- tx1 (mega) sambung ke rx0 (nano), rx1 (mega) sambung ke tx1 (nano)

- gnd nano sambung ke gnd mega

