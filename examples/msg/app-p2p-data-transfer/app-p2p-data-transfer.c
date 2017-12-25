/* Copyright (c) 2010-2016. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#include "simgrid/msg.h"
# define MAX_STRING_SIZE 80

XBT_LOG_NEW_DEFAULT_CATEGORY(msg_app_masterworker, "Messages specific for this msg example");

void sendMessages(int id, int peers_count, int peers_to_contact, char* task_name, char *my_mailbox)
{
  double comp_size = 500000000;
  double comm_size = 1000000000;
  char mailbox[MAX_STRING_SIZE];
  for (int i = 1; i <= peers_to_contact; ++i) {
    int peer_id = (id + i) % peers_count;
    XBT_INFO("worker-%d: I will send a message to '%d'", id, peer_id);
    msg_task_t task = MSG_task_create(task_name, comp_size, comm_size, NULL);
    snprintf(mailbox, MAX_STRING_SIZE - 1, "worker-%d", peer_id); /* - Select a @ref worker in a round-robin way */
    MSG_task_isend(task, mailbox);
  }
}

void receiveMessages(int id, int peers_count, int peers_to_contact, char* my_mailbox)
{
  msg_task_t *tasks = xbt_new(msg_task_t, peers_to_contact);
  xbt_dynar_t comms = xbt_dynar_new(sizeof(msg_comm_t), NULL);
  for (int i = peers_to_contact; i >= 1; --i) {
    int peer_id = (id - i + peers_count) % peers_count;
    XBT_INFO("worker-%d: I'll wait for a message from '%d'", id, peer_id);
    int task_id = i % peers_to_contact;
    tasks[task_id] = NULL;
    msg_comm_t comm = MSG_task_irecv(&tasks[task_id], my_mailbox);
    xbt_dynar_push_as(comms, msg_comm_t, comm);
  }
  while (!xbt_dynar_is_empty(comms)) {
    msg_comm_t comm;
    // MSG_comm_waitany returns the rank of the comm that just ended. Remove it.
    xbt_dynar_remove_at(comms, MSG_comm_waitany(comms), &comm);
    msg_task_t task = MSG_comm_get_task(comm);
    MSG_comm_destroy(comm);
    XBT_INFO("worker-%d: I got task \"%s\" and I will start processing it", id, MSG_task_get_name(task));
    MSG_task_execute(task);
    XBT_INFO("worker-%d: task \"%s\" is done", id, MSG_task_get_name(task));
    msg_error_t err = MSG_task_destroy(task);
    xbt_assert(err == MSG_OK, "MSG_task_destroy failed");
  }
  xbt_dynar_free(&comms);
  xbt_free(tasks);
}

/* Main functions of the Worker processes */
static int peer(int argc, char *argv[])
{
  int id = xbt_str_parse_int(argv[1], "Invalid argument %s");
  int peers_count = xbt_str_parse_int(argv[2], "Invalid argument %s");
  int peers_to_contact = MIN(1, peers_count - 1);
  char task_name[MAX_STRING_SIZE];
  char my_mailbox[MAX_STRING_SIZE];

  snprintf(task_name, MAX_STRING_SIZE - 1, "Task_%d", id);
  snprintf(my_mailbox, MAX_STRING_SIZE - 1, "worker-%d", id);

  XBT_INFO("worker-%d: hi!", id);
  XBT_INFO("worker-%d: there are %d like me", id, peers_count);
  sendMessages(id, peers_count, peers_to_contact, task_name, my_mailbox);
  receiveMessages(id, peers_count, peers_to_contact, my_mailbox);
  return 0;
}

int main(int argc, char *argv[])
{
  MSG_init(&argc, argv);
  xbt_assert(argc > 2, "Usage: %s platform_file deployment_file\n"
             "\tExample: %s exponential_10_nodes_alpha_.5.xml exponential_10_nodes_alpha_.5_deployment.xml\n", argv[0], argv[0]);
  /* - Load the platform description */
  MSG_create_environment(argv[1]);
  MSG_function_register("peer", peer);
  /* - Deploy the application */
  MSG_launch_application(argv[2]);
  /* - Run the simulation */
  msg_error_t res = MSG_main();
  XBT_INFO("Simulation time %g", MSG_get_clock());
  return res != MSG_OK;
}
