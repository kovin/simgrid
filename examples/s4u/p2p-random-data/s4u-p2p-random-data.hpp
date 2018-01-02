#ifndef P2P_RANDOM_DATA_HPP
#define P2P_RANDOM_DATA_HPP
#define SLEEP_DURATION 1

#include "simgrid/s4u.hpp"
#include "message.hpp"
#include <cstdlib>
#include <set>
#include <iostream>
#include <string>
#include <sstream>

class BaseNode
{
protected:
  virtual void create_and_send_message_if_needed() = 0;
  virtual Message* get_message_to_send() = 0;
  virtual void handle_new_transaction(Transaction *transaction) = 0;
};

class Node : public BaseNode {
  int peers_count;
  int connected_peers;
  double msg_size = 1000000;
  simgrid::s4u::CommPtr comm_received = nullptr;
  simgrid::s4u::MailboxPtr my_mailbox;
  bool disconnect_notified = false;

public:
  explicit Node(std::vector<std::string> args);
  static int active_nodes;
  static int network_bytes_produced;
  void operator()();

protected:
  int my_id;
  double messages_to_send = 90;
  std::map<int, Transaction> unconfirmed_transactions;
  std::set<int> blockchain;
  void create_and_send_message_if_needed();
  Message* get_message_to_send();
  int compute_unconfirmed_transactions_size();
  int total_bytes_received = 0;
  void handle_new_transaction(Transaction *transaction);
  void handle_new_block(Block *block);
  void handle_unconfirmed_transactions(UnconfirmedTransactions *message);
  void send_message_to_peers(Message* payload);

private:
  void notify_unconfirmed_transactions_if_needed();
  void receive();
  simgrid::s4u::MailboxPtr get_peer_mailbox(int peer_id);
};

class Miner : public Node {
public:
  explicit Miner(std::vector<std::string> args) : Node(args) {}
protected:
  void create_and_send_message_if_needed();
  Message* get_message_to_send();
  void handle_new_transaction(Transaction *transaction);
};

class Monitor
{
public:
  explicit Monitor(std::vector<std::string> args);
  void operator()();
};

template<typename KeyType, typename Value>
std::map<KeyType, Value> DiffMaps(const std::map<KeyType, Value> & left, const std::map<KeyType, Value> & right)
{
  std::map<KeyType, Value> result;
  typename std::map<KeyType, Value>::const_iterator il = left.begin();
  typename std::map<KeyType, Value>::const_iterator ir = right.begin();
  while (il != left.end())
  {
    if (ir == right.end() || il->first < ir->first) {
      result.insert(std::make_pair(il->first, il->second));
      ++il;
    } else if (ir != right.end()) {
      if (il->first == ir->first) {
        ++il;
      }
      ++ir;
    }
  }
  return result;
}

template<typename KeyType, typename Value>
std::map<KeyType, Value> JoinMaps(const std::map<KeyType, Value> & left, const std::map<KeyType, Value> & right)
{
    std::map<KeyType, Value> result;
    typename std::map<KeyType, Value>::const_iterator il = left.begin();
    typename std::map<KeyType, Value>::const_iterator ir = right.begin();
    while (il != left.end() || ir != right.end())
    {
      if (il != left.end()) {
        result.insert(std::make_pair(il->first, il->second));
        ++il;
      }
      if (ir != right.end()) {
        result.insert(std::make_pair(ir->first, ir->second));
        ++ir;
      }
    }
    return result;
}

#endif /* P2P_RANDOM_DATA_HPP */
