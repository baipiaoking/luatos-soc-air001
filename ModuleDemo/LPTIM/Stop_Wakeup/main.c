#include "main.h"
static void RCC_Configuration(void);   // ����ʱ��
static void GPIO_Configuration(void);  // ����GPIO
static void LPTIM_Configuration(void); // ����LPTIM


int main(void)
{
  /* ����ϵͳʱ�� */
  RCC_Configuration();

  /* ʹ��LPTIM��PWRʱ�� */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPTIM1);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  // GPIO��ʼ��
  GPIO_Configuration();
  /* ���ò�ʹ��LPTIM */
  LPTIM_Configuration();

  // �ȴ���������
  while (LL_GPIO_IsInputPinSet(GPIOF, LL_GPIO_PIN_4) == 0)
  {
  }
  // �ر�GPIOB0
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);

  while (1)
  {
    LL_PWR_EnableLowPowerRunMode(); // ʹ�ܵ͹�������ģʽ
    LL_LPTIM_Disable(LPTIM1);       // �ر�LPTIM
    LL_LPTIM_Enable(LPTIM1);        // ʹ��LPTIM
    LL_uDelay(70);                  // ��ʱ70us

    LL_LPTIM_StartCounter(LPTIM1, LL_LPTIM_OPERATING_MODE_ONESHOT); // ����LPTIM

    LL_LPM_EnableDeepSleep(); // ����STOPģʽ���ȴ��жϻ���
    __WFI();                  // ����͹���ģʽ
  }
}

static void LPTIM_Configuration(void)
{
  LL_RCC_LSI_Enable();              // ʹ��LSIʱ��
  while (LL_RCC_LSI_IsReady() == 0) // �ȴ�LSIʱ�Ӿ���
  {
  }
  LL_RCC_SetLPTIMClockSource(LL_RCC_LPTIM1_CLKSOURCE_LSI);          // LPTIMʱ��Դѡ��LSI
  LL_LPTIM_SetPrescaler(LPTIM1, LL_LPTIM_PRESCALER_DIV32);          // LPTIMԤ��Ƶѡ��32��Ƶ, 32KHz / 32 = 1KHz, Ҳ����1ms
  LL_LPTIM_SetUpdateMode(LPTIM1, LL_LPTIM_UPDATE_MODE_ENDOFPERIOD); // LPTIM����ģʽѡ�����ڽ�������
  LL_LPTIM_EnableIT_ARRM(LPTIM1);                                   // ʹ��LPTIM�Զ���װ��ƥ���ж�

  NVIC_EnableIRQ(LPTIM1_IRQn);      // ʹ��LPTIM�ж�
  NVIC_SetPriority(LPTIM1_IRQn, 0); // ����LPTIM�ж����ȼ�

  LL_LPTIM_Enable(LPTIM1); // ʹ��LPTIM

  LL_LPTIM_SetAutoReload(LPTIM1, 1000 + 1); // ����LPTIM�Զ���װ��ֵΪ1000, Ҳ����1s
}

void LPTIM1_IRQHandler(void)
{
  if (LL_LPTIM_IsActiveFlag_ARRM(LPTIM) == 1) // �ж�LPTIM�Զ���װ��ƥ���жϱ�־�Ƿ���λ
  {
    LL_LPTIM_ClearFLAG_ARRM(LPTIM); // ���LPTIM�Զ���װ��ƥ���жϱ�־

    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_0); // GPIOB0�����ת
  }
}
// GPIO����
static void GPIO_Configuration(void)
{
  // ʹ��GPIOB��GPIOFʱ��
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
  // ����GPIOB0Ϊ���ģʽ
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
  // GPIOB0����ߵ�ƽ
  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0);
  // ����GPIOF4Ϊ����ģʽ
  LL_GPIO_SetPinMode(GPIOF, LL_GPIO_PIN_4, LL_GPIO_MODE_INPUT);
}

static void RCC_Configuration(void)
{
  LL_RCC_HSI_Enable();                                  // ʹ��HSI
  LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_24MHz); // ����HSIΪ24MHz
  while (LL_RCC_HSI_IsReady() != 1)                     // �ȴ�HSI����
  {
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);                           // ����AHB��Ƶ
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);                   // ����ϵͳʱ��ԴΪHSI
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS) // �ȴ�ϵͳʱ��Դ�л�ΪHSI
  {
  }
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1); // ����APB1��Ƶ

  SystemCoreClockUpdate(); // ����ϵͳʱ�ӣ����º��ϵͳʱ�Ӵ����SystemCoreClock��
}

// ����Ϊ��������
void APP_ErrorHandler(void)
{
  // ����
  while (1)
  {
  }
}
