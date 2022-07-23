#include<Arduino_FreeRTOS.h>



#define alarm_pin 30
#define Task1_led_pin 31
#define Task2_led_pin 32
#define Task3_led_pin 33
#define FireSuppression_led_pin 34
#define ActivateCooler_led_pin 35
#define IR_sensor_pin 2
#define flame_sensor_pin 3 
#define gas_sensor_pin 4
#define sensor_thresh 1000
#define relay  5
#define cooler_pin 6

int FlameDetected;
int GasDetected ;
int IRDetected;


void FireSuppression (void *pvParameters);
void ActivateCooler (void *pvParameters);
void IRSensor (void *pvParameters);
void GasSensor (void *pvParameters);
void FlameSensor (void *pvParameters);



TaskHandle_t xGasTask = NULL, xFireTask = NULL,xSenseTask =NULL;


void setup() {

  Serial.begin(115200);
  Serial.println(("In Setup function"));


  pinMode(Task1_led_pin,OUTPUT);
  pinMode(Task2_led_pin,OUTPUT);
  pinMode(FireSuppression_led_pin,OUTPUT);
  pinMode(ActivateCooler_led_pin,OUTPUT);
  pinMode(Task3_led_pin,OUTPUT);
  pinMode(flame_sensor_pin,INPUT);
  pinMode(alarm_pin,OUTPUT);
  pinMode(relay,OUTPUT);
  pinMode(cooler_pin,OUTPUT);
  





  xTaskCreate( FlameSensor, "FlameSensor", 200, NULL,3, &xSenseTask );

  xTaskCreate(FireSuppression, "FireSuppression", 200, NULL,3, &xFireTask );
  xTaskCreate(GasSensor, "GasSensor", 200, NULL,2, NULL);

  xTaskCreate(IRSensor, "IRSensor", 200, NULL,1, NULL );


  xTaskCreate(ActivateCooler, "ActivateCooler", 200, NULL,2, &xGasTask );

  vTaskStartScheduler();
}





void loop()
{}




void ActivateCooler (void *pvParameters)
{
  uint32_t ulNotifiedValueGas;
  for (;;)
  {
   
    ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
    xTaskNotifyWaitIndexed(0, 0x01, 0xffffffff, &ulNotifiedValueGas, portMAX_DELAY );
    Serial.println((" ActivateCooler function running....."));
    digitalWrite(ActivateCooler_led_pin, HIGH);   
    digitalWrite(FireSuppression_led_pin, LOW);
    digitalWrite(Task1_led_pin, LOW);
    digitalWrite(Task3_led_pin, LOW);
    digitalWrite(Task2_led_pin,LOW);
    if ( ( ulNotifiedValueGas & 0x01 ) != 0 )
    {
      digitalWrite(cooler_pin, HIGH);
      Serial.println("fan running....");
      
    
    }
    

    
    Serial.println((" ActivateCooler function exiting....."));
    
  }
  vTaskDelete( NULL );
}




void FireSuppression (void *pvParameters)
{
  uint32_t ulNotifiedValueFire;
  for (;;)
  {  
    ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
    xTaskNotifyWaitIndexed(0, 0x01, 0xffffffff, &ulNotifiedValueFire, portMAX_DELAY );
    Serial.println((" FireSuppression function running....."));
    digitalWrite(FireSuppression_led_pin, HIGH);
    digitalWrite(Task3_led_pin, LOW);
    digitalWrite(Task2_led_pin,LOW);
    digitalWrite(ActivateCooler_led_pin, LOW);
     
     
    if ( ( ulNotifiedValueFire & 0x01 ) != 0 )
    {
     digitalWrite(relay, LOW);
        if(IRDetected == 0)
     {digitalWrite(alarm_pin,HIGH);}
     //vTaskPrioritySet(NULL,3);
    }
    
   Serial.println((" FireSuppression function exiting....."));
  }
  vTaskDelete( NULL );
}


void FlameSensor (void *pvParameters)
{
  TickType_t xFireLastWakeTime;
  const TickType_t xFireFrequency = 100;//120,50,150
  xFireLastWakeTime = xTaskGetTickCount ();
  BaseType_t xFireWasDelayed;
  for (;;)
  {  xFireWasDelayed = xTaskDelayUntil( &xFireLastWakeTime, xFireFrequency );
    digitalWrite(Task1_led_pin, HIGH);
    digitalWrite(Task3_led_pin,LOW);
    digitalWrite(FireSuppression_led_pin,LOW);
    digitalWrite(ActivateCooler_led_pin, LOW);
    digitalWrite(Task2_led_pin, LOW);
    Serial.println((" Flame function running........"));
    int data = digitalRead(flame_sensor_pin);
    if ( data == 0)
    {
      
      xTaskNotifyGive( xFireTask );
    }
    else{digitalWrite(relay, HIGH);digitalWrite(alarm_pin,LOW);}
   
    Serial.println((" Flame function exiting........"));
 
    
  }
  vTaskDelete( NULL );
}



void GasSensor (void *pvParameters)
{
  TickType_t xGasLastWakeTime;
  const TickType_t xGasFrequency = 156;//170,100
  xGasLastWakeTime = xTaskGetTickCount();
  BaseType_t xGasWasDelayed;

  for (;;)
  { xGasWasDelayed =  xTaskDelayUntil( &xGasLastWakeTime, xGasFrequency );
    digitalWrite(Task2_led_pin, HIGH);
    digitalWrite(Task1_led_pin, LOW);
    digitalWrite(Task3_led_pin, LOW);
    digitalWrite(FireSuppression_led_pin,LOW);
    digitalWrite(ActivateCooler_led_pin, LOW);
    Serial.println((" Gas function running........"));
   int GasData = digitalRead(gas_sensor_pin);
    if (GasData ==1)
    {
      
      xTaskNotifyGive( xGasTask );
      
    }
    else
    {digitalWrite(cooler_pin, LOW);
    }
      
   
    Serial.println((" Gas function exiting........"));
    
   
  }
  vTaskDelete( NULL );
}


void IRSensor (void *pvParameters)
{
  TickType_t xIRLastWakeTime;
  const TickType_t xIRFrequency = 220;//220
  xIRLastWakeTime = xTaskGetTickCount ();
  BaseType_t xIRWasDelayed;

  for (;;)
  { xIRWasDelayed =   xTaskDelayUntil( &xIRLastWakeTime, xIRFrequency );
    digitalWrite(Task3_led_pin, HIGH);
     digitalWrite(Task2_led_pin, LOW);
    digitalWrite(Task1_led_pin, LOW);
    digitalWrite(FireSuppression_led_pin,LOW);
    digitalWrite(ActivateCooler_led_pin, LOW);
    Serial.println((" IR function running........"));
    IRDetected = digitalRead(IR_sensor_pin);
   
   
    Serial.println((" IR function exiting........"));
    
  }
  vTaskDelete( NULL );
}
