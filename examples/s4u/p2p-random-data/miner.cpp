#include "s4u-p2p-random-data.hpp"

XBT_LOG_EXTERNAL_DEFAULT_CATEGORY(p2p_random_data);

void Miner::create_and_send_message_if_needed()
{
  if ((messages_to_send > 0) && ((rand() % 100) < 5)) {
    Node::send_message_to_peers(get_message_to_send());
    messages_to_send--;
  }
}

Message* Miner::get_message_to_send()
{
  Message* message = new Block(my_id, unconfirmed_transactions);
  int pre_size = compute_unconfirmed_transactions_size();
  unconfirmed_transactions = std::map<int, Transaction>();
  int post_size = compute_unconfirmed_transactions_size();
  total_bytes_received += (post_size - pre_size + message->size);
  network_bytes_produced += (post_size - pre_size + message->size);
  //XBT_INFO("new block\n\t%d\n\t%d\n\t%d\n", pre_size, post_size, message->size);
  return message;
}

void Miner::handle_new_transaction(Transaction *transaction) {
  //XBT_INFO("transaction size %d\n", transaction->size);
  Node::handle_new_transaction(transaction);
}
