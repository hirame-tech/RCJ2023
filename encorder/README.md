## Hardware spec
- IC            AS5048A
- Voltage       5V
- Communication SPI

## Software memo
angle data : 14bit
(First time)write 0xFF → High byte
(Second time)write 0xFF → Low byte

angle data = (High byte << 8 )+low byte

(In program,you had better add &0x3FFF because angle data is 14bit.)