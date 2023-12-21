# Bluetooth-temperature-controller
### *Hello friends, I would like to share a small bluetooth temperature controller project. It is conceived and implemented in the form in which it is presented to your attention and does not pretend to be used on an industrial scale. Although everything is possible. It will help me cook: control the temperature in the oven. No doubt! I'm sure this will be useful, so I'll just leave it here.*
<img src=https://github.com/artemned/Bluetooth-temperature-controller/blob/main/Image/Additional.jpg>

### *The device consists of a control board, a Bluetooth HC-05 module, a temperature sensor and a plastic case, the parts of which are stored in the “3D_bodies” directory and are already prepared for printing.The control program code is located in the “AVR_ATtiny25_Code” directory, and the files for making the board are in the “Gerber-Files” and “Description” directories.*
<img src=https://github.com/artemned/Bluetooth-temperature-controller/blob/main/Image/Base.jpg>

### *The main function of the control board is interaction with the temperature sensor: receiving data with its subsequent processing. Next, the prepared data in a convenient format is sent via “UART” to the Bluetooth HC-05 module. The Bluetooth HC-05 module receives data from the board and transmits it to the user application installed on the smartphone. In addition the control board functions as a charging driver for the lithium-ion battery, which is necessary for the operation of the entire device.*
<img src=https://github.com/artemned/Bluetooth-temperature-controller/blob/main/Image/Base-Additional.jpg>

 ### *The entire project is covered by a free license. If you have suggestions on how to improve it, contact me via email and we will definitely think together about how this can be done better. Sound criticism is always welcome.*
 ### *P.S. Everyone goodness and fresh cookies!*
 
 ### *License: [Free license](https://github.com/artemned/Bluetooth-temperature-controller/blob/main/LICENSE)*
 ### *Email address for feedback: daineko.a32@mail.ru*
