#ifndef MESSAGE_HPP
#define MESSAGE_HPP

enum e_message_type {
  MESSAGE_BLOCK,
  MESSAGE_TRANSACTION,
  UNCONFIRMED_TRANSACTIONS
};

class Message {
public:
  e_message_type type;
  int id;
  int peer_id;
  int size;
  Message(e_message_type type, int peer_id, int size) : type(type), peer_id(peer_id), size(size) {
      id = rand();
  };
  Message(e_message_type type, int peer_id) {
    Message(type, peer_id, 0);
  };
  ~Message() = default;
};

class Transaction : public Message {
  public:
    Transaction (int peer_id, int size) : Message(MESSAGE_TRANSACTION, peer_id, size) { };
};

class Block : public Message {
  public:
    std::vector<Transaction> *transactions;
    Block (int peer_id, std::vector<Transaction> *transactions) : Message(MESSAGE_BLOCK, peer_id), transactions(transactions) {
        for (auto const& transaction : *transactions) {
          size += transaction.size;
        }
    };
};

class UnconfirmedTransactions : public Message {
  public:
    std::vector<Transaction> *transactions;
    UnconfirmedTransactions (int peer_id, std::vector<Transaction> *transactions) : Message(UNCONFIRMED_TRANSACTIONS, peer_id), transactions(transactions) { };
};

#endif /* MESSAGE_HPP */
