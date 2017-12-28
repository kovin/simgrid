/* Copyright (c) 2010-2017. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

/* This example shows how to use simgrid::s4u::this_actor::wait_any() to wait for the first occurring event.
 *
 * As for the other asynchronous examples, the sender initiate all the messages it wants to send and
 * pack the resulting simgrid::s4u::CommPtr objects in a vector. All messages thus occurs concurrently.
 *
 * The sender then loops until there is no ongoing communication. Using wait_any() ensures that the sender
 * will notice events as soon as they occur even if it does not follow the order of the container.
 *
 * Here, finalize messages will terminate earlier because their size is 0, so they travel faster than the
 * other messages of this application.  As expected, the trace shows that the finalize of worker 1 is
 * processed before 'Message 5' that is sent to worker 0.
 *
 */

#include "simgrid/s4u.hpp"
#include "s4u-p2p-random-data.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

XBT_LOG_NEW_DEFAULT_CATEGORY(p2p_random_data, "Messages specific for this msg example");

#define SLEEP_DURATION 1

class Peer {
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
  explicit Peer(std::vector<std::string> args)
  {
    xbt_assert(args.size() == 4, "Expecting 3 parameters from the XML deployment file but got %zu", args.size());
    my_id = std::stoi(args[1]);
    peers_count = std::stoi(args[3]);
    xbt_assert(peers_count > 0, "You should define at least one peer");
    connected_peers = peers_count - 1;
    peers_to_contact = std::min(std::stoi(args[2]), peers_count - 1);
    std::string my_mailbox_name = std::string("receiver-") + std::to_string(my_id);
    my_mailbox = simgrid::s4u::Mailbox::byName(my_mailbox_name);
  }

  void operator()()
  {
    XBT_INFO("creating blocks");
    while (blocks_to_send > 0) {
      create_and_send_block_if_needed();
    }
    XBT_INFO("waiting blocks");
    while (simgrid::s4u::Engine::getClock() < 50) {
      receive();
      simgrid::s4u::this_actor::sleep_for(1);
    }
    XBT_INFO("Done");
    XBT_INFO(
      "\nStats\n"
      "\ttotal bytes received:\t%d\n"
      "\ttotal network bytes produced:\t%d",
      total_bytes_received, network_bytes_produced);
  }

private:
  void create_and_send_block_if_needed() {
      if (blocks_to_send > 0) {
        create_and_send_block();
      }
  }

  void create_and_send_block() {
    if ((rand() % 100) < 75) {
      int random_peer_index_to_contact = (rand() % peers_to_contact) + 1;
      int peer_id = (random_peer_index_to_contact + my_id) % peers_count;
      simgrid::s4u::MailboxPtr mbox = get_peer_mailbox(peer_id);
      int numberOfBytes;
      Message* payload;
      if ((rand() % 100) < 50) {
        XBT_INFO("Creating block");
        numberOfBytes = 10000000;
        payload = new Message(MESSAGE_NEW_BLOCK, my_id, numberOfBytes);
      } else {
        XBT_INFO("Creating Transaction");
        numberOfBytes = 1000000;
        payload = new Transaction(my_id, numberOfBytes);
      }
      mbox->put_async(payload, msg_size + numberOfBytes);
      blocks_to_send--;
    }
  }

  void receive() {
    if (!my_mailbox->empty()) {
      void* data = my_mailbox->get();
      Message *payload = static_cast<Message*>(data);
      comm_received = nullptr;
      switch (payload->type) {
        case MESSAGE_NEW_BLOCK:
        case MESSAGE_TRANSACTION:
            total_bytes_received += payload->block_size;
            network_bytes_produced += payload->block_size;
          break;
        case MESSAGE_BYE:
            connected_peers--;
            XBT_INFO("I got a bye from %d and now I'm connected to %d peers", payload->peer_id, connected_peers);
          break;
        default:
          THROW_IMPOSSIBLE;
      }
    }
  }

  simgrid::s4u::MailboxPtr get_peer_mailbox(int peer_id) {
    std::string mboxName = std::string("receiver-") + std::to_string(peer_id);
    return simgrid::s4u::Mailbox::byName(mboxName);
  }
};
int Peer::network_bytes_produced = 0;

int main(int argc, char *argv[])
{
  srand(1);
  xbt_assert(argc > 2, "Usage: %s platform_file deployment_file\n", argv[0]);

  simgrid::s4u::Engine e(&argc, argv);
  e.registerFunction<Peer>("peer");

  e.loadPlatform(argv[1]);
  e.loadDeployment(argv[2]);
  e.run();

  return 0;
}
