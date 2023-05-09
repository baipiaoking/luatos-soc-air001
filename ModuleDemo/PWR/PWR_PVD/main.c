#include "main.h"
static void RCC_Configuration(void);  // ����ʱ��
static void GPIO_Configuration(void); // ����GPIO
static void PVD_Configuration(void);  // ����PVD
int main(void)
{
  /* ����ϵͳʱ�� */
  RCC_Configuration();
  // GPIO��ʼ��
  GPIO_Configuration();
  // PVD��ʼ��
  PVD_Configuration();
  while (1)
  {
  }
}

// ��ʼ��GPIO
static void GPIO_Configuration(void)
{
  // ʹ��GPIOBʱ��
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  // ���������м�ĵ�������PB0����ʼ��PB0Ϊ�������
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
}
// ��ʼ��PVD
static void PVD_Configuration(void)
{
  LL_EXTI_InitTypeDef EXTI_InitStruct;
  EXTI_InitStruct.Line = LL_EXTI_LINE_16;            // �ⲿ�ж���
  EXTI_InitStruct.LineCommand = ENABLE;              // ʹ���ⲿ�ж���
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;            // �ж�ģʽ
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING; // �½��ش���
  LL_EXTI_Init(&EXTI_InitStruct);                    // ��ʼ���ⲿ�ж���
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR); // ʹ��PWRʱ��
  LL_PWR_EnablePVD();                                // ʹ��PVD
  LL_PWR_SetPVDLevel(LL_PWR_PVDLEVEL_2V8);           // ����PVD��ѹΪ2.8V
  LL_PWR_EnablePVDFilter();                          // ʹ��PVD�˲�
  LL_PWR_SetPVDFilter(LL_PWR_PVD_FILTER_1024CLOCK);  // ����PVD�˲�ʱ��Ϊ1024��ʱ��,��Լ17ms,�˲�ʱ��Խ��,������Խ��,��������Խ��
  NVIC_SetPriority(PVD_IRQn, 1);                     // �����ж����ȼ�
  NVIC_EnableIRQ(PVD_IRQn);                          // ʹ���ж�
}
void PVD_IRQHandler(void)
{
  if (LL_PWR_IsActiveFlag_PVDO()) // �ж�PVD�����ƽ,���Ϊ�͵�ƽ,�����PB0,����Ϩ��PB0
  {
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0); // ����PB0
  }
  else
  {
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0); // Ϩ��PB0
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
