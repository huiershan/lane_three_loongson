#ifndef _ADC7606_H
#define _ADC7606_H

#define ADC1_PATH  "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define ADC2_PATH  "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define ADC3_PATH  "/sys/bus/iio/devices/iio:device0/in_voltage2_raw"
#define ADC4_PATH  "/sys/bus/iio/devices/iio:device0/in_voltage3_raw"

#define ADC_SCALE_PATH  "/sys/bus/iio/devices/iio:device0/in_voltage_scale"

typedef struct{
	float scale;
	float adc1_current;
	float adc2_current;
	float adc3_current;
	float adc4_current;

	float sensor_1_k;
	float sensor_2_k;
	float sensor_3_k;
	float sensor_4_k;

	float sensor_1_value;
	float sensor_2_value;
	float sensor_3_value;
	float sensor_4_value;

	int sensor_1_error;
	int sensor_2_error;
	int sensor_3_error;
	int sensor_4_error;
}
adc_device_t;


int adc_scale_init(void);
int adc_channel_init(void);

int adc_value_read(int channel, int *value);
int adc_value_get(void);


#endif
