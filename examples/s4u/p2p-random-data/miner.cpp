#include "s4u-p2p-random-data.hpp"

XBT_LOG_EXTERNAL_DEFAULT_CATEGORY(p2p_random_data);

Message* Miner::get_message_to_send()
{
  std::vector<Transaction> *transactions = new std::vector<Transaction>;
  for (int i = 0; i < 3; ++i) {
      int numberOfBytes = rand() & 100000;
      Transaction transaction = Transaction(my_id, numberOfBytes);
      transactions->push_back(transaction);
      XBT_INFO("# de transacciones en bloqueB %lu", transactions->size());
    }
  Message* message = new Block(my_id, transactions);
  return message;
}
