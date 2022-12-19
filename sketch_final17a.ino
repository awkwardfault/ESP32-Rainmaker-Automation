#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"

// ------------------
// libraries
// ------------------
#include <AceButton.h>
using namespace ace_button;
#include <SimpleTimer.h>
#include <Preferences.h>
Preferences pref;

// ------------------------ 
// bluetooth configuration
// ------------------------
const char *service_name = "ESP32 IOT";
const char *pop = "alpha123";

// ---------------------------------
// nodename
// ---------------------------------
char nodeName[] = "ESP32 IOT";


// -------------------------------
// gpio for relay
// -------------------------------
static uint8_t RELAY_1 = 5;
static uint8_t RELAY_2 = 18;
static uint8_t RELAY_3 = 19;
static uint8_t RELAY_4 = 21;


// ------------------------------
// gpio for manual switch
// ------------------------------
static uint8_t switch1 = 32;
static uint8_t switch2 = 33;
static uint8_t switch3 = 25;
static uint8_t switch4 = 26;


// -------------------------
// reset pin and led pin
// -------------------------
static uint8_t WIFI_LED    = 2;
static uint8_t gpio_reset = 0;

// ------------------------------------
// Name of the device shown in the App
// ------------------------------------
char device1[] = "Switch1";
char device2[] = "Switch2";
char device3[] = "Switch3";
char device4[] = "Switch4";

// --------------------------------------
// Relay State
// --------------------------------------
bool STATE_RELAY_1 = LOW;
bool STATE_RELAY_2 = LOW;
bool STATE_RELAY_3 = LOW;
bool STATE_RELAY_4 = LOW;

// --------------------------------------
// Last Relay State
// --------------------------------------
bool switch_state_ch1 = LOW;
bool switch_state_ch2 = LOW;
bool switch_state_ch3 = LOW;
bool switch_state_ch4 = LOW;

//---------------------------------------------------
//The framework provides some standard device types 
//like switch, lightbulb, fan, temperature sensor.
// --------------------------------------------------
static Switch my_switch1(device1, &RELAY_1);
static Switch my_switch2(device2, &RELAY_2);
static Switch my_switch3(device3, &RELAY_3);
static Switch my_switch4(device4, &RELAY_4);

// ------------------------------------
// acebutton
// ------------------------------------
ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);
ButtonConfig config3;
AceButton button3(&config3);
ButtonConfig config4;
AceButton button4(&config4);
ButtonConfig config4;

void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);
void handleEvent3(AceButton*, uint8_t, uint8_t);
void handleEvent4(AceButton*, uint8_t, uint8_t);


// ------------------------------
// sysProvEvent Function
// ------------------------------
void sysProvEvent(arduino_event_t *sys_event)
{
    switch (sys_event->event_id) {      
        case ARDUINO_EVENT_PROV_START:
#if CONFIG_IDF_TARGET_ESP32
        Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on BLE\n", service_name, pop);
        printQR(service_name, pop, "ble");
#else
        Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on SoftAP\n", service_name, pop);
        printQR(service_name, pop, "softap");
#endif        
        break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        Serial.printf("\nConnected to Wi-Fi!\n");
        digitalWrite(WIFI_LED, HIGH);
        break;
    }
}

// -----------------------------------------------
// write_callback Function
// -----------------------------------------------
void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx)
{
    const char *device_name = device->getDeviceName();
    const char *param_name = param->getParamName();
    //----------------------------------------------------------------------------------
    if(strcmp(device_name, device1) == 0) {
      
      Serial.printf("Switch value = %s\n", val.val.b? "true" : "false");
      
      if(strcmp(param_name, "Power") == 0) {
        //Serial.printf("Received value = %s for %s - %s\n", val.val.b? "true" : "false", device_name, param_name);
        STATE_RELAY_1 = val.val.b;
        STATE_RELAY_1 = !STATE_RELAY_1;
        // control_relay(1, RELAY_1, STATE_RELAY_1);
        (STATE_RELAY_1 == false) ? digitalWrite(RELAY_1, HIGH) : digitalWrite(RELAY_1, LOW);
        param->updateAndReport(val);
      }
    }
    //----------------------------------------------------------------------------------
    else if(strcmp(device_name, device2) == 0) {
      
      Serial.printf("Switch value = %s\n", val.val.b? "true" : "false");

      if(strcmp(param_name, "Power") == 0) {
        //Serial.printf("Received value = %s for %s - %s\n", val.val.b? "true" : "false", device_name, param_name);
        STATE_RELAY_2 = val.val.b;
        STATE_RELAY_2 = !STATE_RELAY_2;
        // control_relay(2, RELAY_2, STATE_RELAY_2);
        (STATE_RELAY_2 == false) ? digitalWrite(RELAY_2, HIGH) : digitalWrite(RELAY_2, LOW);
        param->updateAndReport(val);
      }
    }
    //----------------------------------------------------------------------------------
    else if(strcmp(device_name, device3) == 0) {
      
      Serial.printf("Switch value = %s\n", val.val.b? "true" : "false");

      if(strcmp(param_name, "Power") == 0) {
        //Serial.printf("Received value = %s for %s - %s\n", val.val.b? "true" : "false", device_name, param_name);
        STATE_RELAY_3 = val.val.b;
        STATE_RELAY_3 = !STATE_RELAY_3;
        // control_relay(3, RELAY_3, STATE_RELAY_3);        
        (STATE_RELAY_3 == false) ? digitalWrite(RELAY_3, HIGH) : digitalWrite(RELAY_3, LOW);
        param->updateAndReport(val);
      }
  
    }
    //----------------------------------------------------------------------------------
    else if(strcmp(device_name, device4) == 0) {
      
      Serial.printf("Switch value = %s\n", val.val.b? "true" : "false");

      if(strcmp(param_name, "Power") == 0) {
        //Serial.printf("Received value = %s for %s - %s\n", val.val.b? "true" : "false", device_name, param_name);
        STATE_RELAY_4 = val.val.b;
        STATE_RELAY_4 = !STATE_RELAY_4;
        // control_relay(4, RELAY_4, STATE_RELAY_4);
        (STATE_RELAY_4 == false) ? digitalWrite(RELAY_4, HIGH) : digitalWrite(RELAY_4, LOW);
        param->updateAndReport(val);
      } 
    }
    //---------------------------------------------------------------------------------- 
}

// -----------------------------------------------
// This function will recall the last state
// -----------------------------------------------
void getRelayState()
{
  STATE_RELAY_1 = pref.getBool("Relay1", 0);
  Serial.print("Last State Relay1 - "); Serial.println(STATE_RELAY_1);
  digitalWrite(RELAY_1, STATE_RELAY_1);
  my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, STATE_RELAY_1);
  delay(200);
  STATE_RELAY_2 = pref.getBool("Relay2", 0);
  Serial.print("Last State Relay2- "); Serial.println(STATE_RELAY_2);
  digitalWrite(RELAY_2, STATE_RELAY_2);
  my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, STATE_RELAY_2);
  delay(200);
  STATE_RELAY_3 = pref.getBool("Relay3", 0);
  Serial.print("Last State Relay3- "); Serial.println(STATE_RELAY_3);
  digitalWrite(RELAY_3, STATE_RELAY_3);
  my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, STATE_RELAY_3);
  delay(200);
  STATE_RELAY_4 = pref.getBool("Relay4", 0);
  Serial.print("Last State Relay4- "); Serial.println(STATE_RELAY_4);
  digitalWrite(RELAY_4, STATE_RELAY_4);
  my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, STATE_RELAY_4);
  delay(200);
}
// ---------------------------
// setup functon
// ---------------------------
void setup()
{
  uint32_t chipId = 0;
  Serial.begin(115200);
  pref.begin("Relay_State", false);
  // ------------------------------------
  // Set the Relays GPIOs as output mode
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);

  // Configure the input GPIOs
  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  pinMode(switch3, INPUT_PULLUP);
  pinMode(switch4, INPUT_PULLUP);

  // ------------------------------------------
  pinMode(gpio_reset, INPUT);
  pinMode(WIFI_LED, OUTPUT);
  digitalWrite(WIFI_LED, LOW);

  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);
  config3.setEventHandler(button3Handler);
  config4.setEventHandler(button4Handler);

  button1.init(switch1);
  button2.init(switch2);
  button3.init(switch3);
  button4.init(switch4);

  Node my_node;
  my_node = RMaker.initNode("ESP32 IOT");

  //Add switch device to the node
  my_switch1.addCb(write_callback);
  my_node.addDevice(my_switch1);
  my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch1);
  delay(500);

  my_switch2.addCb(write_callback);
  my_node.addDevice(my_switch2);
  my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch2);
  delay(500);

  my_switch3.addCb(write_callback);
  my_node.addDevice(my_switch3);
  my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch3);
  delay(500);

  my_switch4.addCb(write_callback);
  my_node.addDevice(my_switch4);
  my_switch4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch4);
  delay(500);

  // -----------------------------------------------------------------------------------
  // Timer
  // -----------------------------------------------------------------------------------
  //This is optional
  RMaker.enableOTA(OTA_USING_PARAMS);
  //If you want to enable scheduling, set time zone for your region using setTimeZone().
  //The list of available values are provided here https://rainmaker.espressif.com/docs/time-service.html
  RMaker.setTimeZone("Asia/Bangkok");
  // Alternatively, enable the Timezone service and let the phone apps set the appropriate timezone
  RMaker.enableTZService();
  RMaker.enableSchedule();

    //------------------------------------------------------------------------------
  //Service Name
  for(int i=0; i<17; i=i+8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }

  Serial.printf("\nChip ID:  %d Service Name: %s\n", chipId, service_name);
  //------------------------------------------------------------------------------
  Serial.printf("\nStarting ESP-RainMaker\n");
  RMaker.start();
  //------------------------------------------------------------------------------
  WiFi.onEvent(sysProvEvent);
  #if CONFIG_IDF_TARGET_ESP32
    WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
  #else
    WiFiProv.beginProvision(WIFI_PROV_SCHEME_SOFTAP, WIFI_PROV_SCHEME_HANDLER_NONE, WIFI_PROV_SECURITY_1, pop, service_name);
  #endif
  // -----------------------------------------------------------------------------

  // Get the last state of Relays
  getRelayState(); 

  my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  my_switch4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);

  Serial.printf("Relay1 is %s \n", STATE_RELAY_1? "ON" : "OFF");
  Serial.printf("Relay2 is %s \n", STATE_RELAY_2? "ON" : "OFF");
  Serial.printf("Relay3 is %s \n", STATE_RELAY_3? "ON" : "OFF");
  Serial.printf("Relay4 is %s \n", STATE_RELAY_4? "ON" : "OFF");

}

void loop() {

  button1.check();
  button2.check();
  button3.check();
  button4.check();

  // Read GPIO0 (external button to reset device
  if(digitalRead(gpio_reset) == LOW) { //Push button pressed
    Serial.printf("Reset Button Pressed!\n");
    // Key debounce handling
    delay(100);
    int startTime = millis();
    while(digitalRead(gpio_reset) == LOW) delay(50);
    int endTime = millis();
    //_______________________________________________________________________
    if ((endTime - startTime) > 10000) {
      // If key pressed for more than 10secs, reset all
      Serial.printf("Reset to factory.\n");
      RMakerFactoryReset(2);
    } 
    //_______________________________________________________________________
    else if ((endTime - startTime) > 3000) {
      Serial.printf("Reset Wi-Fi.\n");
      // If key pressed for more than 3secs, but less than 10, reset Wi-Fi
      RMakerWiFiReset(2);
    }
    //_______________________________________________________________________
  }
  
  delay(100);
  if (WiFi.status() != WL_CONNECTED){
    //Serial.println("WiFi Not Connected");
    digitalWrite(WIFI_LED, LOW);
  }
  else{
    //Serial.println("WiFi Connected");
    digitalWrite(WIFI_LED, HIGH);
  }
  //------------------------------------------------------------------------------
  // button_control();
  // remote_control();

}

// --------------------------------------------------------------
//functions for defineing manual switch
// --------------------------------------------------------------
void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  //EspalexaDevice* d1 = espalexa.getDevice(0);
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      switch_state_ch1 = true;
      my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch1);
      digitalWrite(RELAY_1, HIGH);
      pref.putBool("RELAY_1", STATE_RELAY_1);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      switch_state_ch1 = false;
      my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch1);
      digitalWrite(RELAY_1, LOW);
      pref.putBool("RELAY_1", STATE_RELAY_1);
      break;
  }
}
void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT2");
  //EspalexaDevice* d2 = espalexa.getDevice(1);
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      switch_state_ch2 = true;
      my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch2);
      digitalWrite(RELAY_2, HIGH);
      pref.putBool("RELAY_2", STATE_RELAY_2);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      switch_state_ch2 = false;
      my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch2);
      digitalWrite(RELAY_2, LOW);
      pref.putBool("RELAY_2", STATE_RELAY_2);
      break;
  }
}
void button3Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT3");
  //EspalexaDevice* d3 = espalexa.getDevice(2);
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      switch_state_ch3 = true;
      my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch3);
      digitalWrite(RELAY_3, HIGH);
      pref.putBool("RELAY_3", STATE_RELAY_3);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      switch_state_ch3 = false;
      my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch3);
      digitalWrite(RELAY_3, LOW);
      pref.putBool("RELAY_3", STATE_RELAY_3);
      break;
  }
}
void button4Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT4");
  //EspalexaDevice* d4 = espalexa.getDevice(3);
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      switch_state_ch4 = true;
      my_switch4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch4);
      digitalWrite(RELAY_4, HIGH);
      pref.putBool("RELAY_4", STATE_RELAY_4);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      switch_state_ch4 = false;
      my_switch4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch4);
      digitalWrite(RELAY_4, LOW);
      pref.putBool("RELAY_4", STATE_RELAY_4);
      break;
  }
}
