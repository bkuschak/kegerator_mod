## Arduino sketch for ATTINY212

At the time of writing, the ATTINY212 is currently available at [Digikey](https://www.digikey.com/en/products/detail/microchip-technology/ATTINY212-SSN/9947536).

### Prerequisites

First, install these libraries:

```
https://github.com/SpenceKonde/megaTinyCore
https://github.com/contrem/arduino-timer
```

Configure as shown:

<img src="arduino_attiny_212_configuration.png" alt="configuration" width="624"/>

### Programming

Programming can be done with a [UPDI programmer](https://www.amazon.com/dp/B09X64YRLD) and 3 jumper wires temporarily inserted into the board.

Burn the bootloader once to program the fuses. Then upload the sketch.
