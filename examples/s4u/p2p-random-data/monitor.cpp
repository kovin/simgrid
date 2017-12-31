#include "s4u-p2p-random-data.hpp"


XBT_LOG_EXTERNAL_DEFAULT_CATEGORY(p2p_random_data);

Monitor::Monitor(std::vector<std::string> args) {
}

void Monitor::operator()() {
  XBT_INFO("\nTime\t\tTotal network bytes produced\n");
  double last_bytes_monitored = Node::network_bytes_produced;
  while (Node::active_nodes > 0) {
    if (last_bytes_monitored != Node::network_bytes_produced) {
        last_bytes_monitored = Node::network_bytes_produced;
        XBT_INFO(
          "%f\t%d\n",
          simgrid::s4u::Engine::getClock(),
          Node::network_bytes_produced
        );
    }
    simgrid::s4u::this_actor::sleep_for(1);
  }
}
