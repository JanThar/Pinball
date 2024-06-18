// leds

// leds at the bottom lanes, + and x circles
#define L_COMM_SND_T 20
#define L_COMM_SND_X 6
#define L_COMM_ATT_T 0
#define L_COMM_ATT_X 1
#define L_COMM_REC_T 12
#define L_COMM_REC_X 9

int commLEDs[] = {L_COMM_SND_X,L_COMM_ATT_X,L_COMM_REC_X, L_COMM_SND_T,L_COMM_ATT_T,L_COMM_REC_T};

// left side LED block: same bases, from top (T) through middle(M) to bottom(B) and right(R) to left(L)
#define L_COMM_RB_TR  22
#define L_COMM_RB_TL  23
#define L_COMM_RB_MTL 21
#define L_COMM_RB_MTR 19
#define L_COMM_RB_MBR 17
#define L_COMM_RB_MBL 18
#define L_COMM_RB_BL  7
#define L_COMM_RB_BR  5

int commLEDs_RB[] = {L_COMM_RB_TL,  L_COMM_RB_TR,  
                     L_COMM_RB_MTL, L_COMM_RB_MTR,
                     L_COMM_RB_MBL, L_COMM_RB_MBR,
                     L_COMM_RB_BL,  L_COMM_RB_BR};

// right side LED block: different bases, from top (T) through middle(M) to bottom(B) and right(R) to left(L)
#define L_COMM_DB_TL  14
#define L_COMM_DB_TR  13
#define L_COMM_DB_MTL 11
#define L_COMM_DB_MTR 10
#define L_COMM_DB_MBR 4
#define L_COMM_DB_MBL 8
#define L_COMM_DB_BL  2
#define L_COMM_DB_BR  3

int commLEDs_DB[] = {L_COMM_DB_TL,  L_COMM_DB_TR,
                     L_COMM_DB_MTL, L_COMM_DB_MTR,
                     L_COMM_DB_MBL, L_COMM_DB_MBR,
                     L_COMM_DB_BL,  L_COMM_DB_BR};

// buttons
#define B_COMM_SND 1
#define B_COMM_REC 0

int commButtons[] = {B_COMM_SND, B_COMM_REC};
                     

boolean code[] = {0,1,1,0};

int commValue[4][16] = {{0,0,1,1,0,0,1,1, 0,0,0,0,1,1,1,1}, // base sender
                        {0,0,1,1,1,1,0,0, 0,0,1,1,0,0,1,1}, // base attacker
                        {0,0,1,1,0,0,1,1, 1,1,1,1,0,0,0,0}, // base receiver
                        {0,0,0,0,2,2,2,2, 5,5,5,5,5,5,5,5}}; // error probability with attacker
int commValueMapping[16];
int commMappingCounter;

boolean laneSender, laneReceiver;
int counterRB,counterDB;
int posValue;

void initComm() {
  commMappingCounter = 0;
  setMapping();
  initLanes();
  counterRB=0;
  counterDB=0;
  initScore();
  delay(100);
}

void initScore() {
  for(int i=0; i<8; i++) {
    ledOff(commLEDs_RB[i]);
    ledOff(commLEDs_DB[i]);
  }
}

void runComm() {
  for(int i=0; i<2; i++) {
    if (evalButton(commButtons[i])) {
      //Serial.print("Comm Button: ");
      //Serial.println(commButtons[i]);
      if (!laneSender) { //i==0 && 
        laneSender=true;
        setLaneLeds(0);
        setLaneLeds(2);
      } else {
        //Serial.print("->");
        posValue = commValueMapping[commMappingCounter];
        //Serial.println(posValue);
        if (commValue[3][posValue]==0) { // correct value, right base -> green on RB
          ledOn(commLEDs_RB[counterRB],CRGB::Green);
          counterRB++;
        } else if (commValue[3][posValue]==5) { // different base, throw away value -> red on DB
          ledOn(commLEDs_DB[counterDB],CRGB::Red);
          counterDB++;
        } else if (commValue[3][posValue]==2) { // right base, attacked value, orange on RB (if attacker active, otherwise green)
          if (attackerOff) {
            ledOn(commLEDs_RB[counterRB],CRGB::Green);
          } else {
            ledOn(commLEDs_RB[counterRB],CRGB::Orange);
          }
          counterRB++;
        }
        commMappingCounter++;
        if(commMappingCounter>15) { //restart
          if (attackerOff) {//winning condition, display code
            // show blinking for some time
            currentBlending = LINEARBLEND;
            //SetupBlackAndWhiteStripedPalette();
            currentPalette = RainbowStripeColors_p; 
            showPalette(5000);
            FastLED.clear ();
            for(int i=0; i<4; i++) {
              if(code[i]) {
                  leds[commLEDs_RB[i*2]] = CRGB::Blue;
              } else {
                  leds[commLEDs_RB[i*2]] = CRGB::Red;
              }
            }
            FastLED.show();
            delay(5000);
            // all off, show code
            //Restart
            attackerOff=false;
            initSens();
          }
          initComm();
        }
        initLanes();
      }
    } 
  }
}

void initLanes() {
  laneSender=false;
  laneReceiver=false;
  for(int i=0; i<6; i++) {
    ledOff(commLEDs[i]);
  }
  if (!attackerOff) setLaneLeds(1);
}

void setLaneLeds(int i) {
  int posValue = commValueMapping[commMappingCounter];
  if (commValue[i][posValue] == 0) {
    ledOn(commLEDs[i], CRGB::White);
    ledOff(commLEDs[i+3]);
  } else {
    ledOff(commLEDs[i]);
    ledOn(commLEDs[i+3], CRGB::White);
  }
}

void setMapping() {
  for(int i=0; i<16; i++) {
    commValueMapping[i] = 17;
  }
  for(int i=0; i<16; i++) {
    int positionValue = random(0,16);
    boolean positionSet = false;
    while(!positionSet) {
      if (commValueMapping[positionValue]==17) {
        commValueMapping[positionValue] = i;
        positionSet=true;
      } else {
        positionValue++;
        if (positionValue>15) positionValue=0;
      }
    }
  }
  Serial.println("Mapping ");
  for(int i=0; i<16; i++) {
    Serial.print(commValueMapping[i]);
    Serial.print(", ");
  }
  Serial.println();
  Serial.println();
  for (int j=0; j<4; j++) {
    for(int i=0; i<16; i++) {
      Serial.print(commValue[j][commValueMapping[i]]);
      Serial.print(", ");
    }
    Serial.println();
  }
}
