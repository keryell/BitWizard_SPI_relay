Modern C++ API for BitWizard SPI relay card for Raspberry Pi
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. section-numbering::

.. contents::


Introduction
------------

On a lengthy journey to automate the central heating of my house with
https://domoticz.com/, I bought by error several SPI versions of the
BitWizard relay card for the Raspberry Pi instead of the GPIO version of
the card... So instead of a native support of the GPIO pins of the
Raspberry Pi by https://domoticz.com/ I had to find some way to deal with
the SPI version...

Unfortunately I did not find a library compatible with my modern C++
palate, so here is a small C++14 library of my own to deal with this card.
It may be useful for some other people, so here it is.

Anyway, it is not complete waste of time: I will recycle and extend this
SPI library in some other projects.

This has been tested with Raspbian GNU/Linux 8 (jessie) with G++ 4.9 on a
Raspberry Pi 3.


The library
-----------

It is a pure header library to limit usage headache, using only C++14 STL
and Linux API. Fortunately, not a single ``malloc()``, ``free()``,
``new``, ``delete`` or pointer is used here. Pure organic vegetarian
source code.

Basically, you just have to include ``include/bw_spi_relay.hpp`` to use it
and the documentation is the comments from the public methods of the
``raspberry_pi::bit_wizard::spi_relay4``.

There is a small demo in ``examples/main.cpp``.

To compile it and run it: ::

  cd examples
  make
  ./main


Related links
-------------

Some useful information:

- http://www.bitwizard.nl/shop/raspberry-pi?product_id=148

- http://www.bitwizard.nl/wiki/Raspberry_Relay

- https://github.com/rewolff/bw_rpi_tools/blob/master/bw_tool/bw_tool.c

- https://www.raspberrypi.org/documentation/hardware/raspberrypi/spi/README.md

- ``linux/Documentation/spi/spidev``

- ``/usr/include/linux/spi/spidev.h``

- in some old kernel sources there is also
  ``linux/Documentation/spi/spidev_test.c``

- http://wiringpi.com/

- https://github.com/JoachimSchurig/CppGPIO

- http://www.airspayce.com/mikem/bcm2835/

- http://www.hertaville.com/interfacing-an-spi-adc-mcp3008-chip-to-the-raspberry-pi-using-c.html


..
    # Some Emacs stuff:
    ### Local Variables:
    ### mode: rst
    ### minor-mode: flyspell
    ### ispell-local-dictionary: "american"
    ### End:
