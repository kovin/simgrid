/* Copyright (c) 2006-2017. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

package app.helloworld;
import org.simgrid.msg.Host;
import org.simgrid.msg.HostNotFoundException;
import org.simgrid.msg.Msg;
import org.simgrid.msg.MsgException;
import org.simgrid.msg.Process;
import org.simgrid.msg.Task;

public class Sender extends Process {
	private static final double COMM_SIZE_LAT = 1;
	private String receiverHostname;

	public Sender(String hostname, String name, String receiverHostname) throws HostNotFoundException {
		super(hostname, name);
		this.receiverHostname = receiverHostname;
	}

	public void main(String[] args) throws MsgException {
		Msg.info("Host count: " + args.length);
		String hostname = Host.getByName(receiverHostname).getName(); // Make sure that this host exists
		MyTask task = new MyTask("hello", 0, COMM_SIZE_LAT);
		task.send(hostname);
		task = (MyTask)Task.receive(getHost().getName());
		Msg.info("Soy " + getHost().getName() + " y recibí '" + task.getName() + "'");
		Msg.info("Done.");
	}
}