#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"

//FPS connected to pin 4 and 5 - see previous schemas
FPS_GT511C3 fps(4, 5);

int choice = 0;
bool isFingerPressed = false;

void setup() {
  Serial.begin(9600);
  delay(100);
  fps.Open();
  fps.SetLED(false);
  mainActivity();
  
}

void loop() {
  
}


void mainActivity(){
  
  hbar();
  Serial.println("-- FINGERPRINT SENSOR --");
  Serial.println("");
  Serial.println("1. Enroll");
  Serial.println("2. Recognize");
  Serial.println("3. Reset All");
  Serial.println("4. Help");
  Serial.println("Choose 1, 2, 3 or 4: ");
  
  while (true) {
    if (Serial.available() > 0) {
      // read the incoming byte:
      choice = char(Serial.read());
      if (choice == 49) {
        choice1();
      }
      if (choice == 50) {
        choice2();
      }
      if (choice == 51) {
        choice3();
      }

      if (choice == 52) {
        choice4();
      }
    }
      delay(2);
  }
}


void choice1(){
  hbar();
  Serial.println("1. Enroll");
  Enroll();
}


void choice2(){
  hbar();
  Serial.println("2. Recognise");
  Serial.println("");
  Serial.println("Please press finger\n");
  
  fps.SetLED(true);
  
  while (true) {
    Serial.print(".");
   if (fps.IsPressFinger()) {
      fps.CaptureFinger(false);
      int id = fps.Identify1_N();
      if (id <200) {
        Serial.print("\nVerified ID:");
        Serial.println(id);
        successNote();
        mainActivity();
      }
      else {
        Serial.println("\nFinger not found");
        failureNote();
        mainActivity();
      }
    }
    delay(100);
  }
}

void choice3(){
  
  fps.DeleteAll();
  successNote();
  hbar();
  Serial.println("3. Reset all");
  Serial.println("");
  Serial.println("Done!");
  mainActivity();
}

void choice4(){
  hbar();
  Serial.println("4. Help");
  Serial.println("");
  Serial.println("- No light: you can not interact with the sensor at the moment.");
  Serial.println("- Uninterrupted light: waiting for your finger.");
  Serial.println("- 2 blinks: you can remove your finger.");
  Serial.println("- 2 long blinks: an error happened.");
  Serial.println("- 5 fast blinks: Success!");
  mainActivity();
}


void hbar(){
  Serial.println("---------------------------------------");
}


// Enroll
void Enroll() {
  // Enroll test
  
  fps.SetLED(true);  

  // find open enroll id
  int enrollid = 0;
  bool usedid = true;
  while (usedid == true) {
    usedid = fps.CheckEnrolled(enrollid);
    if (usedid==true) enrollid++;
  }
  fps.EnrollStart(enrollid);

  // enroll
  Serial.print("Press finger to Enroll #");
  Serial.println(enrollid);
  while(fps.IsPressFinger() == false) {
    delay(75);
    };
  bool bret = fps.CaptureFinger(true);
  int iret = 0;
  if (bret != false) {
    blinks();
    Serial.println("Remove finger");
    fps.Enroll1(); 
    while(fps.IsPressFinger() == true) delay(100);
    Serial.println("Press same finger again");
    while(fps.IsPressFinger() == false) delay(100);
    bret = fps.CaptureFinger(true);
    if (bret != false) {
      blinks();
      Serial.println("Remove finger");
      fps.Enroll2();
      while(fps.IsPressFinger() == true) delay(100);
      Serial.println("Press same finger yet again");
      while(fps.IsPressFinger() == false) delay(100);
      bret = fps.CaptureFinger(true);
      if (bret != false) {
        blinks();
        Serial.println("Remove finger");
        iret = fps.Enroll3();
        if (iret == 0) {
          Serial.println("Enrolling Successfull");
          successNote();
          mainActivity();
        }
        else{
          Serial.print("Enrolling Failed with error code:");
          Serial.println(iret);
          failureNote();
          mainActivity();
        }
      }
      else {
        Serial.println("Failed to capture third finger");
        failureNote();
        mainActivity();
      }
    }
    else {
      Serial.println("Failed to capture second finger");
      failureNote();
      mainActivity();
    }
  }
  else {
    Serial.println("Failed to capture first finger");
    failureNote();
    mainActivity();
   }
}


void blinks() {
  delay(100);
  fps.SetLED(false);
  delay(100);
  fps.SetLED(true);
  delay(100);
  fps.SetLED(false);
  delay(100);
  fps.SetLED(true);
}

void note(int x){
  fps.SetLED(true);
  delay(25);
  fps.SetLED(false);
  delay(x-25);
}

void successNote(){
  fps.SetLED(false);
  note(50);
  note(50);
  note(50);
  note(50);
}

void failureNote(){
  fps.SetLED(false);
  delay(800);
  note(50);
  delay(800);
  note(50);
}

