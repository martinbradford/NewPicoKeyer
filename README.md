RP2040 PicoKeyer

This is an initial port of the K3NG keyer code to the RP2040 processor running under the Arduino core environment.

Please note that almost all the code in this project was written by Anthony, K3NG, and his terms of use must be respected. The initial port was implemented by Martin Bradford, G8FXC, and his additional code is subject to the same terms.

The code has been developed using Microsoft Visual Studio with the Visual Micro extension. This only works with Visual Studio, not Visual Studio Code, so it is currently a MS Windows development environment only.

It may be possible to build the code using the PlatformIO environment which will load into Visual Studio Code and run under Linux. The author did try this a year or two ago and the support for the RP2040 under PlatformIO was not good at that time - it may have improved now.

The original code from Anthony was developed using the Arduino IDE. I've made changes to that code as part of the move to Visual Studio with Visual Micro and I do not believe that it would be possible to build this code now with the Arduino IDE. I don't think I've introduced any fundamental compatibility issues and it should be possible to move the project back into the Arduino IDE if required.

