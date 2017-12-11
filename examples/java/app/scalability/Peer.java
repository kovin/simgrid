package app.scalability;

import org.simgrid.msg.Msg;
import org.simgrid.msg.Comm;
import org.simgrid.msg.Host;
import org.simgrid.msg.MsgException;
import org.simgrid.msg.Process;
import org.simgrid.msg.Task;

public class Peer extends Process {

	public Peer(Host host, String name, String[] args) {
		super(host, name, args);
	}

	@Override
	public void main(String[] args) throws MsgException {
		int id = Integer.parseInt(args[0]);
		int count = Integer.parseInt(args[1]);
		double compSize = 500000000;
	    double commSize = 1000000000;
		Msg.info("Hi, I'm joining the network with id " + id + " and we are " + count);
		String taskName = "Task_" + id;
		String myMailbox = "worker-" + id;
		Msg.info("worker-" + id + ": hi!");
		Msg.info("worker-" + id + ": there are " + count + " like me");
		for (int i = 0; i < count; ++i) {
			if (i != id) {
				Msg.info("worker-" + id + ": I will send a message to '" + i + "'");
				Task task = new Task(taskName, compSize, commSize);
				String mailbox = "worker-" + i;
				task.isend(mailbox);
			}
		}
		Comm[] comms = new Comm[count - 1];
		int waitIndex = 0;
		for (int i = 0; i < count; ++i) {
			if (i != id) {
				Msg.info("worker-" + id + ": I'll wait for a message from '" + i + "'");
				comms[waitIndex] = Task.irecv(myMailbox);
				++waitIndex;
			}
		}
		while (waitIndex > 0) {
			int commIndex = Comm.waitAny(comms);
			Comm comm = comms[commIndex];
			comms = removeCommIndex(comms, commIndex);
			Msg.info("worker-" + id + ": I got comm index: " + commIndex);
			if (comm.test()) {
				Task task = comm.getTask();
				if (task != null) {
					Msg.info("worker-" + id + ": I got task \"" + task.getName() + "\" and I will start processing it");
					task.execute();
					Msg.info("worker-" + id + ": task \"" + task.getName() + "\" is done");
				}
				--waitIndex;
			}
		}
	}

	private Comm[] removeCommIndex(Comm[] comms, int commIndexToRemove) {
		Comm[] ret = new Comm[comms.length - 1];
		int addedSoFar = 0;
		for (int i = 0; i < comms.length; ++i) {
			if (i != commIndexToRemove) {
				ret[addedSoFar] = comms[i];
				++addedSoFar;
			}
		}
		return ret;
	}
}

