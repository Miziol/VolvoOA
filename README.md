# VolvoCANbus

## Description

At the beginning I'd like to add some features to my Volvo:
- display temperature of coolant
- add Android Auto and integrate it with car

I expected that all that features will be supported via CAN bus, but during the project I discovered that in Volvo P3, some interesting me data goes over LIN bus.

## Features

1. Read coolant temperature using OBD II standard
2. Display it on DIM of the car
3. Read button click from LIN bus
4. Emulate keyboard for AA

## Outside part of project

- AAsdk and Openauto forks from [imchos](https://github.com/imchos/openauto-dhu)
