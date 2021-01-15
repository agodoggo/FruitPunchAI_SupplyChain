import processing.serial.*;

Serial myPort1;
Serial myPort2;
Serial myPort3;
Serial myPort4;
String val1;
String val2;
String val3;
String val4;

void setup()
{
  //String portName1 = Serial.list()[0];
  String portName2 = Serial.list()[1];
  String portName3 = Serial.list()[2];
  String portName4 = Serial.list()[3];
  print(Serial.list());
  //myPort1 = new Serial(this, portName1, 9600);
  
  myPort2 = new Serial(this, portName2, 9600);
  
  myPort3 = new Serial(this, portName3, 9600);
  
  //myPort4 = new Serial(this, portName4, 9600);
}
void draw()
{
  if ( myPort2.available() > 0 || myPort3.available() > 0)
  {
    //val1 = myPort1.readStringUntil('\n');
    val2 = myPort2.readStringUntil('\n');
    val3 = myPort3.readStringUntil('\n');
    //val4 = myPort4.readStringUntil('\n');
  }
  //println(val1);
  println(val2);
  println(val3);
  //println(val4);
}
