#include "main.h"
static void RCC_Configuration(void);         // ����ʱ��
static void GPIO_Configuration(void);        // ����GPIO
LL_UTILS_ClkInitTypeDef UTILS_ClkInitStruct; // ϵͳʱ�ӽṹ��

int main(void)
{
  // ����ʱ��
  RCC_Configuration();
  // ����SysTick
  LL_Init1msTick(SystemCoreClock);
  // ����GPIO
  GPIO_Configuration();

  while (1)
  {
    // ��ˮ��
    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_1);
    LL_mDelay(200);
    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_0);
    LL_mDelay(200);
    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_3);
    LL_mDelay(200);
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

  // �������ϵ�������ΪPB1��PB0��PB3
  // �ֱ��ʼ��Ϊ�������
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_OUTPUT);

  // ���������ַ�ʽ��ʼ��
  //  LL_GPIO_InitTypeDef GPIO_InitStruct;                                 // GPIO�ṹ��
  //  GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_3; // ����PB0��PB1��PB3
  //  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;                          // �������
  //  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;                // ����
  //  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;                // ����
  //  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;                              // ����������
  //  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);                               // ��ʼ��GPIOB
}
// ����Ϊ��������
void APP_ErrorHandler(void)
{
  // ����
  while (1)
  {
  }
}
