#ifndef MESSAGE_HPP
#define MESSAGE_HPP

typedef enum {
  MESSAGE_BLOCK,
  MESSAGE_TRANSACTION,
  UNCONFIRMED_TRANSACTIONS
} e_message_type;

long lrand()
{
  if (sizeof(int) < sizeof(long)) {
    return (static_cast<long>(rand()) << (sizeof(int) * 8)) | rand();
  } else {
    return rand();
  }
}

class Message
{
public:
  long id;
  int peer_id;
  int size;
  Message(int peer_id, int size) : peer_id(peer_id), size(size) {
    id = lrand();
  };
  Message(int peer_id): peer_id(peer_id), size(0) {
    id = lrand();
  };
  virtual e_message_type get_type() = 0;
  ~Message() = default;
};

class Transaction : public Message {
  public:
    Transaction (int peer_id, int size) : Message(peer_id, size) { };

    e_message_type get_type() {
      return MESSAGE_TRANSACTION;
    }
};

class Block : public Message
{
  public:
    std::map<int, Transaction> transactions;
    Block (int peer_id, std::map<int, Transaction> transactions) : Message(peer_id), transactions(transactions) {
        size += 1000000;
        for (auto const& idAndTransaction : transactions) {
          size += idAndTransaction.second.size;
        }
    };

    e_message_type get_type() {
      return MESSAGE_BLOCK;
    }
};

class UnconfirmedTransactions : public Message
{
  public:
    std::map<int, Transaction> unconfirmed_transactions;
    UnconfirmedTransactions (int peer_id, std::map<int, Transaction> unconfirmed_transactions) : Message(peer_id), unconfirmed_transactions(unconfirmed_transactions) { };

    e_message_type get_type() {
      return UNCONFIRMED_TRANSACTIONS;
    }
};

#endif /* MESSAGE_HPP */
