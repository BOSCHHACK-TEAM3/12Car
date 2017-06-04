/**
 * This software is copyrighted by Bosch Connected Devices and Solutions GmbH, 2016.
 * The use of this software is subject to the XDK SDK EULA
 */

/* module includes ********************************************************** */

/* system header files */
#include "BCDS_Basics.h"
#include "BCDS_WlanConnect.h"
#include "BCDS_NetworkConfig.h"
#include "PAL_initialize_ih.h"
#include "PAL_socketMonitor_ih.h"


/* additional interface header files */
#include "stdio.h"
#include "FreeRTOS.h"
#include "timers.h"

#include "em_cmu.h"
#include "ADC_ih.h"
#include "XdkGpioConfig.h"
#include "PTD_portDriver_ph.h"
#include "PTD_portDriver_ih.h"
#include "ADC_ih.h"                 /* added for ADC */

/* own header files */
#include "XDKManagement.h"
#include "AcousticDetectApp.h"

/* constant definitions ***************************************************** */
#define PAL_IP_ADDRESS_SIZE UINT8_C(15) /*< Maximum size of the ip address */

/* local variables ********************************************************** */
ADC_Init_TypeDef adc0_init_conf = ADC_INIT_DEFAULT;
ADC_InitSingle_TypeDef adc0_singleinit_conf = ADC_INITSINGLE_DEFAULT;

static uint8_t current_sample = 0;
static uint32_t sample_buffer[50];

/* global variables ********************************************************* */


/* inline functions ********************************************************* */

/* local functions ********************************************************** */

uint32_t sound_level_calc(uint32_t mic_sample) {
	uint8_t i = 0;

	sample_buffer[current_sample] = mic_sample * mic_sample;
	current_sample = (current_sample + 1) % 50;

	for (i = 0, noiseLevel = 0; i < 50; i++) {
		noiseLevel += sample_buffer[i];
	}

	return noiseLevel;
}

void Read_ADC(xTimerHandle pxTimer) {
	(void) (pxTimer);
	uint32_t ADC_read_value;
	ADC_singleAcq_t ADC_acquire_one = { ADC0, adc0_singleinit_conf,
			adcSingleInpCh4, 0UL };

	ADC_singleAcq_tp ADC_acquire_one_p = &ADC_acquire_one;
	ADC_pollSingleData(ADC_acquire_one_p);
	ADC_read_value = ADC_scaleAdcValue(ADC_acquire_one_p);
	uint32_t v = sound_level_calc(ADC_read_value);
	if (current_sample == 0) {
		printf("PD4 = %ld \n\r", (long int) (v));
	}
}
//void mylog(void  *pParameters) {
//	(void) pParameters;
//	vTaskDelay(SECONDS(3));
//	uint8_t clock = ADC_PrescaleCalc(32000, 0);
//	printf("real ADC frequence = %d \n\r", clock);
//	vTaskDelete(NULL);
//}
//void createLogTask(void) {
//	xTaskHandle taskHandle = NULL;
//	xTaskCreate(mylog, // function that implements the task
//			(const char * const) "My Task",// a name for the task
//			configMINIMAL_STACK_SIZE,// depth of the task stack
//			NULL,// parameters passed to the function
//			tskIDLE_PRIORITY,// task priority
//			&taskHandle// pointer to a task handle for late reference
//			);
//}
/**
 * @brief Configure the ADC to scan multiple channels;
 *        Initialize and Start a time to scan the ADC
 */
void initAcousticModule(void) {
	adc0_init_conf.warmUpMode = adcWarmupKeepADCWarm;

	adc0_singleinit_conf.prsSel = adcPRSSELCh0;
	adc0_singleinit_conf.reference = adcRef2V5;
	adc0_singleinit_conf.input = adcSingleInpCh4;
	adc0_singleinit_conf.acqTime = adcAcqTime16;

	PTD_pinOutSet(PTD_PORT_AKU340_VDD, PTD_PIN_AKU340_VDD);

	ADC_node_t ADC_node_one = { ADC0, cmuClock_ADC0, 32000, ADC_INIT_DEFAULT };
	ADC_node_tp ADC_node_one_p = &ADC_node_one;
	ADC_init(ADC_node_one_p);

	/*Task for reading the ADC*/
	xTimerHandle timer_read_adc = xTimerCreate((const char *) "Reading ADC",
	ONESECONDDELAY, TIMER_AUTORELOAD_ON, NULL, Read_ADC);

	xTimerStart(timer_read_adc, TIMERBLOCKTIME);


//	createLogTask();

}


/* global functions ********************************************************* */


/** ************************************************************************* */
