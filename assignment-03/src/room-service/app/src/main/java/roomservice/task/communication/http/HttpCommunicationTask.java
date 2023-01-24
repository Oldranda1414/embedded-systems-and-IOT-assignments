package roomservice.task.communication.http;

import io.vertx.core.Vertx;
import io.vertx.core.http.HttpMethod;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.handler.BodyHandler;

import io.vertx.ext.web.handler.CorsHandler;
import org.apache.commons.lang3.tuple.Pair;
import roomservice.task.smartroom.SmartRoom;
import roomservice.task.Task;

import java.util.Optional;

public class HttpCommunicationTask implements Task {

    final SmartRoom room;
    private final int priority;
    private Optional<Pair<Boolean, Integer>> localState;

    public HttpCommunicationTask(final SmartRoom room, final int priorityLevel) throws RuntimeException {
        this.room = room;
        this.priority = priorityLevel;
        this.localState = Optional.empty();

        final Vertx vertx = Vertx.vertx();
        final Router router = Router.router(vertx);
        router.route()
                .handler(CorsHandler.create("*")
                    .allowedMethod(HttpMethod.GET)
                    .allowedMethod(HttpMethod.POST)
                    .allowedMethod(HttpMethod.PUT)
                    .allowedMethod(HttpMethod.OPTIONS)
                    .allowCredentials(true)
                    .allowedHeader("Access-Control-Allow-Method")
                    .allowedHeader("Access-Control-Allow-Origin")
                    .allowedHeader("Access-Control-Allow-Credentials")
                    .allowedHeader("Content-Type"))
                .handler(BodyHandler.create())
                .handler(req -> {
                    var params = req.request().params();
                    if (params.contains("light") && params.contains("rollerBlind")) {
                        var light = params.get("light");
                        if (light.equals("ON") || light.equals("OFF")) {
                            try {
                                this.setLocalState(Optional.of(
                                        Pair.of(light.equals("ON"), Integer.parseInt(params.get("rollerBlind")))));
                            } catch (Exception ignored) {}
                        }
                    }
                    req.json(new JsonObject(this.room.getHistoryAsJsonString()));
                });

        // Create the HTTP server
        vertx.createHttpServer()
                // Handle every request
                .requestHandler(router)
                // Start listening
                .listen(8088)
                // Print the port
                .onSuccess(server -> System.out.println("HTTP on http://localhost:" + server.actualPort() + "\n--------------------\n"))
                .onFailure(server -> {
                    throw new RuntimeException("Unable to create Http-Communication Task");
                });
    }

    @Override
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
