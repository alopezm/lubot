#define DEBOUNCE_START_BTN 150
#define MAX_SUSPENSE_TIME 10000
#define MIN_SUSPENSE_TIME 5000
// PINS
#define BUTTON_B 13
#define BUTTON_G 12
#define LED_G 11
#define LED_B 10
#define SOUND 7
#define START 2
// STATES
#define STATE_STOP 0
#define STATE_SUSPENSE 1
#define STATE_WAITING 2
#define STATE_RESULT 3

unsigned long debounceStartBtn;
unsigned long startSuspenseTime;
unsigned long suspenseWait;
int resolution;
int state;
int winner;
int teamG;
int teamB;
  
void setup() {
  state = STATE_STOP;
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(SOUND, OUTPUT);
  pinMode(BUTTON_G, INPUT);
  pinMode(BUTTON_B, INPUT);
  pinMode(START, INPUT_PULLUP);
  randomSeed(A0);
  attachInterrupt(digitalPinToInterrupt(START), startGame, FALLING);
  reset();
}

void loop() {
  switch (state) {
    case STATE_STOP:
      reset();
      break;
    case STATE_SUSPENSE:
      suspenseGame();
      break;
    case STATE_WAITING:
      waitingGame();
      break;
    case STATE_RESULT:
      digitalWrite(winner, HIGH);
      break;
  }
}

void waitingGame() {
  teamG = digitalRead(BUTTON_G);
  teamB = digitalRead(BUTTON_B);
  if(teamG || teamB) {
    winner = teamG ? LED_G : LED_B;    
    state = STATE_RESULT;
  }
}

void suspenseGame() {
  resolution = max(int (suspenseWait - (millis() - startSuspenseTime)), 0);
  analogWrite(LED_G, map(resolution, 0, suspenseWait, 0 , 255));
  analogWrite(LED_B, map(resolution, 0, suspenseWait, 0 , 255));
  if (resolution <= 0) {
    digitalWrite(SOUND, HIGH);
    digitalWrite(SOUND, LOW);
    state = STATE_WAITING;
  }
}

void reset() {
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
  digitalWrite(SOUND, LOW);
  winner = 0;
}

void setSuspenseGame() {
  state = STATE_SUSPENSE;
  startSuspenseTime = millis();
  suspenseWait = random(MIN_SUSPENSE_TIME, MAX_SUSPENSE_TIME);
  reset();
}

void startGame() {
  if ((millis() - debounceStartBtn) > DEBOUNCE_START_BTN) {
    state = (state == STATE_STOP) ? STATE_SUSPENSE : STATE_STOP;
    debounceStartBtn = millis();
    if(state == STATE_SUSPENSE) {
      setSuspenseGame();
    }
  }
}
