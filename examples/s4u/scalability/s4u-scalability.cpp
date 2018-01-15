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
#include <cstdlib>
#include <iostream>
#include <string>

XBT_LOG_NEW_DEFAULT_CATEGORY(msg_async_waitall, "Messages specific for this msg example");

class Peer {
  int my_id;
  int peers_count;
  double msg_size = 1000000;

public:
  explicit Peer(std::vector<std::string> args)
  {
    xbt_assert(args.size() == 3, "Expecting 2 parameter from the XML deployment file but got %zu", args.size());
    my_id = std::stoi(args[1]);
    peers_count = std::stoi(args[2]);
  }
  void operator()()
  {
    std::vector<simgrid::s4u::CommPtr> pending_comms;
    /* Start dispatching all messages to receivers, in a round robin fashion */
    for (int i = 1; i < peers_count; i++) {
      int peer_id = (i + my_id) % peers_count;
      std::string mboxName = std::string("receiver-") + std::to_string(peer_id);
      simgrid::s4u::MailboxPtr mbox = simgrid::s4u::Mailbox::byName(mboxName);
      std::string msgName = std::string("Message from ") + std::to_string(my_id);
      std::string* payload = new std::string(msgName); // copy the data we send:
                                                                // 'msgName' is not a stable storage location
      XBT_INFO("Send '%s' to '%s'", msgName.c_str(), mboxName.c_str());
      mbox->put_async(payload, msg_size);
    }
    XBT_INFO("Done dispatching all messages");
    std::string my_mailbox_name = std::string("receiver-") + std::to_string(my_id);
    simgrid::s4u::MailboxPtr my_mailbox = simgrid::s4u::Mailbox::byName(my_mailbox_name);
    int received_messages = 0;
    simgrid::s4u::CommPtr comm_received = nullptr;
    void* data = nullptr;
    while (received_messages < (peers_count - 1)) {
      if (comm_received == nullptr) {
        comm_received = my_mailbox->get_async(&data);
      }
      if (comm_received->test()) {
        std::string* received = static_cast<std::string*>(data);
        XBT_INFO("I got a '%s'.", received->c_str());
        delete received;
        comm_received = nullptr;
        received_messages++;
      } else {
        simgrid::s4u::this_actor::sleep_for(1);
      }
    }
    XBT_INFO("Done receiving all messages");
  }
};

int main(int argc, char *argv[])
{
  xbt_assert(argc > 2, "Usage: %s platform_file deployment_file\n", argv[0]);

  simgrid::s4u::Engine e(&argc, argv);
  e.registerFunction<Peer>("peer");

  e.loadPlatform(argv[1]);
  e.loadDeployment(argv[2]);
  e.run();
  XBT_INFO("Simulation time %g", SIMIX_get_clock());
  return 0;
}
