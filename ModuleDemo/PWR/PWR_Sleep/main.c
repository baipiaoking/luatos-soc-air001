#include "main.h"
static void RCC_Configuration(void);  // ����ʱ��
static void GPIO_Configuration(void); // ����GPIO

int main(void)
{
  /* ����ϵͳʱ�� */
  RCC_Configuration();
  // ����SysTick
  LL_Init1msTick(SystemCoreClock);
  // GPIO��ʼ��
  GPIO_Configuration();

  // �ȴ���������
  while (LL_GPIO_IsInputPinSet(GPIOF, LL_GPIO_PIN_4) == 0)
  {
  }
  // �ر�GPIOB0
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR); // ʹ��PWRʱ��
  LL_LPM_EnableSleep(); // ����˯��ģʽ
  __WFI();                  // ����͹���ģʽ
  while (1)
  {
    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_0); // ��תLED
    LL_mDelay(500);
  }
}

// ��ʼ��GPIO
static void GPIO_Configuration(void)
{
  // ʹ��GPIOBʱ��
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
  // ���������м�ĵ�������PB0����ʼ��PB0Ϊ�������
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
  // GPIOB0����ߵ�ƽ
  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0);
  // ��ʼ��PF4Ϊ���룬�ⲿ����������
  LL_GPIO_SetPinMode(GPIOF, LL_GPIO_PIN_4, LL_GPIO_MODE_INPUT);

  LL_EXTI_InitTypeDef EXTI_InitStruct;              // �ⲿ�жϽṹ��
  EXTI_InitStruct.Line = LL_EXTI_LINE_4;            // �ⲿ�ж���
  EXTI_InitStruct.LineCommand = ENABLE;             // ʹ���ⲿ�ж���
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;           // �ж�ģʽ
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING; // �����ش���
  LL_EXTI_Init(&EXTI_InitStruct);                   // ��ʼ���ⲿ�ж�
  LL_EXTI_EnableIT(LL_EXTI_LINE_4);                 // ʹ���ⲿ�ж�

  LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTF, LL_EXTI_CONFIG_LINE4); // �����ⲿ�ж��ߵ�ԴΪPF4
  NVIC_SetPriority(EXTI4_15_IRQn, 1);                                // �����ж����ȼ�
  NVIC_EnableIRQ(EXTI4_15_IRQn);                                     // ʹ���ж�
}
// �ⲿ�жϷ�����
void EXTI4_15_IRQHandler(void)
{
  if (LL_EXTI_IsActiveFlag(LL_EXTI_LINE_4)) // �ж��ж���Դ
  {
    LL_EXTI_ClearFlag(LL_EXTI_LINE_4); // ����жϱ�־
  }
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
