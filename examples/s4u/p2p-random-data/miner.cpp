#include "s4u-p2p-random-data.hpp"

XBT_LOG_EXTERNAL_DEFAULT_CATEGORY(p2p_random_data);

Message* Miner::get_message_to_send()
{
  std::vector<Transaction> transactions;
  for (int i = 0; i < 3; ++i) {
      int numberOfBytes = rand() & 100000;
      Transaction transaction = Transaction(my_id, numberOfBytes);
      transactions.push_back(transaction);
      XBT_INFO("Created transaction with size %d", transaction.size);
    }
  Message* message = new Block(my_id, &transactions);
  XBT_INFO("Created block with size %d", message->size);
  return message;
}
