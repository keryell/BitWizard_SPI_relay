#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include <boost/program_options.hpp>

#include "bw_spi_relay.hpp"

// To use the nice litterals prefixes to express time, such as 2s, 3h, 27ms
using namespace std::chrono_literals;

namespace po = boost::program_options;


//* Start the daemon process controling the relays
void daemon() {
}


/** Switch a relay on

    \param[in] relay is the id of relay to switch on
*/
void switch_on(int relay) {
}


/** Switch a relay off

    \param[in] relay is the id of relay to switch off
*/
void switch_off(int relay) {
}


//* Exercise the various API methods
int main(int argc, char *argv[]) {

  int relay = -1;

  // The description title when displaying the help
  po::options_description desc {
    "Controler and daemon for the BitWizard SPI relay card"
  };

  // Add the supported options
  desc.add_options()
    ("help,h", "produce this help message")
    ("daemon,d", "run as the detached controling daemon in the background")
    ("relay,r", po::value<int>(&relay), "specify the relay id (0--3)")
    ("on", "switch the relay on")
    ("off", "switch the relay off")
    ;

  // Where to get the option results
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  // Display the help if required
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 0;
  }

  if (vm.count("daemon"))
    daemon();

  if (vm.count("relay")) {
    relay = vm["relay"].as<int>();
  }

  if (vm.count("on")) {
    switch_on(relay);
  }


  if (vm.count("off")) {
    switch_off(relay);
  }

  return 0;
}

/*
    # Some Emacs stuff:
    ### Local Variables:
    ### ispell-local-dictionary: "american"
    ### eval: (flyspell-prog-mode)
    ### End:
*/
