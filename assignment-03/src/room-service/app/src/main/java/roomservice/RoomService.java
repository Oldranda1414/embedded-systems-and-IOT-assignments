package roomservice;

import java.util.*;

import roomservice.scheduler.Scheduler;
import roomservice.task.communication.http.HttpCommunicationTask;
import roomservice.task.communication.mqtt.MqttCommunicationTask;
import roomservice.task.communication.serial.SerialCommunicationTask;
import roomservice.task.smartroom.SmartRoom;
import roomservice.task.smartroom.SmartRoomImpl;
import roomservice.task.Task;

public class RoomService {

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: " + (System.getenv("SHELL") != null ? "./gradlew" : ".\\gradlew.bat") + " run --args=\"<Port name for arduino serial communication>\"");
            System.exit(0);
        }
        System.out.println("""
                --------------------
                Room-Service started
                --------------------""");
        try {
            final Task room = new SmartRoomImpl();
            final Task mqttCommunicationTask = new MqttCommunicationTask((SmartRoom) room, 0);
            final Task httpCommunicationTask = new HttpCommunicationTask((SmartRoom) room, 1);
            final Task serialCommunicationTask = new SerialCommunicationTask(args[0], (SmartRoom) room, 2);

            final Scheduler scheduler = new Scheduler(
                    new LinkedHashSet<>(
                            List.of(
                                mqttCommunicationTask,
                                httpCommunicationTask,
                                serialCommunicationTask,
                                room
                            )
                    )
            );

            scheduler.schedule();
        } catch (Exception e) {
            System.out.println(e.getMessage());
            System.exit(0);
        }
    }

}
