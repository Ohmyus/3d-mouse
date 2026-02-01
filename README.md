# 3d-mouse
Repository for the code and files for the Spacemouse project

# INSTRUCTIONS:

1. Find your sketchbook folder.  
To do this, open the Arduino IDE, go to File/Preferences, and take note of the sketchbook location.
2. Navigate to your sketchbook folder.  
   And create a new folder for the board that you will use, named 'spacemouse'.
3. Inside the spacemouse folder, copy the arduino avr folder. That is provided here, in this repo.  If you are doing it manually, you will have to edit the boards.txt file.  Copy the boards.txt file provided here, and paste it in place of what is inside the file.
4. Restart the Arduino IDE
5. Go to Tools/Boards, and then select the new spacemouse board that appears.  
   Flash the provided sketch to the board.  
6. Install the 3DConnection Software.  
   Your arduino now should be recognized as a new 3D mouse device.
