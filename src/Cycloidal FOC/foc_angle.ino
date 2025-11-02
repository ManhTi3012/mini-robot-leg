#include <SimpleFOC.h>
#include <Wire.h>

// ===== Hardware Setup =====
// Motor: 7 pole pairs (example)
BLDCMotor motor = BLDCMotor(7);

// Driver: 6-PWM (pins for AH, AL, BH, BL, CH, CL)
BLDCDriver6PWM driver = BLDCDriver6PWM(PA8, PA7, PA9, PB0, PC2, PB1, PA0);

// Magnetic encoder (AS5600 I2C)
MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);

// Use USART3 for Serial I/O (PB10 TX, PB11 RX)
HardwareSerial Serial3(USART3);

// Commander for serial control
Commander command = Commander(Serial3);
void doMotor(char* cmd) { command.motor(&motor, cmd); }


// ===== Setup =====
void setup() {
  // --- Serial setup ---
  Serial3.setTx(PB10);
  Serial3.setRx(PB11);
  Serial3.begin(115200);
  SimpleFOCDebug::enable(&Serial3);

  Serial3.println("Starting STM32 SimpleFOC + AS5600 setup...");

  // --- I2C setup (PC9 SDA, PC8 SCL for I2C3) ---
  Wire.setSDA(PB5);
  Wire.setSCL(PC8);
  Wire.begin();
  Wire.setClock(400000);

  // --- Initialize AS5600 sensor ---
  sensor.init();
  motor.linkSensor(&sensor);

  // --- Driver setup ---
  driver.voltage_power_supply = 18;
  driver.voltage_limit = 18;     // limit to protect your 6Ω motor (~0.8 A)
  if (!driver.init()) {
    Serial3.println("Driver init failed!");
    while (1);
  }
  motor.linkDriver(&driver);

  // --- Motor setup ---
  motor.voltage_sensor_align = 3;
  motor.PID_velocity.output_ramp = 3000;

  motor.P_angle.P = 20; 
  motor.P_angle.I = 1.0;  // usually only P controller is enough 
  motor.P_angle.D = 0.0002;  // usually only P controller is enough 

  motor.PID_velocity.P = 0.22;
  motor.PID_velocity.I = 0.6;
  motor.PID_velocity.D = 0.00035;

  motor.controller = MotionControlType::angle;
  motor.torque_controller = TorqueControlType::voltage;

  if (!motor.init()) {
    Serial3.println("Motor init failed!");
    while (1);
  }

  // --- Run FOC alignment ---
  if (!motor.initFOC()) {
    Serial3.println("FOC init failed!");
    while (1);
  }

  // --- Monitoring and Commander ---
  motor.useMonitoring(Serial3);
  command.add('M', doMotor, "Motor");
  command.add('VP', [](char* cmd){ command.scalar(&motor.PID_velocity.P, cmd); }, "vel.P");
  command.add('VI', [](char* cmd){ command.scalar(&motor.PID_velocity.I, cmd); }, "vel.I");
  command.add('VD', [](char* cmd){ command.scalar(&motor.PID_velocity.D, cmd); }, "vel.D");
  command.add('VR', [](char* cmd){ command.scalar(&motor.PID_velocity.output_ramp, cmd); }, "vel.ramp");


  motor.target = 0;  // initial torque target (Volts)

  Serial3.println("Motor ready!");
  Serial3.println("Use 'M' command to control via Serial (e.g. M2.5)");
  _delay(1000);
}


// ===== Main Loop =====
void loop() {
  // FOC algorithm
  motor.loopFOC();
  // Motion control
  motor.move();

  // Serial Commander
  command.run();
}
