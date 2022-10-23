$crystal = 11059200
$baud = 9600
$large
Config Sda = P2.0 , Scl = P2.1
Config Lcdpin = Pin , Db4 = P1.0 , Db5 = P1.1 , Db6 = P1.2 , Db7 = P1.3 , E = P1.4 , Rs = P1.5
Config Lcd = 16 * 2
Const Adresw = &H3C
Const Adresr = &H3D

Dim Raw_x As Integer , Raw_y As Integer
Dim Sp_x As Integer , Sp_y As Integer
Dim Muter As Const 0

Dim Angle As Integer
Dim Degree As Single
Dim Xnew As Integer , Ynew As Integer
Dim Eror As Integer


Raw_x = 0
Raw_y = 0

   Cls
   I2cstart
   I2cwbyte Adresw
   I2cwbyte 1
   I2cwbyte &HE0
   I2cwbyte 1
   I2cstop
   Cls
   I2cstart
   I2cwbyte Adresw
   I2cwbyte 3
   I2cstart
   I2cwbyte Adresr
   I2crbyte Xnew , 8
   I2crbyte Raw_x , 9
   I2cstop
   Shift Xnew , Left , 8
   Sp_x = Raw_x Or Xnew


Do
   Cls
   I2cstart
   I2cwbyte Adresw
   I2cwbyte 1
   I2cwbyte &HE0
   I2cwbyte 1
   I2cstop
   Cls
   I2cstart
   I2cwbyte Adresw
   I2cwbyte 3
   I2cstart
   I2cwbyte Adresr
   I2crbyte Xnew , 8
   I2crbyte Raw_x , 9
   I2cstop
   Shift Xnew , Left , 8
   Raw_x = Raw_x Or Xnew



   Cls
   I2cstart
   I2cwbyte Adresw
   I2cwbyte 2
   I2cwbyte 1
   I2cstop
   Cls
   I2cstart
   I2cwbyte Adresw
   I2cwbyte 7
   I2cstart
   I2cwbyte Adresr
   I2crbyte Ynew , 8
   I2crbyte Raw_y , 9
   I2cstop
   Cls
   Shift Ynew , Left , 8
   Raw_y = Raw_y Or Ynew

   'add bias for calibration
   'Raw_x = Raw_x - 30
   'Raw_y = Raw_y + 30

   Upperline
   Lcd "X="
   Lcd Raw_x
   Lcd " Y="
   Lcd Raw_y
   Lcd " "

   'Exception Rules'
   If Raw_x = 0 Then
      If Raw_y > 0 Then
         Angle = 90
         Goto Jump_here
      Else
         Angle = 270
         Goto Jump_here

      End If
   Elseif Raw_y = 0 Then
      If Raw_x > 0 Then
         Angle = 0
         Goto Jump_here

      Else
         Angle = 180
         Goto Jump_here

         End If
   End If




   Xnew = Abs(raw_x)
   Ynew = Abs(raw_y)
   Degree = Raw_y \ Raw_x




   'Resolusi tan per 10 derajat'
   If Degree =< 0.18 And Degree > 0 Then
      Angle = 10
   Elseif Degree =< 0.37 And Degree > 0.18 Then
      Angle = 20
   Elseif Degree =< 0.58 And Degree > 0.37 Then
      Angle = 30
   Elseif Degree =< 0.84 And Degree > 0.58 Then
      Angle = 40
   Elseif Degree =< 1.2 And Degree > 0.84 Then
      Angle = 50
   Elseif Degree =< 1.74 And Degree > 1.2 Then
      Angle = 60
   Elseif Degree =< 2.8 And Degree > 1.74 Then
      Angle = 70
   Elseif Degree =< 5.7 And Degree > 2.8 Then
      Angle = 80
   Elseif Degree =< 11.5 And Degree > 5.7 Then
      Angle = 90
   End If

   'kuadrant check'
   If Raw_x > 0 Then
      If Raw_y > 0 Then                                       'kuadrant I
         Angle = 0 + Angle
      Elseif Raw_y < 0 Then                                   'kuadrant IV'
         Angle = 360 - Angle                                  '360
      End If
   Elseif Raw_x < 0 Then
      If Raw_y > 0 Then                                       'kuadrant II'
         Angle = 180 - Angle                                  '180
      Elseif Raw_y < 0 Then                                   'kuadrant III'
         Angle = 0 + Angle
      End If
   End If
Jump_here:

   Lowerline
   Lcd "Eror "
   Eror = Angle - 40
   Lcd Eror
   Lcd " "
   Lcd Angle
   Angle = 0


   Wait 1
   Cls
   Home Upperline
Loop Until Muter = 1


End