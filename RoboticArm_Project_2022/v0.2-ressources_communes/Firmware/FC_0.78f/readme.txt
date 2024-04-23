This readme contains useful information for the use/implementation of the custom Flight Control software. "Useful" as judged by me at the time of writing...

mwm 26.05.2011

1. Background info
The custom Flight Control was built off of the FC software from MikroKopter, starting with their version 0.78f. This version was chosen because it was found to work, after a relatively long search with different combinations of FC, NaviControl and MKTools versions. The versions as used are:
  - FC: Branched off of 0.78f
  - NC: 0.18c (std)
  - MKTools: 1.68c (std)

2. Coding/Uploding software
The custom flight control is compiled using "Programmers Notepad" of "WinAVR". The 
version of this software used might matter (search mk wiki/forum for info). To 
upload the compiled HEX file, use the MKTool. If the upload fails, retry. If the 
MKTools is unable to connect to the board, do following (from the "Keine Panik" 
section fo the MKtools wiki page):
  1. Disconnect vehicle from power
  2. On the FC board, disconnect the two ribbon cables from navi ctrl board
  3. Plug MKUsb ribbon cable into the matching slot on the FC board
  4. Start MKTools, select update & load file
  5. When the MKtools is trying to connect (i.e. while the dots are being printed) 
     connect the jumper on the MKUsb, jumpint the two pins sticking out normally 
     from the MKUSB board
  6. The update should now start as normal. If this did not work (MKUsb returns an error)
     disconnect the jumper, return to #4. It is a question of luck with the timing (this 
     can take a while to get right...).
  7. Remove the jumper, and return all ribbon cables to their original positions. Reconnect
     power cable. 
  
3. Communications
I use MKTools to update the software. It is also a good tool to use if you think you've
broken something big, as it knows how to talk to the NC.

I use my own custom program to talk to the vehicle (allows me to read custom info from 
the vehicle, and is 100% predictable). This can be found under 
  <SVN DIR>/Hexa_PC_com/exampleProject/useSerialComms.sln
and is controlled by user keyboard input. The keys are best explained by walking through the code.

3.1 Comms packets (refer to http://www.mikrokopter.de/ucwiki/en/SerialCommands)
The serial communications uses communication packets existant of an array of characters (let's call
it COMM_ARRAY). This array always has three leading characters of the following form:
 - COMM_ARRAY[0]: This always equals '#' and is used to identify the packet.
 - COMM_ARRAY[1]: Contains the address for the data (e.g. Flight control, or Navi Control...).
 - COMM_ARRAY[2]: The ID-byte, which tells us what the package is for. 
 - COMM_ARRAY[3,...]: this is the data (if it is even there), encoded in 64 (?bit). The encoding 
                      is quite complicated, but it should all happen without you noticing.

3.2 Custom ID bytes:
The following ID bytes are introduced in our custom software, and do not exist in the standard 
FC software. They are described below:

------+---------+--------------+---------------------------------------------------------------------
ID    |         | Comms        | 
Byte  | Address | Direction    | Data format
------+---------+--------------+---------------------------------------------------------------------
i     | FC      | TO vehicle   | Contains one byte (boolean) which enables / disables the sending 
      |         |              | of info strings (info strings are user defined strings in the FC 
      |         |              | code which can be used to send out debugging info -- info strings
      |         |              | are *NOT* understood by MKTools, you have to use the custom tool 
      |         |              | here). 
------+---------+--------------+---------------------------------------------------------------------
I     | FC      | FROM Vehicle | Contains an encoded info string, terminated by a 0. Anything after 
      |         |              | the zero is undefined. Once the info string is sent by the vehicle,
      |         |              | it is cleared (i.e. the vehicle does not ask for confirmation that 
      |         |              | the packet was decoded OK).
------+---------+--------------+---------------------------------------------------------------------
      |         |              | 
