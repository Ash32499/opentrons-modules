#ifndef ThermistorsADC_h
#define ThermistorsADC_h

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_ADS1015.h>

#define ADC_TABLE_SIZE 291
#define ADC_PER_DEVICE 4

#define ADDRESS_A 0x48
#define ADDRESS_B 0x49

#define TOTAL_GAIN_SETTINGS 6

#define GAIN_TWOTHIRDS_VOLTAGE   6.144
#define GAIN_ONE_VOLTAGE         4.096
#define GAIN_TWO_VOLTAGE         2.048
#define GAIN_FOUR_VOLTAGE        1.024
#define GAIN_EIGHT_VOLTAGE       0.512
#define GAIN_SIXTEEN_VOLTAGE     0.256

#if HW_VERSION >=3
  #define ADC_INDEX_HEAT_SINK            0
  #define ADC_INDEX_PLATE_FRONT_LEFT     1
  #define ADC_INDEX_PLATE_FRONT_CENTER   2
  #define ADC_INDEX_PLATE_FRONT_RIGHT    3
  #define ADC_INDEX_PLATE_BACK_LEFT      4
  #define ADC_INDEX_COVER                5
  #define ADC_INDEX_PLATE_BACK_RIGHT     6
  #define ADC_INDEX_PLATE_BACK_CENTER    7
  // TODO: Test the use of adc alerts for i2c updates
  #define PIN_ADC_1_ALERT              30
  #define PIN_ADC_2_ALERT              31
#else
  #define ADC_INDEX_HEAT_SINK            0
  #define ADC_INDEX_PLATE_FRONT_RIGHT    1  //Bottom Right on sch.
  #define ADC_INDEX_PLATE_FRONT_CENTER   2  //Bottom Center on sch.
  #define ADC_INDEX_PLATE_FRONT_LEFT     3  //Bottom Lef on sch.
  #define ADC_INDEX_PLATE_BACK_LEFT      4  //Top Left on sch.
  #define ADC_INDEX_PLATE_BACK_CENTER    5  //Top Center on sch.
  #define ADC_INDEX_PLATE_BACK_RIGHT     6  //Top Right on sch.
  #define ADC_INDEX_COVER                7
#endif

#define TOTAL_THERMISTORS              8
#define TOTAL_PLATE_THERMISTORS        6

// When thermistor is disconnected, the value at open ckt = -20
// But we don't want to reach low -ve or  high +ve values either..
#define THERMISTOR_ERROR_VAL_LOW  -10
#define THERMISTOR_ERROR_VAL_HI   120

enum class ThermistorPair
{
    left=0,
    center,
    right,
    cover_n_heatsink
};

typedef struct
{
  int adc_reading;
  float celsius;
}AdcToCelsius;

class ThermistorsADC{
      public:

            ThermistorsADC();
            void setup(float voltage);
            void update(ThermistorPair n);

            float average_plate_temperature();
            float left_pair_temperature();
            float center_pair_temperature();
            float right_pair_temperature();
            float front_left_temperature();
            float front_center_temperature();
            float front_right_temperature();
            float back_left_temperature();
            float back_center_temperature();
            float back_right_temperature();
            float cover_temperature();
            float hottest_plate_therm_temp();
            float coolest_plate_therm_temp();
            float heat_sink_temperature();
            float plate_temp_offset;
            bool detected_invalid_val;

      private:

            Adafruit_ADS1115 *adc_a;
            Adafruit_ADS1115 *adc_b;

            uint16_t _read_adc(int index);
            float _adc_to_celsius(uint16_t _adc);
            bool is_in_range(double celsius);

            double probe_temps[TOTAL_THERMISTORS] = {0, };
            double sum_probe_temps[TOTAL_THERMISTORS] = {0, };
            double probe_sample_count = 0;

            const int temp_read_interval = 100;
            const int inter_temp_read_interval = 1;
            unsigned long temp_read_timestamp = 0;

            adsGain_t gain_settings[6] = {
                  GAIN_TWOTHIRDS,
                  GAIN_ONE,
                  GAIN_TWO,
                  GAIN_FOUR,
                  GAIN_EIGHT,
                  GAIN_SIXTEEN
            };

            float gain_max_voltage[6] = {
                  GAIN_TWOTHIRDS_VOLTAGE,
                  GAIN_ONE_VOLTAGE,
                  GAIN_TWO_VOLTAGE,
                  GAIN_FOUR_VOLTAGE,
                  GAIN_EIGHT_VOLTAGE,
                  GAIN_SIXTEEN_VOLTAGE
            };

            // lookup table provided for thermistor PN: KS103J2
            // ADC values calculated for when powering 1.5 volts into
            // a 10k ohm resistor, followed by the thermistor leading to GND
            static const AdcToCelsius TABLE[ADC_TABLE_SIZE];
};

#endif
