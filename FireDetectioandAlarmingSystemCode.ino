#include <Arduino_FreeRTOS.h> 
 
#define alarm_pin 6 
#define led_pin 13 
#define IR_sensor_pin 2
#define flame_sensor_pin 10 
#define gas_sensor_pin A0 
#define sensor_thresh 1000
#define relay  8
 
int FlameDetected; 
int GasDetected ;
int IRDetected; 
static int led_delay = 500;

/* 3 question to be answered . 
 *  How to set priority (change priority using vSetTaskpriority using handler)?
 *  What is the event to be processed on notification?
 *  Does this code Work???????
 *  
 */

void FireSuppression (void *pvParameters); 
void ActivateCooler (void *pvParameters);
void IRSensor (void *pvParameters); 
void GasSensor (void *pvParameters);
void FlameSensor (void *pvParameters);



TaskHandle_t xGasTask = NULL, xFireTask = NULL;


void setup() {  
  
Serial.begin(9600);
Serial.println(("In Setup function"));   


pinMode(led_pin,OUTPUT); 
pinMode(flame_sensor_pin,INPUT);  
pinMode(alarm_pin,OUTPUT);
pinMode(relay, OUTPUT);
digitalWrite(relay, HIGH);





xTaskCreate( FlameSensor,"FlameSensor",200,NULL,5,NULL );
xTaskCreate(FireSuppression ,"FireSuppression",200,NULL,4,&xFireTask ); 
xTaskCreate(GasSensor,"GasSensor",200,NULL,3,NULL); 
xTaskCreate(ActivateCooler,"ActivateCooler",200,NULL,2,&xGasTask );
xTaskCreate(IRSensor,"IRSensor",200,NULL,1,NULL );

   vTaskStartScheduler();
} 




 
void loop() 
{}









void ActivateCooler (void *pvParameters) 
{  uint32_t ulNotifiedValueGas;
  for (;;) 
  { ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
   xTaskNotifyWaitIndexed(0,0x00,0xffffffff,&ulNotifiedValueGas,portMAX_DELAY );
    
      if( ( ulNotifiedValueGas & 0x01 ) != 0 )
    {
    digitalWrite(alarm_pin,HIGH);
    digitalWrite(led_pin, HIGH);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(led_delay / portTICK_PERIOD_MS); 
    }
  /*else
  {
    
  }*/
  }
  //vTaskDelete( NULL );
} 
 
void FireSuppression (void *pvParameters) 
{ 
     uint32_t ulNotifiedValueFire;
    for (;;) 
    { 
      ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
      xTaskNotifyWaitIndexed(0,0x00,0xffffffff,&ulNotifiedValueFire,portMAX_DELAY );
    
      if( ( ulNotifiedValueFire & 0x01 ) != 0 )
            {
               digitalWrite(relay, LOW);
            }
          /*  else
            {
              
            }*/
      
       
    } 
    //vTaskDelete( NULL );
} 
 
 
void FlameSensor (void *pvParameters) 
{   
  TickType_t xFireLastWakeTime;
  const TickType_t xFireFrequency = 1000;
  xFireLastWakeTime = xTaskGetTickCount ();
  BaseType_t xFireWasDelayed;
  for (;;) 
  { 
      if(digitalRead(flame_sensor_pin)==1)
      {
        xTaskNotifyGive( xFireTask );
      }
    xFireWasDelayed = xTaskDelayUntil( &xFireLastWakeTime, xFireFrequency );
  }
  //vTaskDelete( NULL );
}



void GasSensor (void *pvParameters) 
{ TickType_t xGasLastWakeTime;
  const TickType_t xGasFrequency = 1000;
  xGasLastWakeTime = xTaskGetTickCount ();
  BaseType_t xGasWasDelayed; 
  
  for (;;) 
  { 
    if(digitalRead(gas_sensor_pin)==1)
    {
      xTaskNotifyGive( xGasTask );
    }

   xGasWasDelayed =   xTaskDelayUntil( &xGasLastWakeTime, xGasFrequency );
  }
  //vTaskDelete( NULL );
} 

void IRSensor (void *pvParameters) 
{ TickType_t xIRLastWakeTime;
  const TickType_t xIRFrequency = 1000;
  xIRLastWakeTime = xTaskGetTickCount ();
  BaseType_t xIRWasDelayed; 
  
  for (;;) 
  { 
   IRDetected = digitalRead(IR_sensor_pin);
   xIRWasDelayed =   xTaskDelayUntil( &xIRLastWakeTime, xIRFrequency );
  }
  //vTaskDelete( NULL );
} 
