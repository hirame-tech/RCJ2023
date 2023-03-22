void setup() {
  Serial.begin(115200);
}

void loop() {
  int angleb;
  int angley;
  char lr;
  char by;
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    angleb = input[0] - '0';
    angley = input[1] - '0';
    lr = input[2];
    by = input[3];

  }