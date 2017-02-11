#include <iostream>
#include <stdexcept>

extern "C" {

#include <unistd.h>

}

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/program_options.hpp>

#include "bw_spi_relay.hpp"

namespace po = boost::program_options;

/** The name of the message queue used to communicate between the
    controller and the daemon

    For the curious, it seems to appear inside /dev/shm directory.
*/
auto constexpr command_queue_name { "BitWizard_SPI_relay-command_queue" };


/** A data structure to send commands from the controller to the daemon

    Use a standard-layout object so we can send it through some IPC.
*/
struct command_message {
  //* The relay or command to send
  int relay_command;
  //* On (true)/off (false) state
  bool state;
};


/* The inter-process communication message queue used to talk between the
   controller and daemon
*/
boost::interprocess::message_queue command_queue {
  boost::interprocess::open_or_create,
  command_queue_name,
  // Use only 1 slot queue to detect blocking/absent daemon
  1,
  // The size of the object to send
  sizeof(command_message)
};


//* Handle the requests from the controling process
void handle_requests() {
  /* Close the default file descriptors so it can run detached in the
     background */
  ::close(STDIN_FILENO);
  ::close(STDIN_FILENO);
  ::close(STDIN_FILENO);

  // Physical access to the relays
  raspberry_pi::bit_wizard::spi_relay4 r;
  // The message to be received from the controller
  command_message cm;
  boost::interprocess::message_queue::size_type recvd_size;
  // Not used here
  unsigned int priority;

  for(;;) /* ever */ {
    // Wait for a command
    command_queue.receive(&cm, sizeof(cm), recvd_size, priority);
    if (recvd_size != sizeof(cm))
      throw std::runtime_error { "Received message with wrong size" };
    // Set the state of the relay to the requested state
    r.switch_state(cm.relay_command, cm.state);
  }
}


//* Start the daemon process controling the relays
void daemon() {
  // Clone the process
  auto pid = ::fork();
  if (pid == 0)
    // We are the child and play the role of the daemon
    handle_requests();
  else if (pid == -1)
    throw std::runtime_error { "Cannot fork a process to act as a daemon" };
  /* Otherwise this process continue and can serve for example as the
     controller too */
}


/** Send a command to the daemon

    \param[in] relay_command is the id of relay or command

    \param[in] s is a boolean to define the state of the relay: \c true
    for "on" and \c false for "off"
*/
void send_command(int relay_command, bool state) {
  command_message cm { relay_command, state };
  command_queue.send(&cm, sizeof(cm), 0);
}


/** Switch a relay on

    \param[in] relay is the id of relay to switch on
*/
void switch_on(int relay) {
  send_command(relay, true);
}


/** Switch a relay off

    \param[in] relay is the id of relay to switch off
*/
void switch_off(int relay) {
  send_command(relay, false);
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
