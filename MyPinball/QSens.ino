#define SENS_COOLDOWN 1500
#define MAX_SENS_SCORE 8

int sensScore;

// buttons
#define B_SENS_RR 8
#define B_SENS_RL 7
#define B_SENS_MR 6
#define B_SENS_ML 5
#define B_SENS_LR 4
#define B_SENS_LL 3
#define B_SENS_T  2

int sensButtons[] = {B_SENS_RR,B_SENS_RL,B_SENS_MR,B_SENS_ML,B_SENS_LR,B_SENS_LL,B_SENS_T};

// leds direct on upper targets
#define L_SENS_RR 36
#define L_SENS_RL 35
#define L_SENS_MR 34
#define L_SENS_ML 28
#define L_SENS_LR 27
#define L_SENS_LL 26
#define L_SENS_UR 40
#define L_SENS_UL 32

#define L_DIAM_RR 39
#define L_DIAM_RL 38
#define L_DIAM_MR 37
#define L_DIAM_ML 31
#define L_DIAM_LR 30
#define L_DIAM_LL 29
#define L_DIAM_UR 33
#define L_DIAM_UL 41

#define L_SENS_SCORE_R  15
#define L_SENS_SCORE_MR 16
#define L_SENS_SCORE_ML 25
#define L_SENS_SCORE_L  24

int sensLEDs[] = {L_SENS_RR,L_SENS_RL,L_SENS_MR,L_SENS_ML,L_SENS_LR,L_SENS_LL,L_SENS_UR,L_SENS_UL,
                  L_DIAM_RR,L_DIAM_RL,L_DIAM_MR,L_DIAM_ML,L_DIAM_LR,L_DIAM_LL,L_DIAM_UR,L_DIAM_UL,
                  };

void initSens() {
  sensScore = 0;
  updateSensScore();
}

void runSens() {
  for(int i=0; i<8; i++) {
    if (evalButton(sensButtons[i])) {
//      Serial.print("Sens Button: ");
//      Serial.println(sensButtons[i]);
      ledOn(sensLEDs[i],CRGB::White);
      ledOn(sensLEDs[i+8],CRGB::White);
      sensScore++;
      if (sensScore==MAX_SENS_SCORE) {
        attackerOff = true;
      }
      updateSensScore();
    } else {
      if (millis() > buttonsTime[sensButtons[i]]+SENS_COOLDOWN) {
        ledOff(sensLEDs[i]);
        ledOff(sensLEDs[i+8]);
      }
    }
  }
}

void updateSensScore() {
  switch (sensScore) {
    case 0:
      ledOn(L_SENS_SCORE_R,CRGB::Red);
      ledOn(L_SENS_SCORE_MR,CRGB::Red);
      ledOn(L_SENS_SCORE_ML,CRGB::Red);
      ledOn(L_SENS_SCORE_L,CRGB::Red);
      break;
    case 1:
      ledOn(L_SENS_SCORE_R,CRGB::Yellow);
      break;
    case 2:
      ledOn(L_SENS_SCORE_R,CRGB::Green);
      break;
    case 3:
      ledOn(L_SENS_SCORE_MR,CRGB::Yellow);
      break;
    case 4:
      ledOn(L_SENS_SCORE_MR,CRGB::Green);
      break;
    case 5:
      ledOn(L_SENS_SCORE_ML,CRGB::Yellow);
      break;
    case 6:
      ledOn(L_SENS_SCORE_ML,CRGB::Green);
      break;
    case 7:
      ledOn(L_SENS_SCORE_L,CRGB::Yellow);
      break;
    case 8:
      ledOn(L_SENS_SCORE_L,CRGB::Green);
      break;
  }
}
