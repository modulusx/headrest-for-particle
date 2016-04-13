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

String publishString;
bool publishPending = false;

void setup()
{
  Particle.function("led", ledControl);
  Particle.function("notify",notifyPebble);

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

    publishString = "{\"alert\":\"" + String(led1) + "\",\"info\":\"New State:";
    if (led1State)
      publishString += "On";
    else
      publishString += "Off";
    publishString += "\"}";
    publishPending = true;
  }
  if (publishPending)
  {
    char tempString[publishString.length()+1];
    publishString.toCharArray(tempString, publishString.length()+1);
    Particle.publish("SSEvent",tempString);
    publishPending = false;
    publishString = "";
  }
}

int notifyPebble(String command)
{
  if (command != "")
  {
    publishString = "{\"alert\":\"1\",\"info\":\"" + command + "\"}";
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

    publishString = "{\"led\":\"" + String(led1) + "\",\"state\":\"";
    if (led1State)
      publishString += "On";
    else
      publishString += "Off";
    publishString += "\"}";
    publishPending = true;
  }
  return led1State;
}
