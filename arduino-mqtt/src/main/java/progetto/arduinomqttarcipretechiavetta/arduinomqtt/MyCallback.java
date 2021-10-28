package progetto.arduinomqttarcipretechiavetta.arduinomqtt;

import org.eclipse.paho.client.mqttv3.*;

import java.util.UUID;

public class MyCallback implements MqttCallback {
    public static MqttAsyncClient myClient;
    public void connectionLost(Throwable arg0) {

    }

    public void deliveryComplete(IMqttDeliveryToken arg0) {

    }

    public void messageArrived(String topic, MqttMessage message) throws Exception {
        myClient = new MqttAsyncClient("tcp://broker.hivemq.com:1883", UUID.randomUUID().toString());
        MyCallback myCallback = new MyCallback();
        myClient.setCallback(myCallback);
        IMqttToken token = myClient.connect();
        token.waitForCompletion();
        if (topic.equals("casa/piano1/cameradaletto/alerttemperatura")){
            String s = "Allarme temperatura";
            myClient.publish("casa/piano1/cameradaletto/allarmetemperatura",s.getBytes(),2,false);
        }
        if (topic.equals("casa/piano1/soggiorno/alertfumo")){
            String s = "Allarme fumo";
            myClient.publish("casa/piano1/soggiorno/allarmefumo",s.getBytes(),2,false);
        }
    }
}
