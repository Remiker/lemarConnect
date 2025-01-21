# Design issues and fixes :
## SHT30 :
On the SHT30, there's a series resistor between SDA and SCL instead of a pull-up resistor. The sensor therefore works at first, but eventually stops working. 

You also have to be careful when soldering the SHT: if you put flux in the sensor or heat it too long, the sensor won't work.

<b>This will be updated in the next version <b>



## TEMT6000:
No issues found on this PCB