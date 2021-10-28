package progetto.arduinomqttarcipretechiavetta.arduinomqtt.Controller;


import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttAsyncClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;
import progetto.arduinomqttarcipretechiavetta.arduinomqtt.MyCallback;

import java.util.UUID;

@CrossOrigin(exposedHeaders="Access-Control-Allow-Origin")
@Controller
public class ControlloRemoto {
    public static MqttAsyncClient myClient;
    @PostMapping(path="accendiLed/{led}")
    public @ResponseBody String accendiLed(@PathVariable String led) throws MqttException {
        String s = "Accendi Led " + led;
        mqttConnect().publish("casa/piano1/"+led+"/accendiled", s.getBytes(),0, true);
        return "Messaggio di accensione Led pubblicato con successo";
    }

    @PostMapping(path="spegniLed/{led}")
    public @ResponseBody String spegniLed(@PathVariable String led) throws MqttException {
        String s = "Spegni Led " + led;
        mqttConnect().publish("casa/piano1/"+led+"/spegniled", s.getBytes(),0, true);
        return "Messaggio di spegnimento Led pubblicato con successo";
    }

    @PostMapping(path="spegniAllarme/{allarme}")
    public @ResponseBody String spegniAllarme(@PathVariable String allarme) throws MqttException {
        String s = "Spegni Allarme" + allarme;
        mqttConnect().publish("casa/piano1/"+allarme+"/spegniAllarme", s.getBytes(),2, true);
        return "Messaggio di spegnimento Allarme pubblicato con successo";
    }

    public MqttAsyncClient mqttConnect() throws MqttException {
        myClient = new MqttAsyncClient("tcp://broker.hivemq.com:1883", UUID.randomUUID().toString());
        MyCallback myCallback = new MyCallback();
        myClient.setCallback(myCallback);
        IMqttToken token = myClient.connect();
        token.waitForCompletion();
        return myClient;
    }


}
