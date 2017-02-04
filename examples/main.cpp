#include "bw_spi_relay.hpp"


int main(int argc, char *argv[]) {

  raspberry_pi::bit_wizard::spi_relay4 r;

  // Switch relay 2 on
  r.switch_on(2);

  return 0;
}
