#ifndef P2P_RANDOM_DATA_HPP
#define P2P_RANDOM_DATA_HPP
#define SLEEP_DURATION 1

#include "simgrid/s4u.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

class Node {
  int my_id;
  int peers_to_contact;
  int peers_count;
  int connected_peers;
  double msg_size = 1000000;
  double blocks_to_send = 4;
  simgrid::s4u::CommPtr comm_received = nullptr;
  int total_bytes_received = 0;
  simgrid::s4u::MailboxPtr my_mailbox;
  bool disconnect_notified = false;
  static int network_bytes_produced;

public:
  explicit Node(std::vector<std::string> args);
  void operator()();

private:
  void create_and_send_block_if_needed();

  void create_and_send_block();

  void receive();

  simgrid::s4u::MailboxPtr get_peer_mailbox(int peer_id);
};

#endif /* P2P_RANDOM_DATA_HPP */
