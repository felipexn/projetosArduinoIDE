/-----------------------------------------------------------------------------------------------------------------------------------------
// Área de Inclusão de arquivos e funções 
//#include <esp32Servo.h>
#include <Arduino.h>                                                     // Para usar a framework do Arduino
#include <ESP32Servo.h>
#include <Keypad.h>
#include <WiFi.h>                                                       // Para acesso ao wifi
#include <HTTPClient.h>                                                 // Para criar um cliente HTTP
#include <UrlEncode.h>                                                  // Para enviar a mensagem via post deve ser no formato URL

void blinkVermelho(void *pvparameter);

TaskHandle_t tarefa1; 
//-----------------------------------------------------------------------------------------------------------------------------------------
// Definições de Hardware
Servo meuServo;
const int servoPin = 18;
const int ledAmareloPin = 17;
const int ledVerdePin = 16;
const int ledVermelhoPin = 4;
int a=0;
const byte LINHAS = 4; 
const byte COLUNAS = 3; 
char keys[LINHAS][COLUNAS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pinosLinha[LINHAS] = {12, 26, 27, 14};
byte pinosColuna[COLUNAS] = {33, 25, 32};
Keypad teclado = Keypad( makeKeymap(keys), pinosLinha, pinosColuna, LINHAS, COLUNAS );

String senhaCorreta = "123"; // Senha correta

#define Led1    15                                                      // Led1 ligado ao GPIO 15

//-----------------------------------------------------------------------------------------------------------------------------------------
// Protótipos de função

void WhatsAppMessages(String message);                                  // Função que envia uma mensagem para o whatsapp

//-----------------------------------------------------------------------------------------------------------------------------------------
// Dados da sua WiFi.

const char* SSID = "rede";                           // Nome da rede wifi. Identificador da rede
const char* PASSWORD = "senha";                         // Senha da rede wifi. Sua senha do Wifi

//-----------------------------------------------------------------------------------------------------------------------------------------
// Dados do celular e chave do whatsapp

// bool flag = 0;                                                          // Flag para uso na logica do sensor
String phoneNumber = "seu numero aqui";                                  // Meu número de celular com formato internacional
String apiKey = "apikeyaqui";                                               // A chave que o bot do whatsapp enviou

//-----------------------------------------------------------------------------------------------------------------------------------------

void setup() 
{
  xTaskCreatePinnedToCore (
  blinkVermelho, "tarefa1",
  10000,
  NULL,
  0,
  &tarefa1,
  1);
   
  meuServo.attach(servoPin);
  pinMode(ledAmareloPin, OUTPUT);
  pinMode(ledVerdePin, OUTPUT);
  pinMode(ledVermelhoPin, OUTPUT);
  digitalWrite(ledAmareloPin, HIGH);
  meuServo.write(5);
  pinMode(32, OUTPUT);
  pinMode(Led1, OUTPUT);                                              // Configura pino do led1 como saida
  Serial.begin(115200);                                               // Inicia a serial com 115200 bps
  WiFi.begin(SSID, PASSWORD);                                         // Inicia o wifi com o nome da rede e a senha
  Serial.println("Conectando");                                       // Envia pela serial que esta conectando
  //Serial.print(a);

  while(WiFi.status() != WL_CONNECTED)                                // Aguarda a conexão
    {
        delay(500);                                                     // Espera 500ms
        Serial.print(".");                                              // Vai escrevendo ...
    }
    Serial.println("");                                                 // Pula uma linha
    Serial.print("Conectado no IP: ");                                  // Mostra que esta conectado no IP
    Serial.println(WiFi.localIP());                                     // Mostra o IP no qual foi conectado

   // WhatsAppMessages("Testando 123 WhatsApp Bot !");                    // Envia mensagem para o bot do whatsapp
}

//-----------------------------------------------------------------------------------------------------------------------------------------

void loop() 
{
   char key = teclado.getKey();
    if(key) {
       Serial.println(key); 
   float sinVal = (sin(100*(3.1416/180)));
   float sinVal2 = (sin(random(100)*(3.1416/180)));
   
  int toneVal = 2000+(int(sinVal*1000));
  int toneVal2 = 800+(int(sinVal*500));
  int toneVal3 = 8000+(int(sinVal2*5000));
  
  static String entradaUsuario = "";
  delay(234);
  entradaUsuario += key;

  
  if (entradaUsuario.length() == senhaCorreta.length()) {
 if (entradaUsuario == senhaCorreta) {
     Serial.print("tudo certo!"); 
      digitalWrite(ledAmareloPin, LOW);
      digitalWrite(ledVerdePin, HIGH);
      tone(2, toneVal);
      delay(1000);
      noTone(2);
      meuServo.write(95); // Gira o servo para 90 graus
      delay(5000);
      tone(2, toneVal);
      delay(500);
      noTone(2);
      delay(500);
      tone(2, toneVal);
      delay(500);
      noTone(2);
      delay(500);
      tone(2, toneVal);
      delay(500);
      noTone(2);
      meuServo.write(5); // Retorna o servo para a posição inicial
      delay(500);
      entradaUsuario = "";
      digitalWrite(ledAmareloPin, HIGH);
      digitalWrite(ledVerdePin, LOW);
      a = 0;
      
    } else {

      a++;
      Serial.print("N° de erros:");
      Serial.println(a);
       
      digitalWrite(ledVermelhoPin, HIGH);
      digitalWrite(ledAmareloPin, LOW);
      
      tone(2, toneVal2);
      delay(1000);
      noTone(2);
      
      digitalWrite(ledVermelhoPin, LOW);
      entradaUsuario = "";
      digitalWrite(ledAmareloPin, HIGH);
      
    }

    if(a==3){
      digitalWrite(Led1, HIGH);
        WhatsAppMessages("Houve uma tentativa de invasão em sua residência");                                                 // Liga o led1
        Serial.println("O proprietario foi notificado da sua Presença !");                          // Envia mensagem na serial que deu certo
        delay(1000);                                                            // Espera 1s
        digitalWrite(Led1, LOW);
        a = 0;
    }
  }                                              
    
    
    }
}



void WhatsAppMessages(String message){


    // Dados a serem enviados pelo método Post suportado pelo protocolo HTTP
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
    
    HTTPClient http;                                                            // Cria um objeto HTTP
    http.begin(url);                                                            // Inicia o objeto HTTP passando a string acima
    
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");        // Adiciona um cabeçalho
  
    uint16_t httpResponseCode = http.POST(url);                                 // Envia o método POST HTTP de requisição e lê o resultado
  
    if (httpResponseCode == 200)                                                // Deu tudo certo?
    {
        digitalWrite(Led1, HIGH);                                               // Liga o led1
        Serial.print("Mensagem enviada com sucesso!");                          // Envia mensagem na serial que deu certo
        delay(1000);                                                            // Espera 1s
        digitalWrite(Led1, LOW);                                                // Desliga o led1
    }
    else                                                                        // Caso contrário, se não foi possivel enviar a mensagem
    {
        Serial.println("Erro ao tentar enviar a mensagem!");                    // Envia para a serial a mensagem de erro
        Serial.print("Código HTTP: ");                                          // Vai enviar para a serial o código de erro que retornou
        Serial.println(httpResponseCode);                                       // Envia o código de erro
    }
    http.end();                                                                 // Finaliza o objeto http
}


void blinkVermelho(void *pvparameter){

  
   for(;;){ //evitar a reinicilização do WDT
     float sinVal = (sin(100*(3.1416/180)));
   float sinVal2 = (sin(random(100)*(3.1416/180)));
   
  int toneVal = 2000+(int(sinVal*1000));
  int toneVal2 = 800+(int(sinVal*500));
  int toneVal3 = 8000+(int(sinVal2*5000));
  char key = teclado.getKey();
    if(key){
tone(2,toneVal3);
  delay(234);
  noTone(2);
    }
   }
  }
