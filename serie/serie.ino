char datarec;
int pulso = 60;
double velocidad = 10.2;
double carga = 5.0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode (13,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()>0)
  {
    datarec = Serial.read ();
  }
  
  if (datarec == 'P')
  {
    Serial.println (pulso);
    datarec = 0;
  } else if (datarec == 'V')
  {
    Serial.println (velocidad);
    datarec = 0;
  } else if (datarec == 'L')
  {
    Serial.println (carga);
    datarec = 0;
  } 
}
