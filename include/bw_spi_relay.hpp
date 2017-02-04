/** \file

    A modern C++14 interface to control the Raspberry SPI Relay card from
    BitWizard for Raspberry Pi

    Ronan Keryell.

    Open source according to MIT license
*/

// Do not include twice
#ifndef RASPBERRY_PI_BITWIZARD_SPI_RELAY
#define RASPBERRY_PI_BITWIZARD_SPI_RELAY

extern "C" {

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

}

#include <array>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <stdexcept>

/** Some Raspberry Pi-specific code
 */
namespace raspberry_pi {

/** An abstraction of the Linux spidev user-land device driver for SPI
    peripherals

    For some informations:


    - https://www.raspberrypi.org/documentation/hardware/raspberrypi/spi/README.md

    - linux/Documentation/spi/spidev

    - /usr/include/linux/spi/spidev.h

    - on some old kernels linux/Documentation/spi/spidev_test.c

    - http://www.hertaville.com/interfacing-an-spi-adc-mcp3008-chip-to-the-raspberry-pi-using-c.html
*/
class spidev {

  //* The file descriptor of the SPI device driver
  int fd { -1 };

public:

  //* The Linux device driver to use: SPI bus 0, chip-select 0
  static constexpr auto path = "/dev/spidev0.0";

  //* Use clock polarity: idle low, clock phase on leading edge
  std::uint8_t spi_mode = SPI_MODE_0;

  //* Use 8 bit data transfers
  std::uint8_t bits = 8;

  //* Transfer speed
  std::uint32_t speed_HZ = 240000;


  //* Open the SPI device
  spidev() {
    // Open the device for reading and writing
    fd = ::open(path, O_RDWR);

    if (fd == -1)
      throw std::runtime_error { "Cannot open the SPI device" };

    if (::ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1)
      throw std::runtime_error { "Cannot set bit format for writing with SPI" };

    if (::ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed_HZ) == -1)
      throw std::runtime_error { "Cannot set speed for writing with SPI" };
  }


  //* Close the SPI device
  ~spidev() {
    // Close the device if it was opened successfully
    if (fd != -1)
      ::close(fd);
  }


  //* Send and receive data
  void
  transfer(std::array<std::uint8_t, 3> &buffer) {
    /* Zero-initialize the structure, including currently unused fields,
       to accommodate potential future updates */
    spi_ioc_transfer tr = { };
    tr.tx_buf = reinterpret_cast<__u64>(&buffer);
    tr.rx_buf = reinterpret_cast<__u64>(&buffer);
    tr.len = buffer.size();

    // Send 1 message
    if (::ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1)
      throw std::runtime_error { "Cannot send data over SPI" };
}

};


//* Abstractions specific to BitWizard products
namespace bit_wizard {

/** An abstraction of the Raspberry SPI Relay from BitWizard

    Product Code: RPi-spi-relay
    http://www.bitwizard.nl/shop/raspberry-pi?product_id=148

    http://www.bitwizard.nl/wiki/Raspberry_Relay

    - https://github.com/rewolff/bw_rpi_tools/blob/master/bw_tool/bw_tool.c

*/
class spi_relay4 {

  spidev spi;

  // The SPI address of the relay on the bus
  static auto constexpr address = 0xA6;
  // The register in the SPI DIO controlling the relay
  static auto constexpr register_id = 0x10;

  /** Keep the state of the 4 relays.

      A set bit represents a relay in on state */
  std::bitset<4> state;


  //* Synchronize the state of the relais with the local state
  void update() {
    std::array<std::uint8_t, 3>
      message { address,
                register_id,
                // The 4 lower bits represent the state of the relais
                static_cast<std::uint8_t>(state.to_ulong()) };
    spi.transfer(message);
  }

public:

  /** Switch a relay on

      \param[in] relay is the number (0--3) of the relay to switch on
   */
  void switch_on(std::uint8_t relay) {
    state.set(relay);
    update();
  }

};


}
}


/*
    # Some Emacs stuff:
    ### Local Variables:
    ### ispell-local-dictionary: "american"
    ### eval: (flyspell-prog-mode)
    ### End:
*/

#endif // RASPBERRY_PI_BITWIZARD_SPI_RELAY
