#include <chrono>
#include <thread>

#include "bw_spi_relay.hpp"

// To use the nice litterals prefixes to express time, such as 2s, 3h, 27ms
using namespace std::chrono_literals;

//* Exercise the various API methods
int main(int argc, char *argv[]) {

  raspberry_pi::bit_wizard::spi_relay4 r;

  // Switch relay 2 on
  r.switch_on(2);

  // Save the state for later reuse
  auto saved_state = r.get_state();

  // Wait a half second
  std::this_thread::sleep_for(0.5s);

  // Switch relay 2 off
  r.switch_off(2);
  std::this_thread::sleep_for(0.5s);

  // Full on
  r.all_on();
  std::this_thread::sleep_for(0.5s);

  // Full off
  r.all_off();
  std::this_thread::sleep_for(0.3s);

  // Make a chaser on the relays & LED
  for (int j = 0; j < 3; ++j) {
    for (auto i = r.first_relay; i <= r.last_relay; ++i) {
      if (i > r.first_relay)
        r.switch_off(i - 1);

      r.switch_on(i);
      std::this_thread::sleep_for(200ms);
    }
    for (auto i = r.last_relay; i >= r.first_relay; --i) {
      if (i < r.last_relay)
        r.switch_off(i + 1);

      r.switch_on(i);
      std::this_thread::sleep_for(200ms);
    }
  }

  // Restore a state from the past
  r.set_state(saved_state);
  std::this_thread::sleep_for(200ms);

  r.switch_state(1, true);
  std::this_thread::sleep_for(200ms);

  r.switch_state(1, false);

  return 0;
}

/*
    # Some Emacs stuff:
    ### Local Variables:
    ### ispell-local-dictionary: "american"
    ### eval: (flyspell-prog-mode)
    ### End:
*/
