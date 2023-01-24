package roomservice.task.smartroom;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.*;

import org.apache.commons.lang3.tuple.Pair;

import roomservice.task.Task;

public class SmartRoomImpl implements SmartRoom, Task {

    // Does not manage list-full
    private final NavigableMap<String, Pair<Boolean, Integer>> dateHourHistory;
    private Pair<Boolean, Integer> currState;
    private int lastPriorityLevel;
    private boolean lastPersonOut = false;

    public SmartRoomImpl() {
        this.dateHourHistory = new TreeMap<>();
        this.currState = Pair.of(false, 100);
        this.execute();
    }

    @Override
    public synchronized void setState(Pair<Boolean, Integer> state, final int priorityLevel) {
        if (priorityLevel < this.lastPriorityLevel)
            return;

        if (this.currState.getKey() == state.getKey() && this.currState.getValue() == state.getValue())
            return;

        // Room-Sensor
        if (priorityLevel == 0) {
            int hour = Calendar.getInstance().get(Calendar.HOUR_OF_DAY);

            // After 19, if nobody is in the room
            if (hour < 8 && hour >= 19) {
                if (!this.lastPersonOut) {
                    lastPersonOut = !state.getKey();
                } else {
                    state = Pair.of(state.getKey(), 100);
                }
            } else {
                this.lastPersonOut = false;
            }
        }
        this.lastPriorityLevel = priorityLevel;
        this.currState = state;
    }

    public synchronized Pair<Boolean, Integer> getCurrState() {
        return this.currState;
    }

    @Override
    public synchronized NavigableMap<String, Pair<Boolean, Integer>> getHistory() {
        return new TreeMap<>(this.dateHourHistory);
    }

    @Override
    public String getHistoryAsJsonString() {
        var json = new StringBuilder("{");
        var sj = new StringJoiner(",");
        this.getHistory().forEach((k, v) -> sj.add("\"" + k + "\":" + "{" + "\"light\":" + "\"" + (v.getKey() ? "ON" : "OFF") + "\"" + "," + "\"rollerBlind\":" + v.getValue() + "}"));
        json.append(sj).append("}");
        return json.toString();
    }

    @Override
    public void execute() {
        this.lastPriorityLevel = 0;
        var state = this.currState;
        this.addToHistory(DateTimeFormatter.ofPattern("yyyy-MM-dd'T'HH:mm:ss.SSS").format(LocalDateTime.now()), state);
    }

    private synchronized void addToHistory(final String time, final Pair<Boolean, Integer> state) {
        this.dateHourHistory.put(time, state);
    }

}
