#include <ModbusMaster.h>
#include "REG_CONFIG.h"
#include <HardwareSerial.h>
#include "HardwareSerial_NB_BC95.h"
#include <TaskScheduler.h>
#define _TASK_TIMECRITICAL
HardwareSerial modbus(2);
#define NUMSLAVES 10
int SlaveCnt = 0;

#define CHANNEL 3
#define PRINTSCANRESULTS 0
ModbusMaster node;
void t1CallgetMeter();
Task t1(10000, TASK_FOREVER, &t1CallgetMeter);
Scheduler runner;

struct Meter
{
  String cA;
  String cB;
  String cC;
  String cN;
  String cG;
  String cAvg;
  String vAB;
  String vBC;
  String vCA;
  String vAN;
  String vBN;
  String vCN;

  String vLLAvg;
  String vLNAvg;
  String freq;
  String apTotal;

  String Current_Avg_1;
  String Current_Avg_2;
  String Current_Avg_3;
  String Voltage_Avg_1 ;
  String Voltage_Avg_2 ;
  String Voltage_Avg_3 ;
  String Frequency_1;
  String Frequency_2;
  String Frequency_3;
  String apTotal_1;
  String apTotal_2;
  String apTotal_3;
};
Meter meter;
signal meta ;

void setup()
{

  Serial.begin(115200);
  modbus.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println(F("Starting... Sintrol Monitor"));

  // communicate with Modbus slave ID 1 over Serial (port 2)
  Serial.println();
  Serial.println(F("***********************************"));


  runner.init();
  Serial.println("Initialized scheduler");
  runner.addTask(t1);
  Serial.println("added t1");
  Serial.println("added t2");
  delay(2000);
  t1.enable();  Serial.println("Enabled t1");
}

void readMeter2()
{
  Serial.println("++++++++++ device 2 ++++++++++++");
  meter.Current_Avg_1 = read_Modbus(c_Avg);
  meter.Voltage_Avg_1 = read_Modbus(v_LN_Avg);
  meter.Frequency_1 = read_Modbus(v_Freq);
  meter.apTotal_1 = read_Modbus(ap_Total);
  Serial.print("Current Avg: ");
  Serial.print(meter.Current_Avg_1);
  Serial.println(" Amp");
  Serial.print("Volt AVG: ");
  Serial.print(meter.Voltage_Avg_1);
  Serial.println(" Volt");
  Serial.print("Frequency: ");
  Serial.print(meter.Frequency_1);
  Serial.println(" Hz");
  Serial.print("meter.apTotal_1 : ");
  Serial.print(meter.apTotal_1);
  Serial.println(" kW");
  Serial.println("++++++++++ device 2 ++++++++++++");
  delay(2000);
}
void readMeter3()
{
  Serial.println("++++++++++ device 3 ++++++++++++");
  meter.Current_Avg_2 = read_Modbus(c_Avg);
  meter.Voltage_Avg_2 = read_Modbus(v_LN_Avg);
  meter.Frequency_2 = read_Modbus(v_Freq);
  meter.apTotal_2 = read_Modbus(ap_Total);
  Serial.print("Current Avg: ");
  Serial.print(meter.Current_Avg_2);
  Serial.println(" Amp");
  Serial.print("Volt AVG: ");
  Serial.print(meter.Voltage_Avg_2);
  Serial.println(" Volt");
  Serial.print("Frequency: ");
  Serial.print(meter.Frequency_2);
  Serial.println(" Hz");
  Serial.print("meter.apTotal_2 : ");
  Serial.print(meter.apTotal_2);
  Serial.println(" kW");
  Serial.println("++++++++++ device 3 ++++++++++++");
  delay(2000);
}
void readMeter4()
{
  Serial.println("++++++++++ device 4 ++++++++++++");
  meter.Current_Avg_3 = read_Modbus(c_Avg);
  meter.Voltage_Avg_3 = read_Modbus(v_LN_Avg);
  meter.Frequency_3 = read_Modbus(v_Freq);
  meter.apTotal_3 = read_Modbus(ap_Total);
  Serial.print("Current Avg: ");
  Serial.print(meter.Current_Avg_3);
  Serial.println(" Amp");
  Serial.print("Volt AVG: ");
  Serial.print(meter.Voltage_Avg_3);
  Serial.println(" Volt");
  Serial.print("Frequency: ");
  Serial.print(meter.Frequency_3);
  Serial.println(" Hz");
  Serial.print("meter.apTotal_3 : ");
  Serial.print(meter.apTotal_3);
  Serial.println(" kW");
  Serial.println("++++++++++ device 4 ++++++++++++");
  delay(2000);
}

void t1CallgetMeter() {     // Update read all data
  node.begin(2, modbus);
  readMeter2();
  node.begin(3, modbus);
  readMeter3();
  node.begin(4, modbus);
  readMeter4();
}


float HexTofloat(uint32_t x)
{
  return (*(float*)&x);
}


float read_Modbus(uint16_t  REG)
{
  static uint32_t i;
  uint8_t j, result;
  uint16_t data[2];
  uint32_t value = 0;
  float val = 0.0;

  // slave: read (6) 16-bit registers starting at register 2 to RX buffer
  result = node.readHoldingRegisters(REG, 2);

  // do something with data if read is successful
  if (result == node.ku8MBSuccess)
  {
    for (j = 0; j < 2; j++)
    {
      data[j] = node.getResponseBuffer(j);
    }
    value = data[0];
    value = value << 16;
    value = value + data[1];

    val = HexTofloat(value);

    return val;
  } else {
    Serial.print("Connec modbus fail. REG >>> "); Serial.println(REG, HEX); // Debug
    //    delay(1000);
    return 0;
  }

}

void loop() {
  runner.execute();

}
