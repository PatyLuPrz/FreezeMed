#include <ESP8266WiFi.h>

const char* ssid = "IZZI-8207";//Nombre de la red
const char* password = "A811FC188207";//contraseña

IPAddress ip(192, 168, 0, 25);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server (80);

void setup() {
    Serial.begin(9600);
    delay(10);
    //configuracion del GPIO2
    pinMode(2, OUTPUT);//asignacion de pin del puerto de salida
    digitalWrite(2, LOW); //inicializacion del puerto de salida apagado
    pinMode(4, OUTPUT);//asignacion de pin del puerto de salida
    digitalWrite(4, LOW); //inicializacion del puerto de salida apagado
    Serial.println();
    Serial.println();
    Serial.println("concectandose a red : ");
    Serial.println(ssid);
    
    WiFi.mode(WIFI_STA); //Wifi en modo estación
    WiFi.config(ip, gateway, subnet);//Configuramos la ip estática
    WiFi.begin(ssid, password);//Iniciamos conexión con el nombre y la contraseña del wifi que indicamos
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println(".");
    }
    Serial.println( );
    Serial.println("WiFi conectado");
    
    server.begin();
    Serial.println("Servidor iniciado");
    
    Serial.println("Ingrese desde un navegador web usando la siguiente IP:");
    Serial.println(WiFi.localIP());
}

void loop() {
    //put your main code here, to run repeatedly:
    int sensorValue = analogRead(A0);
    float temp = sensorValue * .3226562;
    
    Serial.println("temp=");
    Serial.println(temp);
    
    if (temp > 8) {
        //Serial.println("Arranca el sistema de refrigeracion");
        digitalWrite(4, HIGH);
    } else if (temp = 2) {
        //Serial.println("Para el sistema de refrigeracion");
        digitalWrite(4, LOW);
    }
    
    delay(1000);
    WiFiClient client = server.available();
    if (client) {
        Serial.println("nuevo cliente");
        // leemos la primera linea de la peticion del cliente.
        while (!client.available() && client.connected()) {
            delay(1);
        }
        
        String linea1 = client.readStringUntil('r');
        Serial.println("########################");
        Serial.println(linea1);
        if (linea1.indexOf("Med=ON") > 0) {
            digitalWrite(2, HIGH);
        } else {
            if (linea1.indexOf("Med=OFF") > 0) {
                digitalWrite(2, LOW);
            }
        }
        
        client.flush();
        Serial.println("Enviando respuesta.....");
        //Encabezado http
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");//la conexion se cierra despues de finalizacion
        client.println();
        //pagina html para el navegador
        client.println("<!doctype html><html lang='es'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><link href='https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3' crossorigin='anonymous'><link rel='preconnect' href='https://fonts.googleapis.com'><link rel='preconnect' href='https://fonts.gstatic.com' crossorigin><link href='https://fonts.googleapis.com/css2?family=Montserrat&family=Poppins:wght@300;400;600&display=swap' rel='stylesheet'><style>body {font-family: 'Poppins', sans-serif;}</style><title>FreezeMed</title></head><body><nav class='navbar navbar-expand-lg navbar-light bg-light'><div class='container'><a class='navbar-brand' href='#'><img src='https://raw.githubusercontent.com/PatyLuPrz/FreezeMed/main/assets/images/logo(5).png' alt='' width='200' class='d-inline-block align-text-top'></a><button class='navbar-toggler' type='button' data-bs-toggle='collapse' data-bs-target='#navbarNavAltMarkup' aria-controls='navbarNavAltMarkup' aria-expanded='false' aria-label='Toggle navigation'><span class='navbar-toggler-icon'></span></button><div class='collapse navbar-collapse justify-content-end' id='navbarNavAltMarkup'><div class='navbar-nav text-end'><a class='nav-link active' aria-current='page' href='#'>Inicio</a><a class='nav-link' href='#'>Ayuda</a></div></div></div></nav><div class='container'><h1 class='mt-5'>Bienvenido de nuevo</h1><h5 class=''>FreezeMed - Panel de control</h5><div class='row'><div class='col-md-6 col-sm-12'><div class='card mt-md-5 mt-4'><div class='card-body'><h5 class='card-title'>Temperatura actual</h5><p class='card-text mb-3'>");
        client.println(temp);
        client.println("</p></div></div></div><div class='col-md-6 col-sm-12 mt-md-5 mt-2'><div class='card'><div class='card-body'><h5 class='card-title'>Controles</h5><div class='row'><div class='col-6'><a href='./?Med=ON' style='width: 100%;' class='btn btn-success'>Prender</a></div> <div class='col-6'><a href='./?Med=OFF' style='width: 100%;' class='btn btn-danger'>Apagar</a></div></div></div></div></div></div></div></body><script src='https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js' integrity='sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p' crossorigin='anonymous'></script><script type='text/javascript'>function actualizar() { location.reload(true); }setInterval('actualizar()', 1000);</script></html>");
        delay(10);
        Serial.println("Respuesta enviada");
        Serial.println();
    }
}