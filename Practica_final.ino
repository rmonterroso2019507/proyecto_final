/*
 * roberto monterroso
 * 2019507
 * EB5AM
 * Centro Educativo Tecnico Laboral KINAL
 * Electronica nivel Tecno
 */
#include <SoftwareSerial.h>

const int MED1 = 7;// Pin positivo
const int MED2 = 6;// Pin negativo
int valor;// Variable que guarda el valor raw (0 a 1023)
float voltaje;// Variable que guarda el voltaje (0.0 a 25.0)
float Sensibilidad = 0.185;//variable que guarda en decimales el valor a dar
const int Pin_A0 = A0;// Pin analógico para medir la tensión
const float Resistencia_evaluada = 1000.0;// Resistencia conocida en ohmios (1k ohm)

SoftwareSerial BT(7,6);

unsigned long Ultimo_Tiempo = 0;
const unsigned long Tiempo_de_Medicion = 1000;// Intervalo de medición en milisegundos

void setup() {
  pinMode(MED1, OUTPUT);// Configurar el pin positivo como salida
  pinMode(MED2, OUTPUT);// Configurar el pin negativo como salida
  Serial.begin(9600);// Iniciar la comunicación serial
  BT.begin(9600);
}

char recep;

void loop() {
    // Verificar si hay datos disponibles desde la aplicación Bluetooth
    if (BT.available()) {
    recep = BT.read();
    Serial.println(recep);
    // Procesar comandos según corresponda
    if (recep == 'A') {
      medirVoltaje();
    }
    else if (recep == 'B') {
      medirResistencia();
    }
    else if (recep == 'C') {
      medirCorriente();
    }
  }
  // Realizar mediciones continuas a intervalos regulares
  unsigned long tiempoActual = millis();
  if (tiempoActual - Ultimo_Tiempo >= Tiempo_de_Medicion) 
  {
    Ultimo_Tiempo = tiempoActual;// Realizar la medición que desees aquí
  }
}

void medirVoltaje() {
  digitalWrite(MED1, HIGH);
  digitalWrite(MED2, HIGH);
  valor = analogRead(A1);// Realizar la lectura
  voltaje = map(valor, 0, 1023, 0, 250) / 10.0;  // Escalar a 0.0 - 25.0
  Serial.print("Voltaje: ");
  Serial.println(voltaje);// Mostrar el valor por serial
  BT.print(voltaje);
  BT.print(";");
}

void medirResistencia() {
  digitalWrite(MED1, LOW);
  digitalWrite(MED2, LOW);
  int valorAnalogico = analogRead(Pin_A0);
  // Convertir el valor analógico a tensión (en voltios)
  float tensionMedida = (float)valorAnalogico * (5.0 / 1023.0);
  // Calcular la corriente (I) usando la ley de Ohm: I = V / R
  float corriente = tensionMedida / Resistencia_evaluada;
  // Calcular la resistencia desconocida usando la ley de Ohm: R = V / I
  float resistenciaDesconocida = tensionMedida / corriente;
  Serial.print("Resistencia: ");
  Serial.println(resistenciaDesconocida);
  BT.print(resistenciaDesconocida);
  BT.print(";");
}

void medirCorriente() {
  float Corriente=calculo(500);
      Serial.print("Corriente: ");
      Serial.println(Corriente,3);
      BT.print(Corriente,3);
      BT.print(";");
}
float calculo(int valorIndicado)
{
float Sensor  = 0;
float Intensidad = 0;
for(int i=0;i<valorIndicado;i++)
{
  Sensor = analogRead(A2) * (5.0 / 1023.0);
  Intensidad=Intensidad+(Sensor -2.5)/Sensibilidad;
}
Intensidad=Intensidad/valorIndicado;
return(Intensidad);
}
