package progetto.arduinomqttarcipretechiavetta.arduinomqtt;

import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttAsyncClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

import java.util.UUID;

@SpringBootApplication
public class ArduinoMqttApplication {
    public static MqttAsyncClient myClient;
    public static void main(String[] args) throws MqttException {
        SpringApplication.run(ArduinoMqttApplication.class, args);
        myClient = new MqttAsyncClient("tcp://broker.hivemq.com:1883", UUID.randomUUID().toString());
        MyCallback myCallback = new MyCallback();
        myClient.setCallback(myCallback);
        IMqttToken token = myClient.connect();
        token.waitForCompletion();
        myClient.subscribe("casa/piano1/cameradaletto/alerttemperatura",2);
        myClient.subscribe("casa/piano1/soggiorno/alertfumo",2);
    }

}
