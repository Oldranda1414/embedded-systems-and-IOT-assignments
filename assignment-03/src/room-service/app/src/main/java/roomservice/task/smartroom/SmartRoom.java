package roomservice.task.smartroom;

import java.util.NavigableMap;
import java.util.Optional;

import org.apache.commons.lang3.tuple.Pair;


public interface SmartRoom {

    void setState(final Pair<Boolean, Integer> currState, final int priorityLevel);

    Pair<Boolean, Integer> getCurrState();

    NavigableMap<String, Pair<Boolean, Integer>> getHistory();

    String getHistoryAsJsonString();
    
}
