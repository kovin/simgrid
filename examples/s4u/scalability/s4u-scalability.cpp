#include "simgrid/s4u.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

XBT_LOG_NEW_DEFAULT_CATEGORY(msg_async_waitall, "Messages specific for this msg example");

class Peer {
  int my_id;
  int peers_count;
  double comp_size = 500000000;
  double msg_size = 10000;

public:
  explicit Peer(std::vector<std::string> args)
  {
    xbt_assert(args.size() == 3, "Expecting 2 parameter from the XML deployment file but got %zu", args.size());
    my_id = std::stoi(args[1]);
    peers_count = std::stoi(args[2]);
  }
  void operator()()
  {
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
    std::vector<simgrid::s4u::CommPtr> pending_comms;
    void* datum = nullptr;
    for (int i = 0; i < (peers_count - 1); ++i) {
      simgrid::s4u::CommPtr comm = my_mailbox->get_async(&datum);
      pending_comms.push_back(comm);
    }
    while (not pending_comms.empty()) {
      int changed_pos = simgrid::s4u::Comm::wait_any(&pending_comms);
      std::string* received = static_cast<std::string*>(datum);
      XBT_INFO("I got a '%s'.", received->c_str());
      pending_comms.erase(pending_comms.begin() + changed_pos);
      simgrid::s4u::this_actor::execute(comp_size);
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
