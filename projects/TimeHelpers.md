# Implementing `Time8A`

```cpp
#include <Felix8A.h>

const unsigned long timeInterval = 1000; // 1 sec
unsigned long lastUpdate = 0;

void setup() {
  // setup code here
}

void loop() {
  if (Time8A::every(timeInterval, lastUpdate)) {
    // loop code here
  }
}
```

## Similar Behavior:

```cpp
#include <Felix8A.h>

const unsigned long timeInterval = 1000; // 1 sec
unsigned long lastUpdate = 0;

void setup() {
  Time8A::reset(lastUpdate); // start timer
  // setup code here
}

void loop() {
  if (Time8A::after(timeInterval, lastUpdate)) {
    // loop code here
    Time8A::reset(lastUpdate); // restart timer manually
  }
}
```

## Implementing `Time8A` Inside Functions

```cpp
void myFunction() {
  static unsigned long lastUpdate = 0; // persists between calls

  if (Time8A::every(500, lastUpdate)) {
    // code
  }
}
```

## `Time8A` As a One-shot Timer

```cpp
#include <Felix8A.h>

unsigned long startTime = 0;
bool done = false;

void setup() {
  Serial.begin(115200);
  Time8A::reset(startTime);
}

void loop() {
  if (!done && Time8A::after(3000, startTime)) {
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
  Time8A::arm(startTime);
}

void loop() {
  if (Time8A::once(3000, startTime)) {
    Serial.println("3 seconds passed!");
  }
}
```
