#include "s4u-p2p-random-data.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

XBT_LOG_EXTERNAL_DEFAULT_CATEGORY(p2p_random_data);

int Node::active_nodes = 0;
int Node::network_bytes_produced = 0;

Node::Node(std::vector<std::string> args)
{
  active_nodes++;
  xbt_assert(args.size() == 3, "Expecting 2 parameters from the XML deployment file but got %zu", args.size());
  my_id = std::stoi(args[1]);
  peers_count = std::stoi(args[2]);
  xbt_assert(peers_count > 0, "You should define at least one peer");
  connected_peers = peers_count - 1;
  std::string my_mailbox_name = std::string("receiver-") + std::to_string(my_id);
  my_mailbox = simgrid::s4u::Mailbox::byName(my_mailbox_name);
}

void Node::operator()()
{
  while (messages_to_send > 0) {
    receive();
    create_and_send_message_if_needed();
  }
  while (simgrid::s4u::Engine::getClock() < 1000) {
    receive();
    simgrid::s4u::this_actor::sleep_for(1);
  }
  /*XBT_INFO(
    "\nStats\n"
    "\ttotal bytes received:\t%d\n"
    "\ttotal network bytes produced:\t%d",
    total_bytes_received, network_bytes_produced
  );*/
  active_nodes--;
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
    Message *payload = new UnconfirmedTransactions(my_id, unconfirmed_transactions);
    send_message_to_peers(payload);
  }
}

void Node::send_message_to_peers(Message* payload) {
  for (int i = 0; i < 5; ++i) {
    int random_peer_index_to_contact = (rand() % (peers_count - 1)) + 1;
    int peer_id = (random_peer_index_to_contact + my_id) % peers_count;
    simgrid::s4u::MailboxPtr mbox = get_peer_mailbox(peer_id);
    mbox->put_async(payload, msg_size + payload->size);
  }
}

Message* Node::get_message_to_send()
{
  int numberOfBytes = rand() & 100000;
  Message* message = new Transaction(my_id, numberOfBytes);
  total_bytes_received += message->size;
  network_bytes_produced += message->size;
  return message;
}

void Node::receive()
{
  if (!my_mailbox->empty()) {
    void* data = my_mailbox->get();
    Message *payload = static_cast<Message*>(data);
    comm_received = nullptr;
    switch (payload->get_type()) {
      case MESSAGE_TRANSACTION:
        handle_new_transaction(static_cast<Transaction*>(data));
        break;
      case MESSAGE_BLOCK:
        handle_new_block(static_cast<Block*>(data));
        break;
      case UNCONFIRMED_TRANSACTIONS:
        handle_unconfirmed_transactions(static_cast<UnconfirmedTransactions*>(data));
        //XBT_INFO("Recibi unconfirmed transactions\n");
        break;
      default:
        THROW_IMPOSSIBLE;
    }
    notify_unconfirmed_transactions_if_needed();
  }
}

void Node::handle_new_transaction(Transaction *transaction) {
  std::map<int, Transaction>::iterator it;
  it = unconfirmed_transactions.find(transaction->id);
  if (it == unconfirmed_transactions.end()) {
    int pre_size = compute_unconfirmed_transactions_size();
    unconfirmed_transactions.insert(std::make_pair(transaction->id, *transaction));
    int post_size = compute_unconfirmed_transactions_size();
    total_bytes_received += (post_size - pre_size);
    network_bytes_produced += (post_size - pre_size);
  }
}

void Node::handle_new_block(Block *block) {
  int pre_size = compute_unconfirmed_transactions_size();
  unconfirmed_transactions = DiffMaps(unconfirmed_transactions, block->transactions);
  int post_size = compute_unconfirmed_transactions_size() + block->size;
  XBT_INFO("Despues de recibir el bloque gane %d\n", post_size - pre_size);

  total_bytes_received += (post_size - pre_size);
  network_bytes_produced += (post_size - pre_size);
}

void Node::handle_unconfirmed_transactions(UnconfirmedTransactions *message) {
  //XBT_INFO("START handle_unconfirmed_transactions\n");
  int pre_size = compute_unconfirmed_transactions_size();
  unconfirmed_transactions = JoinMaps(unconfirmed_transactions, message->unconfirmed_transactions);
  int post_size = compute_unconfirmed_transactions_size();
  total_bytes_received += (post_size - pre_size);
  network_bytes_produced += (post_size - pre_size);
  //XBT_INFO("END handle_unconfirmed_transactions\n\t%d\n\t%d\n", pre_size, post_size);
}

int Node::compute_unconfirmed_transactions_size() {
  int result = 0;
  typename std::map<int, Transaction>::const_iterator it = unconfirmed_transactions.begin();
  while (it != unconfirmed_transactions.end())
  {
    result += it->second.size;
    ++it;
  }
  return result;
}
/*
void Node::update_unconfirmed_transactions_size() {
  int pre_size = compute_unconfirmed_transactions_size();
  XBT_INFO("Empiezo con: %d\n", transaction.id);
}
*/
simgrid::s4u::MailboxPtr Node::get_peer_mailbox(int peer_id)
{
  std::string mboxName = std::string("receiver-") + std::to_string(peer_id);
  return simgrid::s4u::Mailbox::byName(mboxName);
}