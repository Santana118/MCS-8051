$crystal = 11059200
$baud = 9600
Config Lcdpin = Pin , Db4 = P1.1 , Db5 = P1.2 , Db6 = P1.3 , Db7 = P1.4 , E = P1.5 , Rs = P1.6
Config Lcd = 16 * 2

Dim A As Byte
A = 0
Do
Cls
Lcd A
A = A + 1
Lowerline
Lcd "Giri Atma Santana"
Waitms 100
Loop Until A = 101
End