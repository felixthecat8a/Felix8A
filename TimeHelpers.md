
# Implementing `Felix8A::Time`

```cpp
#include <Felix8A.h>

const unsigned long timeInterval = 1000; // 1 sec
unsigned long lastUpdate = 0;

void setup() {
  // setup code here
}

void loop() {
  if (Felix8A::Time::every(timeInterval, lastUpdate)) {
    // loop code here
  }
}
```

## Similiar Behavior:

```cpp
#include <Felix8A.h>

const unsigned long timeInterval = 1000; // 1 sec
unsigned long lastUpdate = 0;

void setup() {
  Felix8A::Time::reset(lastUpdate); // start timer
  // setup code here
}

void loop() {
  if (Felix8A::Time::after(timeInterval, lastUpdate)) {
    // loop code here
    Felix8A::Time::reset(lastUpdate); // restart timer manually
  }
}
```

## Implementing `Felix8A::Time` Inside Functions

```cpp
void myFunction() {
  static unsigned long lastUpdate = 0; // persists between calls

  if (Felix8A::Time::every(500, lastUpdate)) {
    // code
  }
}
```

## `Felix8A::Time` As a One-shot Timers

```cpp
#include <Felix8A.h>

unsigned long startTime = 0;
bool done = false;

void setup() {
  Serial.begin(115200);
  Felix8A::Time::reset(startTime);
}

void loop() {
  if (!done && Felix8A::Time::after(3000, startTime)) {
    done = true;
    Serial.println("3 seconds passed!");
  }
}
```
```cpp
#include <Felix8A.h>

unsigned long startTime = 0;

void setup() {
  Serial.begin(115200);
  Felix8A::Time::arm(startTime);
}

void loop() {
  if (Felix8A::Time::once(3000, startTime)) {
    Serial.println("3 seconds passed!");
  }
}
```
