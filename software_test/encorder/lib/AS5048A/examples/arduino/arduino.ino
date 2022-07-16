#include <AS5048A.h>

AS5048A angleSensor(10, true);

void setup()
{
	Serial.begin(19200);
	angleSensor.begin();
}

void loop()
{
	delay(1000);

	float val = angleSensor.getRotationInDegrees();
	Serial.print("\nGot rotation of: ");
	Serial.println(val);
	Serial.println("State: ");
	angleSensor.printState();
	Serial.println("Errors: ");
	Serial.println(angleSensor.getErrors());
	Serial.println("Diagnostics: ");
	Serial.println(angleSensor.getDiagnostic());
}
