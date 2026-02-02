# 3d-mouse
Repository for the code and files for the Spacemouse project

This spacemouse is a self-contained project. You are free to copy, modify, or add to it as you wish, as long as you credit its authors.

This project builds on [this](https://www.printables.com/model/864950-open-source-spacemouse-space-mushroom-remix) iteration of a 3D CAD mouse, by Teaching Tech. What we've done is created a new, smaller design, based on [these smaller joysticks](https://es.aliexpress.com/item/1005007021831547.html?spm=a2g0o.productlist.main.2.1bdd2358LI6Vo9&algo_pvid=52eb9f8d-11a8-4804-94bb-e7eaf73d970d&algo_exp_id=52eb9f8d-11a8-4804-94bb-e7eaf73d970d-1&pdp_ext_f=%7B%22order%22%3A%2268%22%2C%22spu_best_type%22%3A%22price%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%214.69%214.69%21%21%2137.91%2137.91%21%40210385db17699847441764245e9248%2112000049800936130%21sea%21ES%214293090942%21X%211%210%21n_tag%3A-29919%3Bd%3A382ce27%3Bm03_new_user%3A-29895&curPageLogUid=GalQMk3aBXTC&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005007021831547%7C_p_origin_prod%3A) from the oculus controller. To interface with these joysticks, a PCB has been designed as well, that allows easier connection management, and the .grbr files are provided.

## Bill of Materials:
For each device that is built, the needed materials are:

 | Nº | Element | Quantity |
 | ---| ------- | -------- |
 | 1  | Arduino Pro Micro | 1 |
 | 2  | Spacemouse PCB | 1 |
 | 3  | 3D printed parts | All |
 | 4  | 0.5 mm pitch, 5-pin ZIF connector | 4 |
 | 5  | Oculus Quest Joystick | 4 |
 | 6  | Wire | ~30 cm |
 | 7  | 6x6x4.3 mm pushbutton | 1 to 4 (user choice) |
 | 8  | 2-pin magnetic pogo connector| as many as pusbuttons |
 | 9  | USB cable (Arduino-Computer) | 1 |

## Build Instructions:
This section is still WIP. If possible, pictures will be added. Feedback is welcome.

### PCB & Electronics Assembly:
1.  Start by soldering all ZIF connectors to the PCB. The best results are obtained by using a lot of soldering flux and tin, and then removing the excess with a wick.  
  It is important to do this step first, because later on access will be lost to the nº 1 connector after soldering the Arduino.  
  It is recommended to test the soldering of the connectors with a multimeter in continuity mode.
2. Solder the arduino so that it is on the same side as the connectors, and the USB port is facing outside the PCB. 
3. Solder 3cm wires to the pogo-pin connectors.
4. Solder 4cm wires to the 6x6 mm pushbuttons.

### Mouse Assembly:
1. Mount the balljoint pieces on the joysticks. Be careful not to damage their cables.
2. Connect the joysticks to the PCB, so that the joysticks are facing the side opposite the Arduino.
3. (OPTIONAL) Add some small weights to the 3D printed bottom part. 
4. Carefully mount the PCB and the joysticks to the 3D printed bottom part. 
5. Mount the pogo-pin connectors to the 3D printed bottom part
6. Solder the connector cables to the PCB.

7. Put in the 3D printed top part the heat inserts for the buttons that you wish to use. 
8. Mount the pushbuttons to the 3D printed top part, by using the 3D printed button holder pieces.
9. Solder the pogo-pin connectors to the pushbutton cables
10. Mount the pogo-pin connectos to the 3D printed top part.

12. Mount the knob assembly (...)
13. Carefully lift the PCB from the bottom part, and mount the knob assembly.

14. Place the 3D printed top part on top of the 3D printed bottom part.


## Software Instructions:
### Installing the 3D Connexion Software
Go to 3D Connexion's [software download page](https://3dconnexion.com/us/drivers/) and download the appropriate software for your computer.

### Flashing the Arduino Code
1. Create a new Arduino AVR board:  
2. Find your sketchbook folder by checking in the Arduino IDE File/Preferences, where your sketchbook folder is. It typically is `C:/Users/YOUR_USER/Documents/Arduino`.  
3. Create a new folder named `hardware`. Copy the spacemouse folder from this repo inside `hardware`.
4. Restart the Arduino IDE
5. Plug in you Arduino Pro Micro
6. In the Arduino IDE go to Tools/Board and select `Arduino AVR Boards (in sketchbook) (spacemouse)/spacemouse`
7. Flash the code in `spacemouse-code`

And that should get you a fully working spacemouse!
---

## Known Issues:

As of now, no code has been written to send the state of the pushbuttons. We still don't know how to send that state over the USB HID protocol. However, all the axis are working.

## Instructions for contributors:

1. Remember to export the 3D mouse design as a `.step` file. 
2. Always document the changes you make to the sketches.