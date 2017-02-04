#include <chrono>
#include <thread>

#include "bw_spi_relay.hpp"

// To use the nice litterals prefixes to express time, such as 2s, 3h, 27ms
using namespace std::chrono_literals;


int main(int argc, char *argv[]) {

  raspberry_pi::bit_wizard::spi_relay4 r;

  // Switch relay 2 on
  r.switch_on(2);

  // Wait a half second
  std::this_thread::sleep_for(0.5s);

  // Switch relay 2 off
  r.switch_off(2);

  return 0;
}

/*
    # Some Emacs stuff:
    ### Local Variables:
    ### ispell-local-dictionary: "american"
    ### eval: (flyspell-prog-mode)
    ### End:
*/
