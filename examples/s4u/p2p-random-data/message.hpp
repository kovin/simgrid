#ifndef MESSAGE_HPP
#define MESSAGE_HPP

enum e_message_type {
  MESSAGE_BLOCK,
  MESSAGE_TRANSACTION
};

class Message {
public:
  e_message_type type;
  int peer_id;
  int size = 0;
  Message(e_message_type type, int peer_id) : type(type), peer_id(peer_id){};
  Message(e_message_type type, int peer_id, int size) : type(type), peer_id(peer_id), size(size){};
  ~Message() = default;
};

class Transaction : public Message {
    public:
        Transaction (int peer_id, int size) : Message(MESSAGE_TRANSACTION, peer_id, size) { };
};

class Block : public Message {
    public:
        std::vector<Transaction> *transactions;
        Block (int peer_id, std::vector<Transaction> *transactions) : Message(MESSAGE_TRANSACTION, peer_id), transactions(transactions) { };
};

#endif /* MESSAGE_HPP */
