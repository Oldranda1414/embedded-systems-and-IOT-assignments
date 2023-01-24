package roomservice.scheduler;

import java.util.LinkedHashSet;
import java.util.Set;
import java.util.SortedSet;

import roomservice.task.Task;

public class Scheduler {

    final private LinkedHashSet<Task> taskSet;

    public Scheduler(final LinkedHashSet<Task> taskSet) {
        this.taskSet = new LinkedHashSet<>(taskSet);
    }

    public void schedule() {
        while (true) {
            this.taskSet.forEach(Task::execute);
            try {
                Thread.sleep(1000);
            } catch (InterruptedException ignored) {}
        }
    }

}
