/* Copyright (c) 2006-2017. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

package app.helloworld;
import org.simgrid.msg.Task;

public class MyTask extends Task {
  public MyTask(String name, double computeDuration, double messageSize) {
    super(name,computeDuration,messageSize);
  }
}
