# 3d-mouse
Repository for the code and files for the Spacemouse project

This spacemouse is a self-contained project. You are free to copy, modify, or add to it as you wish, as long as you credit its authors.

This project builds on [this](https://www.printables.com/model/864950-open-source-spacemouse-space-mushroom-remix) iteration of a 3D CAD mouse, by Teaching Tech. What we've done is created a new, smaller design, based on [these smaller joysticks](https://es.aliexpress.com/item/1005007021831547.html?spm=a2g0o.productlist.main.2.1bdd2358LI6Vo9&algo_pvid=52eb9f8d-11a8-4804-94bb-e7eaf73d970d&algo_exp_id=52eb9f8d-11a8-4804-94bb-e7eaf73d970d-1&pdp_ext_f=%7B%22order%22%3A%2268%22%2C%22spu_best_type%22%3A%22price%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%214.69%214.69%21%21%2137.91%2137.91%21%40210385db17699847441764245e9248%2112000049800936130%21sea%21ES%214293090942%21X%211%210%21n_tag%3A-29919%3Bd%3A382ce27%3Bm03_new_user%3A-29895&curPageLogUid=GalQMk3aBXTC&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005007021831547%7C_p_origin_prod%3A) from the oculus controller. To interface with these joysticks, a PCB has been designed as well, that allows easier connection management, and the .grbr files are provided.

# Bill of Materials:
For each device that is built, the following are needed:
- Arduino Pro Micro x1
- Spacemouse PCB x1
- 3D printed parts
- 0.5 mm pitch, 5-pin ZIF connector x4
- Oculus quest joystick x4
- Wire ~30 cm
- 6x6x4.3 mm pushbutton (1 to 4, user choice)
- 2-pin magnetic pogo connectors (as many as pushbuttons)
- USB cable (Arduino-Computer) Check that USB ports match before buying

# Build Instructions:
This section is still WIP. Feedback is welcome.

# Software Instructions:
## Installing the 3D Connexion Software
Go to their [software download page](https://3dconnexion.com/us/drivers/) and download the appropriate software for your computer.

## Flashing the Arduino Code
1. Create a new Arduino AVR board:  
2. Find your sketchbook folder by checking in the Arduino IDE File/Preferences, where your sketchbook folder is. It typically is C:/Users/YOUR_USER/Documents/Arduino  
3. Create a new folder named `hardware`. Copy the spacemouse folder from this repo inside `hardware`.
4. Restart the Arduino IDE
5. Plug in you Arduino Pro Micro
6. In the Arduino IDE go to Tools/Board and select `Arduino AVR Boards (in sketchbook) (spacemouse)/spacemouse`
7. Flash the code in `spacemouse-code`
---
And that should get you a fully working spacemouse!
