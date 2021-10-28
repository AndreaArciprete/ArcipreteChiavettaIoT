package progetto.arduinomqttarcipretechiavetta.arduinomqtt.Controller;

import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttAsyncClient;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;
import progetto.arduinomqttarcipretechiavetta.arduinomqtt.MyCallback;
import progetto.arduinomqttarcipretechiavetta.arduinomqtt.Utility.StatoLed;
import java.util.UUID;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static org.awaitility.Awaitility.await;


@CrossOrigin(exposedHeaders="Access-Control-Allow-Origin")
@Controller
public class AcquisisciGrandezze {
    private static MqttAsyncClient myClient;
    private StatoLed statoled;
    private String valore_grandezza;
    private static MqttClient myClientSync;
    private CountDownLatch processingFinishedLatch; /*Utilizziamo questo oggetto per fare in modo che il metodo invocato dal Client ritorni una
    risposta soltanto se è stata ricevuta un messaggio dal Broker Mqtt.
    In pratica, utilizzando questo oggetto riusciamo a fare in modo che al Client la risposta arriva soltanto se è stato ricevuto un messaggio dal Broker.
    Ad esempio, utilizzando questo oggetto, il metodo isLedAcceso verrà bloccato, prima di effettuare il return, fino a quando non riceve una risposta dal Broker.
    */
    @GetMapping(path="isledaccesi")
    public @ResponseBody StatoLed isLedAcceso() throws MqttException, InterruptedException {
        statoled = new StatoLed("","");
        processingFinishedLatch = new CountDownLatch(1); /*Inizializziamo il contatore a 1. Esso verrà decrementato di 1 e dunque diventerà 0 non appena
        verrà ricevuta una risposta da parte del Broker.
        */
        String s = "Ottieni stato led della casa";
        mqttConnect().publish("casa/piano1/led/isaccesi",s.getBytes() ,0, false);
        mqttConnectSync().subscribe("casa/piano1/statoledSoggiorno", (topic, message) -> {
            String risultato = new String(message.getPayload());
            if (risultato.equals("On")){
                statoled.setLedSoggiorno("On");
            }
            else {
                statoled.setLedSoggiorno("Off");
            }
            this.processingFinishedLatch.countDown(); /*Non appena viene ricevuto un messaggio dal Broker il contatore viene decrementato di 1 in maniera
            tale da diventare 0.
            */
        });
        mqttConnectSync().subscribe("casa/piano1/statoledCameraDaLetto", (topic, message) -> {
            String risultato1 = new String(message.getPayload());
            if (risultato1.equals("On")){
                statoled.setLedCameraDaLetto("On");
            }
            else {
                statoled.setLedCameraDaLetto("Off");
            }
            this.processingFinishedLatch.countDown();
        });
        this.processingFinishedLatch.await(); /*Così facendo, prima che il metodo possa effettuare il return, viene bloccato fino a quando il
        contatore non diventa pari a 0. Non appena il contatore diventa pari a 0 significa che il messaggio proveniente dal Broker è statp ricevuto correttamente
        e dunque è possibile restituire la risposta al Client.
        */
        return statoled;
    }

    @GetMapping(path="ottieni/{grandezza}")
    public @ResponseBody String ottieniGrandezze(@PathVariable String grandezza) throws MqttException, InterruptedException {
        valore_grandezza = null;
        processingFinishedLatch = new CountDownLatch(1);
        String s = "Ottieni "+ grandezza;
        if(grandezza.equals("temperatura") || grandezza.equals("umidita")) {
            mqttConnect().publish("casa/piano1/cameradaletto/richiedi/" + grandezza, s.getBytes(), 0, false);
            mqttConnectSync().subscribe("casa/piano1/cameradaletto/ottieni/" + grandezza, (topic, message) -> {
                valore_grandezza = new String(message.getPayload());
                this.processingFinishedLatch.countDown();
            });
        }
        if(grandezza.equals("qualitaaria")) {
            mqttConnect().publish("casa/piano1/soggiorno/richiedi/" + grandezza, s.getBytes(), 0, false);
            mqttConnectSync().subscribe("casa/piano1/soggiorno/ottieni/" + grandezza, (topic, message) -> {
                valore_grandezza = new String(message.getPayload());
                this.processingFinishedLatch.countDown();
            });
        }
        this.processingFinishedLatch.await();
        return valore_grandezza;
    }


    public MqttAsyncClient mqttConnect() throws MqttException {
        myClient = new MqttAsyncClient("tcp://broker.hivemq.com:1883", UUID.randomUUID().toString());
        MyCallback myCallback = new MyCallback();
        myClient.setCallback(myCallback);
        IMqttToken token = myClient.connect();
        token.waitForCompletion();
        return myClient;
    }

    public MqttClient mqttConnectSync() throws MqttException {
        myClientSync = new MqttClient("tcp://broker.hivemq.com:1883", UUID.randomUUID().toString());
        MyCallback myCallback = new MyCallback();
        myClientSync.setCallback(myCallback);
        myClientSync.connect();
        return myClientSync;
    }
}
