## Practica2: Interrupció per GPIO

En aquesta practica implementarem les interrupcions i veurem quan va be utilitzar-les i quan no

# Codi

```
#include <Arduino.h>

struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

Button button1 = {18, 0, false};

void IRAM_ATTR isr() {
  button1.numberKeyPresses += 1;
  button1.pressed = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);
}

void loop() {
  if (button1.pressed) {
    Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
    button1.pressed = false;
  }
//Detach Interrupt after 1 Minute
static uint32_t lastMillis = 0;
if (millis() - lastMillis > 60000) {
  lastMillis = millis();
  detachInterrupt(button1.PIN);
  Serial.println("Interrupt Detached!");
  }
}
```

# Explicació del codi

Dins l'estructura Button tenim tres variables: numero de pin, numero de pulsacions i booleà que indica si esta sent pulsat:

```
struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};
```

Definim el botó button1 amb les tres variables (num. de pin = 18, num. de pulsacions = 0, booleà = false)

```
Button button1 = {18, 0, false};
```

Declarem l'acció d'interrupció (el codi que s'executarà en el moment de la interrupció):

```
void IRAM_ATTR isr() {
  button1.numberKeyPresses += 1;
  button1.pressed = true;
}
```
**Set up:**
```
void setup() {
  Serial.begin(115200);
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);
}

```
**Loop:**
```
void loop() {
  if (button1.pressed) {
    Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
    button1.pressed = false;
  }
```

Quan el botó porti més de 60000ms (1min) sense ser pulsat s'apagarà, és a dir, s'imprimirà per pantalla "Interrupt Detached!" i caldrà fer un reset per tornar a posar en funcionament el programa.

```
static uint32_t lastMillis = 0;

  if (millis() - lastMillis > 60000) {
    lastMillis = millis();
    detachInterrupt(button1.PIN);
    Serial.println("Interrupt Detached!");
  }
}
```

## Practica 2b: Interrupció per Timer

En aquesta part de la practica les interrupcions seràn controlades per un "timer"

# Codi

```
#include <Arduino.h>

// Declaramos las variables que usaremos, la de la interrupción puntual y el contador total
volatile int interruptCounter;
int totalInterruptCounter;

// Creamos un puntero para el temporizador 
hw_timer_t * timer = NULL;
// I una variable para sincronizarllo todo
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}
 
void setup() {
 
  Serial.begin(115200);
 
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
 
}
 
void loop() {
 
  if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;
 
    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
  }
}
```

# Explicació del codi

En les interrupcions per timer és el propi programa el que interrumpeix amb un contador de temps fixat per nosaltres, en aquest cas sera cada 1 segon, i surtirà per pantalla el nombre d'interrupcions que porta el programa.

Ara declararem les variables:

La primera variable s'utilitzarà per comunicar el codi de la interrupció amb el codi del loop i servirà per tenir un registre de si s'ha produït una interrupció. Aquesta es una variable de tipus volatile, això vol dir que aquesta s'emmagetzemarà a la memòria RAM.

La segona variable és un contador total d'interrupcions

Les dues variables de continuació s'utilitzen per a poder sincronitzar la interrupció i el procés del loop

```
volatile int interruptCounter;

int totalInterruptCounter;

hw_timer_t * timer = NULL;

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

```

El codi de continuació sumarà ++ al contador de interrupcions cada cop que s'en produeixi una.


```
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}

```

**Set up:**

```
void setup() {
 
  Serial.begin(115200);
 
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
 
}
```

**Loop:**

```
void loop() {
 
  if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;
 
    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
  }
}
```

