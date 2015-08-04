/*

# EXAMPLE REQUEST IN TERMINAL
# Core ID is 0123456789abcdef
# Your access token is 123412341234
curl https://api.spark.io/v1/devices/0123456789abcdef/led \
  -d access_token=123412341234 \
  -d params=1

*/

int led1State = LOW;
const int led1 = D7;
int switch1LastState = HIGH;
const int switch1 = D6;

char publishString[64];
bool publishPending = false;

void setup()
{
  Spark.function("led", ledControl);
  Spark.function("notify",notifyPebble);

  pinMode(switch1, INPUT_PULLUP);

  pinMode(led1, OUTPUT);
  digitalWrite(led1, led1State);
}

void loop()
{
  if (digitalRead(switch1) != switch1LastState)
  {
    led1State = !led1State;
    switch1LastState = !switch1LastState;
    digitalWrite(led1,led1State);

    sprintf(publishString,"{\"alert\": \"%d\", \"info\": \"New State: %s\"}",led1,(led1State)? "On" : "Off");
    publishPending = true;
  }
  if (publishPending)
  {
    Spark.publish("SSEvent",publishString);
    publishPending = false;
  }
}

int notifyPebble(String command)
{
  if (command == "1")
  {
    sprintf(publishString,"{\"alert\": \"%d\", \"info\": \"%s\"}",1,"HELLO WORLD!");
    publishPending = true;
  }
  return 1;
}

int ledControl(String command)
{
  if (led1State != (command == "1") ? HIGH : LOW)
  {
    led1State = !led1State;
    digitalWrite(led1,led1State);

    sprintf(publishString,"{\"led\": \"%d\", \"state\": \"%s\"}",led1,(led1State)? "On" : "Off");
    publishPending = true;
  }

  return led1State;
}
