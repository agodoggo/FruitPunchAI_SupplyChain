import processing.serial.*;

Serial myPort1;
Serial myPort2;
Serial myPort3;
Serial myPort4;
String val1;
String val2;
String val3;
String val4;


boolean firstContact = false;

void setup()
{
  size(200,200);
  
  String portName2 = Serial.list()[1];
  String portName3 = Serial.list()[2];
  
  print(Serial.list());
  myPort2 = new Serial(this, portName2, 9600);
  myPort3 = new Serial(this, portName3, 9600);
  
  myPort2.bufferUntil('\n');
  myPort3.bufferUntil('\n');
}
void draw()
{
  //if (mousePressed == true)
  //{
  //  myPort2.write('1');
  //  myPort3.write('1');
  //  println("1");
  //}else
  //{
  //  myPort2.write('0');
  //  myPort3.write('0');
  //}
  
  
  //if ( myPort2.available() > 0 || myPort3.available() > 0)
  //{
    
  //  val2 = myPort2.readStringUntil('\n');
  //  val3 = myPort3.readStringUntil('\n');
    
  //}
  
  //println(val2);
  //println(val3);
  
}

void serialEvent ( Serial myPort2) {
  val2 = myPort2.readStringUntil('\n');
  //val3 = myPort3.readStringUntil('\n');
  if (val2 != null){
    val2 = trim(val2);
    println(val2);
    
    if(firstContact == false){
      if (val2.equals("A")){
        myPort2.clear();
        firstContact = true;
        myPort2.write("A");
        println("contact");
      }
    }
    else {
      println(val2);
      
      if (mousePressed == true)
      {
        myPort2.write('1');
        println("1");
        
      }
      myPort2.write("A");
    }
  }    
}
