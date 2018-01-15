#include "s4u-block-verification.hpp"

XBT_LOG_EXTERNAL_DEFAULT_CATEGORY(block_verification);

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
  long pre_size = compute_unconfirmed_transactions_size();
  unconfirmed_transactions = std::map<long, Transaction>();
  long post_size = compute_unconfirmed_transactions_size();
  total_bytes_received += (post_size - pre_size + message->size);
  network_bytes_produced += (post_size - pre_size + message->size);
  return message;
}

void Miner::handle_new_transaction(Transaction *transaction) {
  Node::handle_new_transaction(transaction);
}
