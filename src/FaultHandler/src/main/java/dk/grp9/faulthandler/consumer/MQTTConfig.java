package dk.grp9.faulthandler.consumer;

import org.eclipse.paho.mqttv5.client.MqttConnectionOptions;
import org.springframework.context.annotation.Configuration;

@Configuration
public class MQTTConfig {

    private final String mqttAddr = "tcp://mosquitto:1883";
    private final String clientID = "faultHandler";
    private final String mainTopic = "faultTopic";

    public MqttConnectionOptions mqttConnectOptions() {
        MqttConnectionOptions options = new MqttConnectionOptions();
        options.setServerURIs(new String[]{mqttAddr});
        options.setAutomaticReconnect(true);
        options.setCleanStart(true);
        return options;
    }

}
