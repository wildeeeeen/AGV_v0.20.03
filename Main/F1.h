#include "CountDown.h"
CountDown CD1(CountDown::SECONDS);

void F1A_PlayMenu();
void F1B_Go();

void GOOO();
void LetsGOOO(int responseLetsGo);

int doAction(int statusDoAction);

void doAction_Berhenti();
void doAction_Maju();
void doAction_BelokKiri();
void doAction_BelokKanan();
void doAction_BalikKiri();
void doAction_BalikKanan();
void doAction_LiftOn();
void doAction_LiftOff();
void doAction_BuzzerOn();
void doAction_BuzzerOff();
void doAction_DO(int pinDO);
void doAction_LORA(String dataDoActionLORA);

// ========== Action ==========
// 0 > (kosong)
// 1 > Berhenti
// 2 > Maju
// 3 > Belok Kiri
// 4 > Belok Kanan
// 5 > Balik Kiri
// 6 > Balik Kanan
// 7 > Lift On
// 8 > Lift Off
// 9 > Buzzer On
// 10 > Buzzer Off
// 11 > DO 00,LOW
// 12 > DO 00,HIGH
// 13 > DO 01,LOW
// 14 > DO 01,HIGH
// 15 > DO 02,LOW
// 16 > DO 02,HIGH
// 17 > DO 03,LOW
// 18 > DO 03,HIGH
// 19 > DO 04,LOW
// 20 > DO 04,HIGH
// 21 > DO 05,LOW
// 22 > DO 05,HIGH
// 23 > Tx_1
// 24 > Tx_2
// 25 > Tx_3
// 26 > Tx_4
// 27 > Tx_5

int pwmKiri, pwmKanan;


void F1A_PlayMenu(){
  pageF1A.show();
  Serial.println("F1A_PlayMenu");
  Gmode = 1;
  nF1ANum.setValue(Gmode);
  
  
  while(true){
    nexLoop(nex_listen_list_F1A_PlayMenu);
    switch(Tombol){
      case tBACK:
        Tombol = tIDLE;
        Serial.println("bF1ABack");
        Menu = F0;
        return false;
        break;
      case tNEXT:
        Tombol = tIDLE;
        Serial.println("bF1ANext");
        F1B_Go();
        pageF1A.show();
        Serial.println("F1A_PlayMenu");
        nF1ANum.setValue(Gmode);
        break;
      case tMINUS:
        Tombol = tIDLE;
        Serial.println("bF1AMinus");
        nF1ANum.getValue(&Gmode);
        Gmode -= 1;
        if(Gmode <= 1)            Gmode = 1;
        if(Gmode >= jumlahData)   Gmode = jumlahData;
        nF1ANum.setValue(Gmode);
        break;
      case tPLUS:
        Tombol = tIDLE;
        Serial.println("bF1APlus");
        nF1ANum.getValue(&Gmode);
        Gmode += 1;
        if(Gmode <= 1)            Gmode = 1;
        if(Gmode >= jumlahData)   Gmode = jumlahData;
        nF1ANum.setValue(Gmode);
        break;
      default:
        break;
    }
  }
}


void F1B_Go(){
  pageF1B.show();
  Serial.println("F1B_Go");
// ================== disini untuk menentukan siapa si Next Target ============================
  int totalKe[1+jumlahData];
  for(int i = 1; i <= jumlahData; i++){ // reset totalKe[]
    totalKe[i] = 0;
    Serial.print(totalKe[i]);
  }
  
  Serial.println("\n");
  for(int j = 1; j <= jumlahData; j++){
    for(int i = 1; i <= jumlahData; i++){
      if(modeKe[i] == j){
        totalKe[j] += 1;
      }
    }
  }
  urutanKe = 0;
  for(int i = 1; i < Gmode; i++){
    urutanKe += totalKe[i];
  }
  urutanKe = urutanKe + 1;

  if(typeKe[urutanKe] == 1){
    nextTarget = idRFID[triggerKe[urutanKe]];
  }
  else if(typeKe[urutanKe] == 2){
    nextTarget = stringDI(triggerKe[urutanKe]);
  }
  else if(typeKe[urutanKe] == 3){
    nextTarget = stringLORA(triggerKe[urutanKe]);
  }
  else{
    nextTarget = "TIDAK ADA";
  }
  Serial.print("urutanKe    : ");  Serial.println(urutanKe);
  Serial.print("Gmode       : ");  Serial.println(Gmode);
  Serial.print("TIPEnya     : ");  Serial.println(typeKe[urutanKe]);
  Serial.print("TRIGGER DI  : ");  Serial.println(triggerKe[urutanKe]);
  Serial.print("Action DI   : ");  Serial.println(actionKe[urutanKe]);
  Serial.print("nextTarget  : ");  Serial.println(nextTarget);

  Serial.println("=========================================");
  Serial.print("totalKe[");   Serial.print(Gmode);
  Serial.print("] : ");       Serial.println(totalKe[Gmode]);
  urutanLastTarget = totalKe[Gmode] + urutanKe - 1;
  Serial.print("urutanLastTarget:");   Serial.println(urutanLastTarget);
  
  nF1BMode.setValue(Gmode);
  Serial2.print("gF1BStatusMode.txt=");
  Serial2.print("\"");
  Serial2.print("Stanby <->");
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  
  Serial2.print("tF1BNextTarget.txt=");
  Serial2.print("\"");
  Serial2.print(nextTarget);
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff); 

  Serial2.print("tF1BNextAction.txt=");
  Serial2.print("\"");
  Serial2.print(stringAction(actionKe[urutanKe]));
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  while(true){
    nexLoop(nex_listen_list_F1B_Go);
                                                // Serial plot ====================================================================
                                                  Serial.print(100);
                                                  Serial.print(" ");
//                                                  Serial.print(sensorCompass());
//                                                  Serial.print(" ");
                                                  Serial.print(sensorLine());
                                                  Serial.print(" ");
                                                  Serial.print(PID_result);
                                                  Serial.print(" ");
                                                  Serial.print(pwmKiri);
                                                  Serial.print(" ");
                                                  Serial.print(pwmKanan);
                                                  Serial.print(" ");
                                                  Serial.print(CD1.remaining());
                                                  Serial.print(" ");
                                                  Serial.println(-100);
                                                // Serial plot ====================================================================  
    if(digitalRead(pinStop) == LOW) Tombol = tSTOP;
    if(digitalRead(pinEmergencyStop) == LOW) Tombol = tCANCEL;
    
    switch(Tombol){
      case tCANCEL:
        Tombol = tIDLE;
        Serial.println("bF1BCancel");
        return false;
        break;
      case tSTOP:
        Tombol = tIDLE;
        Serial.println("bF1BStop");

        Serial2.print("gF1BStatusMode.txt=");
        Serial2.print("\"");
        Serial2.print("Stop  X");
        Serial2.print("\"");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);

        Serial2.print("bF1BGo.bco=");
        Serial2.print(50712);
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        
        break;
      case tGO:
        Tombol = tIDLE;
        Serial.println("bF1BGo");
        
        for(int i = 3; i >= 1; i--){    // count down for go
          Serial2.print("gF1BStatusMode.txt=");
          Serial2.print("\"");
          Serial2.print(i);
          Serial2.print("\"");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          delay(100);
        }
        
        Serial2.print("gF1BStatusMode.txt=");
        Serial2.print("\"");
        Serial2.print("Jalan  -->>");
        Serial2.print("\"");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);

        Serial2.print("bF1BGo.bco=");
        Serial2.print(1500);
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);

        CD1.start(5);
        GOOO();
        
      default:
        break;
    }
  }
}

int responseGOOO = 0;

void GOOO(){
  responseGOOO = 0;
  Serial.println("GO GO Gooo");

  Serial2.print("tF1BNextTarget.txt=");
  Serial2.print("\"");
  Serial2.print(nextTarget);
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  
  Serial2.print("tF1BNextAction.txt=");
  Serial2.print("\"");
  Serial2.print(stringAction(actionKe[urutanKe]));
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  
  String scanRFID;
  String messageFromLora;
  messageFromLora = "";
  scanRFID = "";
  
  while(true)
  { 
    LetsGOOO(responseGOOO);
    
    nexLoop(nex_listen_list_F1B_Go);
    Usb.Task();
    
    if(scanFinished == true){
      scanRFID = scanResult;
      scanFinished = false;
      scanResult = "";
    }

    if(digitalRead(pinStop) == LOW) Tombol = tSTOP;
    if(digitalRead(pinEmergencyStop) == LOW) Tombol = tCANCEL;
    
    if(Tombol == tSTOP)    return false;
    if(Tombol == tCANCEL)   return false;
    
    switch(typeKe[urutanKe]){
      case typeRFID:
        if(idRFID[triggerKe[urutanKe]] == scanRFID && nextTarget == idRFID[triggerKe[urutanKe]])
        {
          prevTarget = idRFID[triggerKe[urutanKe]];
          actionKe[urutanKe];
          stringAction(actionKe[urutanKe]); // ini untuk tampilan ke serial monitor

          Serial.print(" urutanKe       : ");   Serial.println(urutanKe);
          Serial.print(" prevTarget     : ");   Serial.println(prevTarget);
          Serial.print(" stringAction   : ");   Serial.println(stringAction(actionKe[urutanKe]));

          Serial.print(" nowRFID        : ");   Serial.println(scanRFID);
          Serial.print(" actionKe[]     : ");   Serial.println(actionKe[urutanKe]);
          responseGOOO = doAction(actionKe[urutanKe]);

          if(urutanKe == urutanLastTarget){
            Tombol = tCANCEL;
            return false;
          }
          
          Serial2.print("tF1BPrevTarget.txt=");
          Serial2.print("\"");
          Serial2.print(prevTarget);
          Serial2.print("\"");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          
          Serial2.print("tF1BPrevAction.txt=");
          Serial2.print("\"");
          Serial2.print(stringAction(actionKe[urutanKe]));
          Serial2.print("\"");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);

          // Tempatkan ini di bawah, karena ini increment dari urutanKe, sehingga nextTarget berubah
          urutanKe++;
          if(typeKe[urutanKe] == 1){
            nextTarget = idRFID[triggerKe[urutanKe]];
          }
          else if(typeKe[urutanKe] == 2){
            nextTarget = stringDI(triggerKe[urutanKe]);
          }
          else if(typeKe[urutanKe] == 3){
            nextTarget = stringLORA(triggerKe[urutanKe]);
          }
          Serial.print(" nextTarget     : ");   Serial.println(nextTarget);
          Serial.print(" stringAction   : ");   Serial.println(stringAction(actionKe[urutanKe]));

          
          Serial2.print("tF1BNextTarget.txt=");
          Serial2.print("\"");
          Serial2.print(nextTarget);
          Serial2.print("\"");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);

          Serial2.print("tF1BNextAction.txt=");
          Serial2.print("\"");
          Serial2.print(stringAction(actionKe[urutanKe]));
          Serial2.print("\"");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          
          Serial.print(" urutanKe-------: ");   Serial.println(urutanKe);
          scanRFID = "";
        }
        break;
      case typeDI:
        bool statusDI;
        if(nextTarget.substring(5,6) == "H")  statusDI = HIGH;
        else                                  statusDI = LOW;
        
        if(digitalRead(digitalInput[(nextTarget.substring(3,4)).toInt()]) == statusDI && nextTarget == stringDI(triggerKe[urutanKe]))
        {
          prevTarget = stringDI(triggerKe[urutanKe]);
          actionKe[urutanKe];
          stringAction(actionKe[urutanKe]); // ini untuk tampilan ke serial monitor

          Serial.print(" urutanKe       : ");   Serial.println(urutanKe);
          Serial.print(" prevTarget     : ");   Serial.println(prevTarget);
          Serial.print(" stringAction   : ");   Serial.println(stringAction(actionKe[urutanKe]));

          Serial.print(" nowRFID        : ");   Serial.println(scanRFID);
          Serial.print(" actionKe[]     : ");   Serial.println(actionKe[urutanKe]);
          responseGOOO = doAction(actionKe[urutanKe]);

          if(urutanKe == urutanLastTarget){
            Tombol = tCANCEL;
            return false;
          }
          
          Serial2.print("tF1BPrevTarget.txt=");
          Serial2.print("\"");
          Serial2.print(prevTarget);
          Serial2.print("\"");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          
          Serial2.print("tF1BPrevAction.txt=");
          Serial2.print("\"");
          Serial2.print(stringAction(actionKe[urutanKe]));
          Serial2.print("\"");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);

          // Tempatkan ini di bawah, karena ini increment dari urutanKe, sehingga nextTarget berubah
          urutanKe++;
          if(typeKe[urutanKe] == 1){
            nextTarget = idRFID[triggerKe[urutanKe]];
          }
          else if(typeKe[urutanKe] == 2){
            nextTarget = stringDI(triggerKe[urutanKe]);
          }
          else if(typeKe[urutanKe] == 3){
            nextTarget = stringLORA(triggerKe[urutanKe]);
          }
          Serial.print(" nextTarget     : ");   Serial.println(nextTarget);
          Serial.print(" stringAction   : ");   Serial.println(stringAction(actionKe[urutanKe]));

          
          Serial2.print("tF1BNextTarget.txt=");
          Serial2.print("\"");
          Serial2.print(nextTarget);
          Serial2.print("\"");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);

          Serial2.print("tF1BNextAction.txt=");
          Serial2.print("\"");
          Serial2.print(stringAction(actionKe[urutanKe]));
          Serial2.print("\"");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          
          Serial.print(" urutanKe-------: ");   Serial.println(urutanKe);
          scanRFID = "";
        }
        break;
      case typeLORA:
        if(Serial3.available()){                      // cek bahwa si lora node mengirim data cuma satu kali aja, sebenernya dia punya program otomatisasi kalau ga ke kirim, bakal ngirim lagi, karena ada callback,,,,, pokonya si lora node harus ngirim sekali aja
          messageFromLora = Serial3.readString();
        }

        if(stringLORA(triggerKe[urutanKe]) == messageFromLora && nextTarget == stringLORA(triggerKe[urutanKe]))
        {
          Serial.print(" urutanKe-------: ");   Serial.println(urutanKe);
          prevTarget = stringLORA(triggerKe[urutanKe]);
          actionKe[urutanKe];
          stringAction(actionKe[urutanKe]); // ini untuk tampilan ke serial monitor

          responseGOOO = doAction(actionKe[urutanKe]);

          if(urutanKe == urutanLastTarget){
            Tombol = tCANCEL;
            return false;
          }
          
          Serial2.print("tF1BPrevTarget.txt=");
          Serial2.print("\"");
          Serial2.print(prevTarget);
          Serial2.print("\"");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          
          Serial2.print("tF1BPrevAction.txt=");
          Serial2.print("\"");
          Serial2.print(stringAction(actionKe[urutanKe]));
          Serial2.print("\"");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);

          // Tempatkan ini di bawah, karena ini increment dari urutanKe, sehingga nextTarget berubah
          urutanKe++;
          if(typeKe[urutanKe] == 1){
            nextTarget = idRFID[triggerKe[urutanKe]];
          }
          else if(typeKe[urutanKe] == 2){
            nextTarget = stringDI(triggerKe[urutanKe]);
          }
          else if(typeKe[urutanKe] == 3){
            nextTarget = stringLORA(triggerKe[urutanKe]);
          }
          Serial.print(" nextTarget     : ");   Serial.println(nextTarget);
          Serial.print(" stringAction   : ");   Serial.println(stringLORA(triggerKe[urutanKe]));

          Serial2.print("tF1BNextTarget.txt=");
          Serial2.print("\"");
          Serial2.print(nextTarget);
          Serial2.print("\"");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);

          Serial2.print("tF1BNextAction.txt=");
          Serial2.print("\"");
          Serial2.print(stringAction(actionKe[urutanKe]));
          Serial2.print("\"");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          
          Serial.print(" urutanKe-------: ");   Serial.println(urutanKe);
          messageFromLora = "";
          scanRFID = "";
        }
        break;
      default:
        break;  
    }    
  }
}


void LetsGOOO(int responseLetsGo){
  
  PID_compute(sensorLine());
  while(readDistanceSensor() != -1){
    pwmKanan = 0;
    pwmKiri = 0;
    CD1.start(5);
  }
  
  if(responseLetsGo == 1){
    pwmKanan = 0;
    pwmKiri = 0;
  }
  else if(responseLetsGo != 1 && CD1.remaining() != 0){   // accel aktif
    pwmKanan = Gspeed + (Gaccel * CD1.remaining()) + - PID_result;
    pwmKiri = Gspeed + (Gaccel * CD1.remaining()) + PID_result;
  }
  else if(responseLetsGo != 1 && CD1.remaining() == 0){
    pwmKanan = Gspeed - PID_result;
    pwmKiri = Gspeed + PID_result;
  }

    Serial.print(pwmKanan);                 Serial.print(" ");
    Serial.print(pwmKiri);                  Serial.print(" ");
    Serial.println(CD1.remaining());
    
}

int doAction(int statusDoAction){
       if(statusDoAction == 0)    return 0;   // kosong
  else if(statusDoAction == 1)    {doAction_Berhenti();           return 1;}   // berhenti
  else if(statusDoAction == 2)    {doAction_Maju();               return 2;}   // maju
  else if(statusDoAction == 3)    {doAction_BelokKiri();          return 3;}
  else if(statusDoAction == 4)    {doAction_BelokKanan();         return 4;}
  else if(statusDoAction == 5)    {doAction_BalikKiri();          return 5;}
  else if(statusDoAction == 6)    {doAction_BalikKanan();         return 6;}
  else if(statusDoAction == 7)    {doAction_LiftOn();             return 7;}
  else if(statusDoAction == 8)    {doAction_LiftOff();            return 8;}
  else if(statusDoAction == 9)    {doAction_BuzzerOn();           return 9;}
  else if(statusDoAction == 10)   {doAction_BuzzerOff();          return 10;}
  else if(statusDoAction == 11)   {doAction_DO(statusDoAction);   return 11;}   // seharusnya bukan (statusDoAction), tapi pin nya, ntar di benerin deh
  else if(statusDoAction == 12)   {doAction_DO(statusDoAction);   return 12;}
  else if(statusDoAction == 13)   {doAction_DO(statusDoAction);   return 13;}
  else if(statusDoAction == 14)   {doAction_DO(statusDoAction);   return 14;}
  else if(statusDoAction == 15)   {doAction_DO(statusDoAction);   return 15;}
  else if(statusDoAction == 16)   {doAction_DO(statusDoAction);   return 16;}
  else if(statusDoAction == 17)   {doAction_DO(statusDoAction);   return 17;}
  else if(statusDoAction == 18)   {doAction_DO(statusDoAction);   return 18;}
  else if(statusDoAction == 19)   {doAction_DO(statusDoAction);   return 19;}
  else if(statusDoAction == 20)   {doAction_DO(statusDoAction);   return 20;}
  else if(statusDoAction == 21)   {doAction_DO(statusDoAction);   return 21;}
  else if(statusDoAction == 22)   {doAction_DO(statusDoAction);   return 22;}
  else if(statusDoAction == 23)   {doAction_LORA("Tx_1");         return 23;}
  else if(statusDoAction == 24)   {doAction_LORA("Tx_2");         return 24;}
  else if(statusDoAction == 25)   {doAction_LORA("Tx_3");         return 25;}
  else if(statusDoAction == 26)   {doAction_LORA("Tx_4");         return 26;}
  else if(statusDoAction == 27)   {doAction_LORA("Tx_5");         return 27;}
}

void doAction_Berhenti(){
  // pura pura belok Berhenti
  Serial2.print("gF1BStatusMode.bco=");
  Serial2.print(63488);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  
  Serial2.print("gF1BStatusMode.txt=");
  Serial2.print("\"");
  Serial2.print("BERHENTI");
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

void doAction_Maju(){
  // pura pura belok Maju
  Serial2.print("gF1BStatusMode.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  
  Serial2.print("gF1BStatusMode.txt=");
  Serial2.print("\"");
  Serial2.print("Jalan  -->>");
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  
  CD1.start(5);
}

void doAction_BelokKiri(){
  Serial2.print("gF1BStatusMode.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("gF1BStatusMode.txt=");
  Serial2.print("\"");
  Serial2.print("Jalan  -->>");
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(1500);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  
  int targetHeading = sensorCompass() - 90;
  
  int _doAction = 0;
  while(_doAction == 0){
    
    nexLoop(nex_listen_list_F1B_Go);
    if(Tombol == tSTOP)    return false;
    if(digitalRead(pinStop) == LOW) {
      Tombol = tSTOP;
      _doAction = 1;
      return false;
    }
    if(digitalRead(pinEmergencyStop) == LOW) {
      Tombol = tCANCEL;
      _doAction = 1;
      return false;
    }
    
    int selisihRaw = targetHeading - sensorCompass();
    if(selisihRaw <= -180)           selisihRaw = (selisihRaw + 180) + 180;
    
    int selisihTargetHeading = map(selisihRaw, 0, -90, 0, 100);
    if(selisihTargetHeading >= 100)   selisihTargetHeading = 100;
    if(selisihTargetHeading <= 0)     selisihTargetHeading = 0;
    
    float selisihPersen = (float)selisihTargetHeading / 100;

    if(selisihPersen > 0.5 && selisihRaw < 0){
      pwmKanan  = Gspeed * selisihPersen;
      pwmKiri   = 0;
    }else if(selisihPersen <= 0.5 && selisihRaw < 0){
      pwmKanan  = Gspeed * 0.5;
      pwmKiri   = 0;
    }else if(selisihRaw > 0){
      pwmKanan  = 0;
      pwmKiri   = 0;
    }

    Serial.print("Compass: ");
    Serial.print(sensorCompass());
    Serial.print(" targetHeading: ");
    Serial.print(targetHeading);
    Serial.print(" SelisihRaw: ");
    Serial.print(selisihRaw);
    Serial.print(" selisihTargetHeading: ");
    Serial.print(selisihTargetHeading);
    Serial.print(" selisihPersen: ");
    Serial.print(selisihPersen);
    Serial.print(" pwmKiri: ");
    Serial.print(pwmKiri);
    Serial.print(" pwmKanan: ");
    Serial.print(pwmKanan);
    Serial.println();

    if(selisihRaw > -10 && selisihRaw < 10 ) _doAction = 1;   // nilai 10 ini tergantung nanti pembacaan sensor garis, bisa dapet ga di range 10 ini pas garis nya ke detect
  }
  
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

void doAction_BelokKanan(){
  Serial2.print("gF1BStatusMode.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("gF1BStatusMode.txt=");
  Serial2.print("\"");
  Serial2.print("Jalan  -->>");
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(64512);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  
  int targetHeading = sensorCompass() + 90;

  int _doAction = 0;
  while(_doAction == 0){
    
    nexLoop(nex_listen_list_F1B_Go);
    if(Tombol == tSTOP)    return false;
    if(digitalRead(pinStop) == LOW) {
      Tombol = tSTOP;
      _doAction = 1;
      return false;
    }
    if(digitalRead(pinEmergencyStop) == LOW) {
      Tombol = tCANCEL;
      _doAction = 1;
      return false;
    }
    
    int selisihRaw = targetHeading - sensorCompass();
    if(selisihRaw >= 180)          selisihRaw = (selisihRaw - 180) - 180;
    
    int selisihTargetHeading = map(selisihRaw, 0, 90, 0, 100);
    if(selisihTargetHeading >= 100)   selisihTargetHeading = 100;
    if(selisihTargetHeading <= 0)     selisihTargetHeading = 0;
    
    float selisihPersen = (float)selisihTargetHeading / 100;

    if(selisihPersen > 0.5 && selisihRaw > 0){
      pwmKanan  = 0;
      pwmKiri   = Gspeed * selisihPersen;
    }else if(selisihPersen <= 0.5 && selisihRaw > 0){
      pwmKanan  = 0;
      pwmKiri   = Gspeed * 0.5;
    }else if(selisihRaw < 0){
      pwmKanan  = 0;
      pwmKiri   = 0;
    }

    Serial.print("Compass: ");
    Serial.print(sensorCompass());
    Serial.print(" targetHeading: ");
    Serial.print(targetHeading);
    Serial.print(" SelisihRaw: ");
    Serial.print(selisihRaw);
    Serial.print(" selisihTargetHeading: ");
    Serial.print(selisihTargetHeading);
    Serial.print(" selisihPersen: ");
    Serial.print(selisihPersen);
    Serial.print(" pwmKiri: ");
    Serial.print(pwmKiri);
    Serial.print(" pwmKanan: ");
    Serial.print(pwmKanan);
    Serial.println();
    
    if(selisihRaw > -10 && selisihRaw < 10 ) _doAction = 1;
  }
  
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

void doAction_BalikKiri(){
  Serial2.print("gF1BStatusMode.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("gF1BStatusMode.txt=");
  Serial2.print("\"");
  Serial2.print("Jalan  -->>");
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(2500);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  int targetHeading = sensorCompass() - 180;
  
  int _doAction = 0;
  while(_doAction == 0){
    
    nexLoop(nex_listen_list_F1B_Go);
    if(Tombol == tSTOP)    return false;
    if(digitalRead(pinStop) == LOW) {
      Tombol = tSTOP;
      _doAction = 1;
      return false;
    }
    if(digitalRead(pinEmergencyStop) == LOW) {
      Tombol = tCANCEL;
      _doAction = 1;
      return false;
    }
    
    int selisihRaw = targetHeading - sensorCompass();
    if(selisihRaw <= -180)           selisihRaw = (selisihRaw + 180) + 180;
    
    int selisihTargetHeading = map(selisihRaw, 0, -180, 0, 100);
    if(selisihTargetHeading >= 100)   selisihTargetHeading = 100;
    if(selisihTargetHeading <= 0)     selisihTargetHeading = 0;
    
    float selisihPersen = (float)selisihTargetHeading / 100;

    if(selisihRaw > 135){    // 135 = 180 - 45, kenapa karena untuk range si robot menghadap ke depannya, di kasih range dulu
      pwmKanan  = Gspeed;
      pwmKiri   = 0;
    }else if(selisihPersen > 0.5 && selisihRaw < 0){
      pwmKanan  = Gspeed * selisihPersen;
      pwmKiri   = 0;
    }else if(selisihPersen <= 0.5 && selisihRaw < 0){
      pwmKanan  = Gspeed * 0.5;
      pwmKiri   = 0;
    }else if(selisihRaw >= 0 && selisihRaw <= 135){
      pwmKanan  = 0;
      pwmKiri   = 0;
    }

    Serial.print("Compass: ");
    Serial.print(sensorCompass());
    Serial.print(" targetHeading: ");
    Serial.print(targetHeading);
    Serial.print(" SelisihRaw: ");
    Serial.print(selisihRaw);
    Serial.print(" selisihTargetHeading: ");
    Serial.print(selisihTargetHeading);
    Serial.print(" selisihPersen: ");
    Serial.print(selisihPersen);
    Serial.print(" pwmKiri: ");
    Serial.print(pwmKiri);
    Serial.print(" pwmKanan: ");
    Serial.print(pwmKanan);
    Serial.println();
    
    if(selisihRaw > -10 && selisihRaw < 10 ) _doAction = 1;
  }
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

void doAction_BalikKanan(){
  Serial2.print("gF1BStatusMode.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("gF1BStatusMode.txt=");
  Serial2.print("\"");
  Serial2.print("Jalan  -->>");
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(3500);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  int targetHeading = sensorCompass() + 180;
  
  int _doAction = 0;
  while(_doAction == 0){

    nexLoop(nex_listen_list_F1B_Go);
    if(Tombol == tSTOP)    return false;
    if(digitalRead(pinStop) == LOW) {
      Tombol = tSTOP;
      _doAction = 1;
      return false;
    }
    if(digitalRead(pinEmergencyStop) == LOW) {
      Tombol = tCANCEL;
      _doAction = 1;
      return false;
    }
    
    int selisihRaw = targetHeading - sensorCompass();
    if(selisihRaw >= 180)           selisihRaw = (selisihRaw - 180) - 180;
    
    int selisihTargetHeading = map(selisihRaw, 0, 180, 0, 100);
    if(selisihTargetHeading >= 100)   selisihTargetHeading = 100;
    if(selisihTargetHeading <= 0)     selisihTargetHeading = 0;
    
    float selisihPersen = (float)selisihTargetHeading / 100;

    if(selisihRaw < -135){    // -135 = -180 + 45, kenapa karena untuk range si robot menghadap ke depannya, di kasih range dulu
      pwmKanan  = 0;
      pwmKiri   = Gspeed;
    }else if(selisihPersen > 0.5 && selisihRaw > 0){
      pwmKanan  = 0;
      pwmKiri   = Gspeed * selisihPersen;
    }else if(selisihPersen <= 0.5 && selisihRaw > 0){
      pwmKanan  = 0;
      pwmKiri   = Gspeed * 0.5;
    }else if(selisihRaw <= 0 && selisihRaw >= -135){
      pwmKanan  = 0;
      pwmKiri   = 0;
    }

    Serial.print("Compass: ");
    Serial.print(sensorCompass());
    Serial.print(" targetHeading: ");
    Serial.print(targetHeading);
    Serial.print(" SelisihRaw: ");
    Serial.print(selisihRaw);
    Serial.print(" selisihTargetHeading: ");
    Serial.print(selisihTargetHeading);
    Serial.print(" selisihPersen: ");
    Serial.print(selisihPersen);
    Serial.print(" pwmKiri: ");
    Serial.print(pwmKiri);
    Serial.print(" pwmKanan: ");
    Serial.print(pwmKanan);
    Serial.println();

    if(selisihRaw > -10 && selisihRaw < 10 ) _doAction = 1;
  }
  
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

void doAction_LiftOn(){
  Serial2.print("gF1BStatusMode.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("gF1BStatusMode.txt=");
  Serial2.print("\"");
  Serial2.print("Jalan  -->>");
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(64543);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  // ini lempar ke ke PIN X aja  
  delay(5000);
  
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

void doAction_LiftOff(){
  Serial2.print("gF1BStatusMode.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("gF1BStatusMode.txt=");
  Serial2.print("\"");
  Serial2.print("Jalan  -->>");
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(34815);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  // ini lempar ke ke PIN X aja
  delay(1000);

  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

void doAction_BuzzerOn(){
  Serial2.print("gF1BStatusMode.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("gF1BStatusMode.txt=");
  Serial2.print("\"");
  Serial2.print("Jalan  -->>");
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(33792);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  // Lempar ke MP3
  delay(5000);
  
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

void doAction_BuzzerOff(){
  Serial2.print("gF1BStatusMode.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("gF1BStatusMode.txt=");
  Serial2.print("\"");
  Serial2.print("Jalan  -->>");
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(50712);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  
  // Lempar ke MP3
  delay(5000);
  
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

void doAction_DO(int pinDO){
  Serial2.print("gF1BStatusMode.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("gF1BStatusMode.txt=");
  Serial2.print("\"");
  Serial2.print("Jalan  -->>");
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(3000 + pinDO*100);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  
  // Lempar ke Digital Output
  delay(5000);
  
  Serial2.print("tF1BNextAction.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

void doAction_LORA(String dataDoActionLORA){
  Serial2.print("gF1BStatusMode.bco=");
  Serial2.print(65504);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("gF1BStatusMode.txt=");
  Serial2.print("\"");
  Serial2.print("Jalan  -->>");
  Serial2.print("\"");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  if(dataDoActionLORA == "Tx_1"){
    Serial3.print(dataDoActionLORA);
    
    Serial2.print("tF1BNextAction.bco=");
    Serial2.print(4500);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    delay(1000);
    Serial2.print("tF1BNextAction.bco=");
    Serial2.print(65504);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }
  else if(dataDoActionLORA == "Tx_2"){
    Serial3.print(dataDoActionLORA);
    
    Serial2.print("tF1BNextAction.bco=");
    Serial2.print(4600);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    delay(1000);
    Serial2.print("tF1BNextAction.bco=");
    Serial2.print(65504);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }
  else if(dataDoActionLORA == "Tx_3"){
    Serial3.print(dataDoActionLORA);
    
    Serial2.print("tF1BNextAction.bco=");
    Serial2.print(4700);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    delay(1000);
    Serial2.print("tF1BNextAction.bco=");
    Serial2.print(65504);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }
  else if(dataDoActionLORA == "Tx_4"){
    Serial3.print(dataDoActionLORA);
    
    Serial2.print("tF1BNextAction.bco=");
    Serial2.print(4800);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    delay(1000);
    Serial2.print("tF1BNextAction.bco=");
    Serial2.print(65504);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }
  else if(dataDoActionLORA == "Tx_5"){
    Serial3.print(dataDoActionLORA);
    
    Serial2.print("tF1BNextAction.bco=");
    Serial2.print(4900);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    delay(1000);
    Serial2.print("tF1BNextAction.bco=");
    Serial2.print(65504);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }
}
