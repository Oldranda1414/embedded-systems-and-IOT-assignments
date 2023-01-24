package roomservice.task.communication.mqtt;

import java.util.Optional;

import io.vertx.core.Vertx;
import io.vertx.core.json.JsonObject;
import io.vertx.mqtt.MqttClient;
import io.vertx.mqtt.MqttClientOptions;
import io.vertx.mqtt.MqttServer;

import io.vertx.mqtt.MqttTopicSubscription;
import org.apache.commons.lang3.tuple.Pair;
import roomservice.task.smartroom.SmartRoom;
import roomservice.task.Task;


public class MqttCommunicationTask implements Task {

	final SmartRoom room;
	private final int priority;
	private final String mqtt_server = "broker.mqtt-dashboard.com";
	private final String topic = "Leonardo";
    private final int port = 1883;
    private Optional<Pair<Boolean, Integer>> localState;

    public MqttCommunicationTask(final SmartRoom room, final int priorityLevel) {
        this.room = room;
        this.priority = priorityLevel;
        this.localState = Optional.empty();

        Vertx vertx = Vertx.vertx();
        MqttClient mqttClient = MqttClient.create(vertx, new MqttClientOptions()
                .setAutoKeepAlive(true)
                .setClientId("room-service")
        );
        mqttClient.connect(this.port, this.mqtt_server, conn -> {
            if (conn.failed()) {
                throw new RuntimeException("Unable to create Mqtt-Communication Task");
            }
            mqttClient.subscribe(this.topic, 0, sub -> {}).publishHandler(message -> {
                try {
                    JsonObject received = new JsonObject(message.payload().toString());
                    boolean light = received.getBoolean("pirState");
                    int rollerBlind = received.getInteger("lumValue");
                    this.setLocalState(Optional.of(
                            Pair.of(light, rollerBlind)));
                } catch (Exception ignored) {}
            });
        });
	}

    public void execute() {
        var state = this.getLocalState();
        if (state.isPresent()) {
            this.room.setState(state.get(), this.priority);
            this.setLocalState(Optional.empty());
        }
    }

    private synchronized void setLocalState(final Optional<Pair<Boolean, Integer>> state) {
        this.localState = state;
    }

    private synchronized Optional<Pair<Boolean, Integer>> getLocalState() {
        return this.localState;
    }

}
