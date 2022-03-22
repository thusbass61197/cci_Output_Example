/* ************************************************************************ */
/*!
   \file

   \brief          VT client demo application

   \author         Erwin Hammerl
   \date           Created 17.09.2007

   \copyright  Wegscheider Hammerl Ingenieure Partnerschaft

   \par HISTORY:

*/
/* **************************  includes ********************************** */

#include "IsoDef.h"

#ifdef _LAY6_  /* compile only if VT client is enabled */


#include "VIEngine.h"
#include "App_VTClientLev2.h"   // -> Object defines

#include "MyProject1.iop.h"
#include "MyProject1.c.h"

#include "settings.h"

#include "esp_log.h"

static const char *TAG = "App_VTClientLev2";

#include "driver/gpio.h"







#include "TimeLib.h"
#include "StandardLib.h"
#include "UtilLib.h"
#include "ExtraLib.h"
#include "BasicLib.h"


//********************************************************************************************
// Each command has several Unique Features. here they are encapsulated !
//********************************************************************************************
typedef struct {
	//
	//! The Commandnumber is here redundant, however a Testdriver can check for Validity !!!
	//
	iso_u16 objectIdOfAuxObject; // objectIdOfAuxObject
	iso_u16 objectIdOfButtonObject; // objectIdOfButtonObject

} CommandTranslateObject;

CommandTranslateObject CommandTranslate[] = {
	{ AuxFunction2_Q1,			SoftKey_Q1		}, //	0
	{ AuxFunction2_Q2,			SoftKey_Q2		}, //	1
	{ AuxFunction2_Q3,			SoftKey_Q3		}, //	2
	{ AuxFunction2_Q4,			SoftKey_Q4		}, //	3
	{ AuxFunction2_29005,		SoftKey_5000		}, //	4
	{ AuxFunction2_29006,		SoftKey_5005		}, //	5
	{ AuxFunction2_29007,		SoftKey_5006		}, //	6
	{ AuxFunction2_29008,		SoftKey_5007		}, //	7
	{ AuxFunction2_29009,		SoftKey_5007		} //	8
};


//make this Defines (Right side)
const int FIRST_AUX = AuxFunction2_Q1;
const int  LAST_AUX = AuxFunction2_29009;
//do not Change this.
const int  NUM_AUX_FUNCTIONS = ((LAST_AUX-FIRST_AUX)+1);
iso_s32 InputSignalData_old_value1[20] = {AUX_PRESS_OFF};








// called from AppPoolSettings()
void VTC_setPoolManipulation(const ISOVT_EVENT_DATA_T* psEvData)
{
   iso_u16  u16DM_Scal  = 10000u;          // Scaling factor * 10000
   iso_u16  u16SKM_Scal = 10000u;

   // ------------------------------------------------------------------------------



   u16DM_Scal = (iso_u16)IsoVtcPoolReadInfo(psEvData->u8Instance, PoolDataMaskScalFaktor);       // Call only after PoolInit !!
   u16SKM_Scal = (iso_u16)IsoVtcPoolReadInfo(psEvData->u8Instance, PoolSoftKeyMaskScalFaktor);



   u16DM_Scal /= 2;
   u16SKM_Scal /= 2;

   ESP_LOGW(TAG, "PoolDataMaskScalFaktor    =  %i", u16DM_Scal);
   ESP_LOGW(TAG, "PoolSoftKeyMaskScalFaktor =  %i", u16SKM_Scal);


	// ------------------------------------------------------------------------------




	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance,  1000u,  1999u, u16DM_Scal, Scaling);        // DataMask
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance,  2000u,  2999u, u16DM_Scal, Scaling);        // AlarmMask
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance,  3000u,  3499u, u16DM_Scal, Scaling);        // Container
																								// SoftKeyMask
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance,  6000u,  6999u, u16DM_Scal, Scaling);        // Button
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance,  7000u,  7999u, u16DM_Scal, Scaling);        // InputBoolean
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance,  8000u,  8999u, u16DM_Scal, Scaling);        // InputString
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance,  9000u,  9999u, u16DM_Scal, Scaling);        // InputNumber
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 10000u, 10999u, u16DM_Scal, Scaling);        // InputList
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 11000u, 11999u, u16DM_Scal, Scaling);        // OutputString
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 12000u, 12999u, u16DM_Scal, Scaling);        // OutputNumber
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 13000u, 13499u, u16DM_Scal, Scaling);        // Line
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 14000u, 14499u, u16DM_Scal, Scaling);        // Rectangle
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 15000u, 15499u, u16DM_Scal, Scaling);        // Ellipse
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 16000u, 16499u, u16DM_Scal, Scaling);        // Polygon
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 17000u, 17499u, u16DM_Scal, Scaling);        // Meter
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 18000u, 18499u, u16DM_Scal, Scaling);        // LinearBargraph
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 19000u, 19499u, u16DM_Scal, Scaling);        // ArchedBargraph
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 20000u, 20499u, u16DM_Scal, Scaling);        // PictureGraphic
																								// ObjectPointer
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 23000u, 23999u, u16DM_Scal, Scaling);        // FontAttributes
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 24000u, 24499u, u16DM_Scal, Scaling);        // LineAttributes
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 25000u, 25499u, u16DM_Scal, Scaling);        // FillAttributes
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 30000u, 30999u, u16DM_Scal, Scaling);        // OutputList

	/*HANDLE SOFTKEY MANIPULATION*/

	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance,     0u,     0u, u16SKM_Scal, Centering);     // Working set object
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance,  3500u,  3999u, u16SKM_Scal, Scaling);       // Container
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance,  5000u,  5999u, u16SKM_Scal, Centering);     // Softkeys
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 13500u, 13999u, u16SKM_Scal, Scaling);       // Line
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 14500u, 14999u, u16SKM_Scal, Scaling);       // Rectangle
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 15500u, 15999u, u16SKM_Scal, Scaling);       // Ellipse
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 16500u, 16999u, u16SKM_Scal, Scaling);       // Polygon
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 20500u, 20999u, u16SKM_Scal, Scaling);       // Working set bitmaps
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 24500u, 24900u, u16SKM_Scal, Scaling);       // LineAttributes
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 25000u, 25499u, u16SKM_Scal, Scaling);       // FillAttributes
	IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 29000u, 29999u, u16SKM_Scal, Centering);     // Auxiliary function


	// LineAttributes	24901-24999 is unscaled !






	if (IsoVtcGetStatusInfo(psEvData->u8Instance, VT_VERSIONNR) == VT_V2_FE)
	{
		// Transforming Auxiliary function Type 2 into Type 1
		IsoVtcPoolSetIDRangeMode(psEvData->u8Instance, 29000, 29999, 0, AuxToV2);
	}
}














void VTC_handleNumericValues(const struct InputNumber_S * pInputNumberData) {
	// what number was entered
	switch (pInputNumberData->objectIdOfInputNumber) {



	default:
		break;
	}

}


void VTC_setNewVT(const ISOVT_EVENT_DATA_T* psEvData)
{
   // ------------------------------------------------------------------------------

   PARAM_UNUSED(psEvData->u8Instance);


   // ------------------------------------------------------------------------------
}

void VTC_setPoolReady(const ISOVT_EVENT_DATA_T* psEvData)
{

	// beim n�chsten Anstecken des Ger�tes muss der letzte gespeicherte Wert auf das Display gesendet werden.

}



#define BUTTON_I1 GPIO_NUM_32		// Pin 32.
#define BUTTON_I2 GPIO_NUM_26		// Pin 26.
#define GPIO_Q1 GPIO_NUM_19
#define GPIO_Q2 GPIO_NUM_23
#define GPIO_Q3 GPIO_NUM_33
#define GPIO_Q4 GPIO_NUM_25


void init_GPIO(void)
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_reset_pin(GPIO_Q1);
    gpio_reset_pin(GPIO_Q2);
    gpio_reset_pin(GPIO_Q3);
    gpio_reset_pin(GPIO_Q4);
    gpio_reset_pin(BUTTON_I1);
    gpio_reset_pin(BUTTON_I2);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(GPIO_Q1, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_Q2, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_Q3, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_Q4, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON_I1, GPIO_MODE_INPUT);
    gpio_set_direction(BUTTON_I2, GPIO_MODE_INPUT);
    gpio_set_level(GPIO_Q1, 0);
    gpio_set_level(GPIO_Q2, 0);
    gpio_set_level(GPIO_Q3, 0);
    gpio_set_level(GPIO_Q4, 0);
}



static int I1 = 0;
static int I2 = 0;
CYCLE_8_TAP CYCLE_8A;

/* ************************************************************************ */
void AppVTClientDoProcess(const ISOVT_EVENT_DATA_T* psEvData)
{  /* Cyclic VTClient function */



	// to achieve this:

	//IsoVtcCmd_CtrlAudioSignal(u8Instance, 0, 700, 500, 0);
	//vTaskDelay(pdMS_TO_TICKS(500));
	//IsoVtcCmd_CtrlAudioSignal(u8Instance, 0, 940, 1000, 0);


	I1 = !gpio_get_level(BUTTON_I1);
	I2 = !gpio_get_level(BUTTON_I2);
	CYCLE_8A.T2=3000;
	CYCLE_8A(I1);														//Frikwenz und Dauer
	if (CYCLE_8A.Q0) IsoVtcCmd_CtrlAudioSignal(psEvData->u8Instance, 1, 700,  500, 0);
	if (CYCLE_8A.Q1) IsoVtcCmd_CtrlAudioSignal(psEvData->u8Instance, 1, 940, 1000, 0);
	if (CYCLE_8A.Q2) IsoVtcCmd_CtrlAudioSignal(psEvData->u8Instance, 1, 1030, 500, 0);
	if (CYCLE_8A.Q3) IsoVtcCmd_CtrlAudioSignal(psEvData->u8Instance, 1, 700,  500, 0);
	if (CYCLE_8A.Q4) IsoVtcCmd_CtrlAudioSignal(psEvData->u8Instance, 1, 940, 1000, 0);
	if (CYCLE_8A.Q5) IsoVtcCmd_CtrlAudioSignal(psEvData->u8Instance, 1, 1030, 500, 0);
	if (CYCLE_8A.Q6) IsoVtcCmd_CtrlAudioSignal(psEvData->u8Instance, 1, 700,  500, 0);


}

RS RS1;
TON TON1;
void VTC_handleSoftkeysAndButton_Q1(const struct ButtonActivation_S *pButtonData) {

	TON1.PT = 3000;
	switch (pButtonData->keyActivationCode) {



	case BUTTON_STATE_HELD:
		TON1(true);
		RS1(TON1.Q,false);
		break;

	case BUTTON_STATE_PRESSED:
		TON1(false);
		RS1(false, true);
		break;

	case BUTTON_STATE_RELEASED:
	case BUTTON_STATE_ABORTED:
		break;


	}
	gpio_set_level(GPIO_Q1, RS1.Q1);
}

void VTC_handleSoftkeysAndButton_Q2(const struct ButtonActivation_S *pButtonData) {

	switch (pButtonData->keyActivationCode) {


	case BUTTON_STATE_PRESSED:
	case BUTTON_STATE_HELD:
		gpio_set_level(GPIO_Q2, 1);
		break;


	case BUTTON_STATE_RELEASED:
	case BUTTON_STATE_ABORTED:
		gpio_set_level(GPIO_Q2, 0);
		break;


	}
}

RS RS3;
TON TON3;
void VTC_handleSoftkeysAndButton_Q3(const struct ButtonActivation_S *pButtonData) {

	TON3.PT = 3000;
	switch (pButtonData->keyActivationCode) {



	case BUTTON_STATE_HELD:
		TON3(true);
		RS3(TON3.Q,false);
		break;

	case BUTTON_STATE_PRESSED:
		TON3(false);
		RS3(false, true);
		break;

	case BUTTON_STATE_RELEASED:
	case BUTTON_STATE_ABORTED:
		break;


	}

	gpio_set_level(GPIO_Q3, RS3.Q1);


	}


void VTC_handleSoftkeysAndButton_Q4(const struct ButtonActivation_S *pButtonData) {

	switch (pButtonData->keyActivationCode) {


	case BUTTON_STATE_PRESSED:
	case BUTTON_STATE_HELD:
		gpio_set_level(GPIO_Q4, 1);
		break;


	case BUTTON_STATE_RELEASED:
	case BUTTON_STATE_ABORTED:
		gpio_set_level(GPIO_Q4, 0);
		break;


	}
}



void VTC_handleSoftkeysAndButtons(const struct ButtonActivation_S *pButtonData)
{

	ESP_LOGI(TAG, "press");

	// what button was operated
	switch (pButtonData->objectIdOfButtonObject) {
	case SoftKey_Q1:
		VTC_handleSoftkeysAndButton_Q1(pButtonData);
		break;
	case SoftKey_Q2:
		VTC_handleSoftkeysAndButton_Q2(pButtonData);
		break;
	case SoftKey_Q3:
		VTC_handleSoftkeysAndButton_Q3(pButtonData);
		break;
	case SoftKey_Q4:
		VTC_handleSoftkeysAndButton_Q4(pButtonData);
		break;
	}

}

void VTC_handleAux(const struct AUX_InputSignalData_T *InputSignalData) {
	struct ButtonActivation_S pButtonData;


	// Application may decide to lookup the object ID of the function assigned to this input
	// and then check the status data reported...
	//functionObjID = <maybe you have a lookup to get function object Id from (InputSignalData.inputObjectID)>;

	// Store the sprintf format string in HUGE ROM.
	//static SprinthfFormatChar_T format[] = "Pressed = %s, Status= %i\n";


	if (InputSignalData->objectIdOfAuxObject < FIRST_AUX)
	{
		return;
	}



	pButtonData.objectIdOfButtonObject = CommandTranslate[InputSignalData->objectIdOfAuxObject - FIRST_AUX].objectIdOfButtonObject;
	pButtonData.wPara2 = (iso_u16)InputSignalData->value1;
	pButtonData.u8Instance = InputSignalData->u8Instance;

	switch (InputSignalData->value1) {
	case AUX_PRESS_OFF:
		switch (InputSignalData_old_value1[InputSignalData->objectIdOfAuxObject - FIRST_AUX]) {
		case AUX_PRESS_OFF:
			//NOOP!
			break;
		default:
			pButtonData.keyActivationCode = BUTTON_STATE_RELEASED;
			VTC_handleSoftkeysAndButtons(&pButtonData);
			break;
		}
		break;
	case AUX_PRESS_MOMENTARY:
		pButtonData.keyActivationCode = BUTTON_STATE_PRESSED;
		VTC_handleSoftkeysAndButtons(&pButtonData);
		break;
	case AUX_PRESS_HELD:
		switch (InputSignalData_old_value1[InputSignalData->objectIdOfAuxObject - FIRST_AUX]) {
		case AUX_PRESS_OFF:
			pButtonData.keyActivationCode = BUTTON_STATE_PRESSED;
			VTC_handleSoftkeysAndButtons(&pButtonData);
			break;
		default:
			pButtonData.keyActivationCode = BUTTON_STATE_HELD;
			VTC_handleSoftkeysAndButtons(&pButtonData);
			break;
		}
		break;
	default:
		break;

	}

	InputSignalData_old_value1[InputSignalData->objectIdOfAuxObject - FIRST_AUX] = InputSignalData->value1;
}



/* ************************************************************************ */
#endif /* _LAY6_ */
/* ************************************************************************ */
