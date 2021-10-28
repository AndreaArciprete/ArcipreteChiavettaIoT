package progetto.arduinomqttarcipretechiavetta.arduinomqtt.Utility;

public class StatoLed {
    private String ledSoggiorno;
    private String ledCameraDaLetto;

    public StatoLed(String ledSoggiorno, String ledCameraDaLetto) {
        this.ledSoggiorno = ledSoggiorno;
        this.ledCameraDaLetto = ledCameraDaLetto;
    }

    public String getLedSoggiorno() {
        return ledSoggiorno;
    }

    public StatoLed setLedSoggiorno(String ledSoggiorno) {
        this.ledSoggiorno = ledSoggiorno;
        return this;
    }

    public String getLedCameraDaLetto() {
        return ledCameraDaLetto;
    }

    public StatoLed setLedCameraDaLetto(String ledCameraDaLetto) {
        this.ledCameraDaLetto = ledCameraDaLetto;
        return this;
    }
}
