#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "aux-functions.hpp"

typedef enum {
  MESSAGE_BLOCK,
  MESSAGE_TRANSACTION,
  UNCONFIRMED_TRANSACTIONS
} e_message_type;


class Message
{
public:
  int id;
  int peer_id;
  long size;
  Message(int peer_id, long size) : peer_id(peer_id), size(size) {
    id = lrand();
  };
  Message(int peer_id): peer_id(peer_id), size(0) {
    id = lrand();
  };
  virtual e_message_type get_type() = 0;
  ~Message() = default;
};

class Transaction : public Message
{
  public:
    Transaction (int peer_id, long size) : Message(peer_id, size) { };

    e_message_type get_type() {
      return MESSAGE_TRANSACTION;
    }
};

class Block : public Message
{
  public:
    std::map<long, Transaction> transactions;
    Block (int peer_id, std::map<long, Transaction> transactions) : Message(peer_id), transactions(transactions) {
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
    std::map<long, Transaction> unconfirmed_transactions;
    UnconfirmedTransactions (int peer_id, std::map<long, Transaction> unconfirmed_transactions) : Message(peer_id), unconfirmed_transactions(unconfirmed_transactions) { };

    e_message_type get_type() {
      return UNCONFIRMED_TRANSACTIONS;
    }
};

#endif /* MESSAGE_HPP */
