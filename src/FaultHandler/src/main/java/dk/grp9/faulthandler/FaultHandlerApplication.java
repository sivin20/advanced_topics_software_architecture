package dk.grp9.faulthandler;


import dk.grp9.faulthandler.consumer.FaultConsumer;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import org.springframework.integration.annotation.ServiceActivator;
import org.springframework.integration.channel.DirectChannel;
import org.springframework.integration.dsl.IntegrationFlow;
import org.springframework.integration.mqtt.inbound.Mqttv5PahoMessageDrivenChannelAdapter;
import org.springframework.messaging.Message;
import org.springframework.messaging.MessageChannel;
import org.springframework.messaging.MessageHandler;

@SpringBootApplication
public class FaultHandlerApplication {
    public static void main(String[] args) {
        SpringApplication.run(FaultHandlerApplication.class, args);
    }


    private final String mqttAddr = "tcp://mosquitto:1883";
    private final String clientID = "faultHandler";
    private final String mainTopic = "fault_topic";

    @Bean
    public IntegrationFlow inbound() {
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

//    @Bean
//    @ServiceActivator(inputChannel = "mqttInputChannel")
//    public MessageHandler handler() {
//        return message -> System.out.println(message.getPayload() );
//    }

    private void handleMessage(Message<?> message) {
        // Your message processing logic here
        System.out.println("Received message: " +  new String((byte[]) message.getPayload()));
    }
}
