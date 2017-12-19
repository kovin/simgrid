/* Copyright (c) 2010-2016. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#include "simgrid/msg.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(msg_app_masterworker, "Messages specific for this msg example");

/* Main functions of the Worker processes */
static int peer(int argc, char *argv[])
{
  int id = xbt_str_parse_int(argv[1], "Invalid argument %s");
  int count = xbt_str_parse_int(argv[2], "Invalid argument %s");
  double comp_size = 500000000;
  double comm_size = 1000000000;
  int max_string_size = 80;
  char task_name[max_string_size];
  char mailbox[max_string_size];
  char my_mailbox[max_string_size];

  snprintf(task_name, max_string_size - 1, "Task_%d", id);
  snprintf(my_mailbox, max_string_size - 1, "worker-%d", id);

  XBT_INFO("worker-%d: hi!", id);
  XBT_INFO("worker-%d: there are %d like me", id, count);
  for (int i = 0; i < count; ++i) {
    if (i != id) {
      XBT_INFO("worker-%d: I will send a message to '%d'", id, i);
      msg_task_t task = MSG_task_create(task_name, comp_size, comm_size, NULL);
      snprintf(mailbox, max_string_size - 1, "worker-%d", i); /* - Select a @ref worker in a round-robin way */
      MSG_task_isend(task, mailbox);
    }
  }
  msg_task_t *tasks = xbt_new(msg_task_t, count);
  xbt_dynar_t comms = xbt_dynar_new(sizeof(msg_comm_t), NULL);
  for (int i = 0; i < count; ++i) {
    if (i != id) {
      XBT_INFO("worker-%d: I'll wait for a message from '%d'", id, i);
      tasks[i] = NULL;
      msg_comm_t comm = MSG_task_irecv(&tasks[i], my_mailbox);
      xbt_dynar_push_as(comms, msg_comm_t, comm);
    }
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
