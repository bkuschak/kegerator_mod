# kegerator mod  

This project is a workaround to prevent short cycling on the Komos V2 kegerator. 

For some unknown reason the temperature controller occassionally turns the 
compressor on for about 1 second then immediately shuts it off. On my kegerator,
this can happen several times per hour. It tends to happen more often when the 
temperature setpoint is above 45 °F.

The workaround is to insert a small microcontroller board between the controlller
board and the compressor. This board intercepts the signal that controls the 
compressor. The software filters out any short transients and enforces a minimum 
on-time and a minimum off-time for the compressor.  

It has been running fine on my kegerator for several years now.

<p align="center"><img src="photos/komos_v2.png" alt="komos_v2" width="240"/></p>

The board is installed inline between the front panel temperature controller and the 
compressor power board.

<p align="center"><img src="photos/front_rev_b.jpg" alt="front" width="800"/></p>
<p align="center"><img src="photos/back_rev_b.jpg" alt="back" width="800"/></p>

See the other folders for more information on the [hardware](hardware/README.md) 
and [software](software/README.md).

If you need hardware, contact me.  I may still have a few boards in stock.
