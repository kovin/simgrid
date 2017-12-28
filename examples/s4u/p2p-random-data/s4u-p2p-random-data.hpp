#include <simgrid/s4u.hpp>

enum e_message_type {
  MESSAGE_NEW_BLOCK,
  MESSAGE_TRANSACTION,
  MESSAGE_BYE
};

class Message {
public:
  e_message_type type;
  int peer_id;
  int block_size = 0;
  Message(e_message_type type, int peer_id) : type(type), peer_id(peer_id){};
  Message(e_message_type type, int peer_id, int block_size) : type(type), peer_id(peer_id), block_size(block_size){};
  ~Message() = default;
};


class Transaction : public Message {
    public:
        int size = 0;
        Transaction (int peer_id, int size) : Message(MESSAGE_TRANSACTION, peer_id), size(size) { };
};
/*}

class Mother {
  public:
    Mother ()
      { cout << "Mother: no parameters\n"; }
    Mother (int a)
      { cout << "Mother: int parameter\n"; }
};

class Daughter : public Mother {
  public:
    Daughter (int a)
      { cout << "Daughter: int parameter\n\n"; }
};

class Son : public Mother {
  public:
    Son (int a) : Mother (a)
      { cout << "Son: int parameter\n\n"; }
};
*/
