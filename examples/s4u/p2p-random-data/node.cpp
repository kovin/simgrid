#include "s4u-p2p-random-data.hpp"
#include "message.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

XBT_LOG_EXTERNAL_DEFAULT_CATEGORY(p2p_random_data);

int Node::network_bytes_produced = 0;

Node::Node(std::vector<std::string> args)
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

void Node::operator()()
{
  while (messages_to_send > 0) {
    create_and_send_message_if_needed();
  }
  while (simgrid::s4u::Engine::getClock() < 50) {
    receive();
    simgrid::s4u::this_actor::sleep_for(1);
  }
  XBT_INFO(
    "\nStats\n"
    "\ttotal bytes received:\t%d\n"
    "\ttotal network bytes produced:\t%d",
    total_bytes_received, network_bytes_produced
  );
}

void Node::create_and_send_message_if_needed()
{
  if ((messages_to_send > 0) && ((rand() % 100) < 75)) {
    send_message_to_peers(get_message_to_send());
    messages_to_send--;
  }
}

void Node::notify_unconfirmed_transactions_if_needed()
{
  if ((rand() % 100) < 25) {
    std::vector<Transaction> *transactions = new std::vector<Transaction>;
    Message *payload = new UnconfirmedTransactions(my_id, transactions);
    //Message *payload = new Otro(my_id, 1234);
    send_message_to_peers(payload);
    XBT_INFO("Tengo que notificar de transacciones no confirmadas");
  }
}

void Node::send_message_to_peers(Message* payload) {
    switch (payload->get_type()) {
      case MESSAGE_TRANSACTION:
        XBT_INFO("Envio message transaction");
        break;
      case UNCONFIRMED_TRANSACTIONS:
        XBT_INFO("Envio unconfirmed transactions");
        break;
      case MESSAGE_BLOCK:
        XBT_INFO("Envio message block");
        break;
    }
    int random_peer_index_to_contact = (rand() % peers_to_contact) + 1;
    int peer_id = (random_peer_index_to_contact + my_id) % peers_count;
    simgrid::s4u::MailboxPtr mbox = get_peer_mailbox(peer_id);
    mbox->put_async(payload, msg_size + payload->size);
}

Message* Node::get_message_to_send()
{
  int numberOfBytes = rand() & 100000;
  Message* message = new Transaction(my_id, numberOfBytes);
  return message;
}

void Node::receive()
{
  if (!my_mailbox->empty()) {
    void* data = my_mailbox->get();
    Message *payload = static_cast<Message*>(data);
    Block *block;
    comm_received = nullptr;
    Transaction transaction = Transaction(payload->peer_id, payload->size);
    switch (payload->get_type()) {
      case MESSAGE_TRANSACTION:
        transaction.id = payload->id;
        unconfirmed_transactions.push_back(transaction);
        total_bytes_received += payload->size;
        network_bytes_produced += payload->size;
        break;
      case MESSAGE_BLOCK:
        block = static_cast<Block*>(data);
        total_bytes_received += payload->size;
        network_bytes_produced += payload->size;
        break;
      case UNCONFIRMED_TRANSACTIONS:
        XBT_INFO("Recibi unconfirmed transactions");
        break;
      default:
        THROW_IMPOSSIBLE;
    }
    notify_unconfirmed_transactions_if_needed();
  }
}

simgrid::s4u::MailboxPtr Node::get_peer_mailbox(int peer_id)
{
  std::string mboxName = std::string("receiver-") + std::to_string(peer_id);
  return simgrid::s4u::Mailbox::byName(mboxName);
}
