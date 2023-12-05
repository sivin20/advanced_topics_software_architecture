package dk.grp9.faulthandler.consumer;

import org.eclipse.paho.mqttv5.client.MqttConnectionOptions;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.annotation.Bean;
import org.springframework.integration.annotation.ServiceActivator;
import org.springframework.integration.channel.DirectChannel;
import org.springframework.integration.core.MessageProducer;
import org.springframework.integration.dsl.IntegrationFlow;
import org.springframework.integration.mqtt.inbound.Mqttv5PahoMessageDrivenChannelAdapter;
import org.springframework.messaging.Message;
import org.springframework.messaging.MessageChannel;
import org.springframework.messaging.MessageHandler;
import org.springframework.messaging.MessagingException;

public class FaultConsumer {

    private final String mqttAddr = "tcp://mosquitto:1883";
    private final String clientID = "faultHandler";
    private final String mainTopic = "faultTopic";
    private final Logger LOGGER = LoggerFactory.getLogger(FaultConsumer.class);

    @Bean
    public IntegrationFlow inbound() {
        LOGGER.info("ABBB");
        Mqttv5PahoMessageDrivenChannelAdapter adapter =
                new Mqttv5PahoMessageDrivenChannelAdapter(mqttAddr, clientID, mainTopic);
        adapter.setPayloadType(String.class);
        adapter.setManualAcks(true);

        return IntegrationFlow.from(adapter)
                .channel(channels -> channels.queue("faultChannel"))
                .handle(message -> handleMessage(message))
                .get();
    }

    @Bean
    public MessageChannel mqttInputChannel() {
        return new DirectChannel();
    }

    @Bean
    @ServiceActivator(inputChannel = "mqttInputChannel")
    public MessageHandler handler() {
        return message -> System.out.println(message.getPayload() );
    }

    private void handleMessage(Message<?> message) {
            // Your message processing logic here
        LOGGER.info("Yabba");
        System.out.println("HAHAHA");
            System.out.println("Received message: " + message.getPayload());
    }
}
