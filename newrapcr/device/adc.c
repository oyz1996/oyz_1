#include "adc_in.h"


static void ADC_RCC_Config(void)
{
	ADC_RCC_INIT;
	ADC_GPIO_RCC_INIT;

}
static void ADC_GPIO_Config(void)
{
	ADC_GPIO_INIT;
}

static void ADC_Mode_Config(void)
{
	
	ADC_InitTypeDef ADC_Struct;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	
	
	  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
	
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化

	ADC_StructInit(&ADC_Struct); 
	
	ADC_Struct.ADC_Resolution = ADC_Resolution_12b;
	ADC_Struct.ADC_ScanConvMode = DISABLE;
	ADC_Struct.ADC_ContinuousConvMode = DISABLE;
	ADC_Struct.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADC_Struct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Struct.ADC_NbrOfConversion = 1;

	ADC_Init(ADC1,&ADC_Struct);	
}
void ADC_Config(void)
{
	ADC_RCC_Config();
	ADC_GPIO_Config();
	ADC_Mode_Config();
	ADC_Cmd(ADC1,ENABLE);
}


static uint16_t Read_Channel(uint8_t channel)
{
	
		ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,采样时间为239.5周期	  		
	
		ADC_SoftwareStartConv(ADC1);
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
 
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果

}

static uint16_t Read_ADC(uint8_t channel)
{
	uint8_t num = 0;
	uint32_t  sum = 0;
	
	for(num = 0; num < 5 ; num++)
	{
		sum += Read_Channel(channel);
		vTaskDelay(5);
	}
	return sum/num;
}
double iui = 0;
double iuiu = 0;
double popo = 0;
double Read_Pin_Volt(_Volt_Pin Volt_Pin)
{

	uint32_t voltage = 0;
	double result = 0;
	double calcresist=10000;
	double NTCresit=0;
	
	switch(Volt_Pin)
	{
		case TEMP_TOP_HEAT:
			voltage = Read_ADC(TEMP_TOP_HEAT_CHIN);
			NTCresit=(double )calcresist/((4095/(double)voltage)-1);
			result = (double)1/((double)1/(273.15+25)+(double)1/3435*log(NTCresit/10000))-273.15;
		break;
		case TEMP_FIN:
			voltage = Read_ADC(TEMP_FIN_CHIN);
			NTCresit=calcresist/(((double)4095/(double)voltage)-1);
			result = (double)1/((double)1/(273.15+25)+(double)1/(double)3435*log(NTCresit/10000))-273.15;
		break;
		case Cur_Heatside:
		 voltage= Read_ADC(Cur_Heatside_CHIN);
		 result=3.269*(double)voltage/4095;
		break;
		case Cur_Heattop:
			voltage = Read_ADC(Cur_Heattop_CHIN);
		  result=3.269*(double)voltage/4095;
		break;
		case Cur_LAMP:
			voltage = Read_ADC(Cur_LAMP_CHIN);
			result=3.269*(double)voltage/4095;
		break;
	}
	return result;
}


