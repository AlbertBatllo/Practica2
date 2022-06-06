# Practica2: Interrupció per GPIO

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
