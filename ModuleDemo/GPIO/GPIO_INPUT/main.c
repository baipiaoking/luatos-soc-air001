#include "main.h"
static void RCC_Configuration(void);         // ����ʱ��
static void GPIO_Configuration(void);        // ����GPIO
LL_UTILS_ClkInitTypeDef UTILS_ClkInitStruct; // ϵͳʱ�ӽṹ��

// �ⲿ�ж�ʾ�����룬ʹ��GPIOF4��Ϊ�ⲿ�ж����룬�����ش����жϣ��жϷ������н�GPIOB0�����ת

int main(void)
{
  // ����ʱ��
  RCC_Configuration();
  // ����GPIO
  GPIO_Configuration();
  while (1)
  {
  }
}
// ����ʱ��
static void RCC_Configuration(void)
{
  LL_RCC_HSI_Enable();                                  // ʹ��HSI
  LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_24MHz); // ����HSIΪ24MHz
  while (LL_RCC_HSI_IsReady() != 1)                     // �ȴ�HSI����
  {
  }

  LL_PLL_ConfigSystemClock_HSI(&UTILS_ClkInitStruct); // ����HSIΪϵͳʱ��

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1); // ����AHB��Ƶ

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);                   // ����ϵͳʱ��ԴΪPLL
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) // �ȴ�ϵͳʱ��Դ�л�ΪPLL
  {
  }

  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1); // ����APB1��Ƶ

  SystemCoreClockUpdate(); // ����ϵͳʱ�ӣ����º��ϵͳʱ�Ӵ����SystemCoreClock��
}
// ��ʼ��GPIO
static void GPIO_Configuration(void)
{
  // ʹ��GPIOBʱ��
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
  // ���������м�ĵ�������PB0����ʼ��PB0Ϊ�������
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
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
  NVIC_SetPriority(EXTI4_15_IRQn, 0);                                // �����ж����ȼ�
  NVIC_EnableIRQ(EXTI4_15_IRQn);                                     // ʹ���ж�
}
// �ⲿ�жϷ�����
void EXTI4_15_IRQHandler(void)
{
  if (LL_EXTI_IsActiveFlag(LL_EXTI_LINE_4)) // �ж��ж���Դ
  {
    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_0); // ��תLED
    LL_EXTI_ClearFlag(LL_EXTI_LINE_4);       // ����жϱ�־
  }
}
// ����Ϊ��������
void APP_ErrorHandler(void)
{
  // ����
  while (1)
  {
  }
}
