#include <Servo.h>

//ambil tempalte_id, device_ide dan token dari aplikasi blynk
#define BLYNK_TEMPLATE_ID "TMPL6dcmuTFH4"
#define BLYNK_TEMPLATE_NAME "Pakan Hamster"
#define BLYNK_AUTH_TOKEN "irdXjhgXPGbWORvCwCuv32lBuPZ7U_4b"

//libary ultrasonic
#define echoPin D7
#define trigPin D6

long duration;
int distance; 
int jarak;

//include libary Blynk
#include <BlynkSimpleEsp8266.h>

//buat sebuah object servo
Servo myservo ;
void JarakPakan()
{
    long duration, distance, jarak;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1; //rumus menghitung sensor ultrasonic
    jarak = (15-distance);
    Serial.print("Distance: ");
    Serial.println(jarak);
    Serial.println(" %");
    Blynk.virtualWrite(V1, jarak);
    delay(500);
}

//variable menampung datastream statuspakan
int StatusPakan;

void setup() {
  //pin yang digunakan untuk servo = D2 = 4
  myservo.attach(4);
  //posisi servo awal
  myservo.write(0);

  Serial.begin(115200);
  //Ultra
  pinMode(34, INPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 

  //koneksi ke Wifi
  WiFi.begin("hamsterku","12345678");
  //uji konek atau tidak
  while(WiFi.status() !=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("WiFi Terkoneksi");

  //apabila terkoneksi, koneksikan perangkat ke blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, "hamsterku", "12345678");

  Serial.println("Blynk Terkoneksi");
}

void BeriPakan()
{
      //proses perputaran pakan
  for(int posisi=0; posisi<=90; posisi++)
  {
    // set posisi servo
    myservo.write(posisi);
    delay(10);
  }
  //proses perputaran t4 pakan ke posisi awal
  for(int posisi=90; posisi>=0; posisi--)
  {
    myservo.write(posisi);
    delay(10);
  }
}
void loop() {
  //jalankan aplikasi Blynk dari nodeMCU
  Blynk.run();
  JarakPakan();
  //tampilkan StatusPakan diserial monitor
  Serial.println("Status Pakan : " + String(StatusPakan));

  //jika StatusPakan = 1 maka diberi pakan
  if(StatusPakan == 1)
  {
    //panggil void beripakan, 1 Siklus perputaran Sampai kembali keposisi awal
    BeriPakan();
    //kembalikan StatusPakan menjadi 0 setelah diberipakan
    Blynk.virtualWrite(V0, 0);
    StatusPakan = 0;
    delay(2000);
  }

}

//baca datastream StatusPakan
BLYNK_WRITE(V0)
{
  //baca datastream kemudian tampung kedalam statuspakan
  StatusPakan = param.asInt();
}
//baca datastream ultrasonic
BLYNK_WRITE(V1)
{
  jarak = param.asInt();
}
