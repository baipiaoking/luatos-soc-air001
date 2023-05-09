#include "main.h"
static void RCC_Configuration(void);         // ����ʱ��
static void GPIO_Configuration(void);        // ����GPIO
static void UART_Configuration(void);        // ����UART
static void CRC_Configuration(void);         // ����CRC
LL_UTILS_ClkInitTypeDef UTILS_ClkInitStruct; // ϵͳʱ�ӽṹ��
// ��ʼ��һ��uint32_t����100��С�����飬���ڴ��CRCУ������ݣ������е����ݿ����������ã�����ֻ�Ǿ���
uint32_t uwCRCData[100] = {
    0x00000000, 0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000008, 0x00000009,
    0x0000000A, 0x0000000B, 0x0000000C, 0x0000000D, 0x0000000E, 0x0000000F, 0x00000010, 0x00000011, 0x00000012, 0x00000013,
    0x00000014, 0x00000015, 0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B, 0x0000001C, 0x0000001D,
    0x0000001E, 0x0000001F, 0x00000020, 0x00000021, 0x00000022, 0x00000023, 0x00000024, 0x00000025, 0x00000026, 0x00000027,
    0x00000028, 0x00000029, 0x0000002A, 0x0000002B, 0x0000002C, 0x0000002D, 0x0000002E, 0x0000002F, 0x00000030, 0x00000031,
    0x00000032, 0x00000033, 0x00000034, 0x00000035, 0x00000036, 0x00000037, 0x00000038, 0x00000039, 0x0000003A, 0x0000003B,
    0x0000003C, 0x0000003D, 0x0000003E, 0x0000003F, 0x00000040, 0x00000041, 0x00000042, 0x00000043, 0x00000044, 0x00000045,
    0x00000046, 0x00000047, 0x00000048, 0x00000049, 0x0000004A, 0x0000004B, 0x0000004C, 0x0000004D, 0x0000004E, 0x0000004F,
    0x00000050, 0x00000051, 0x00000052, 0x00000053, 0x00000054, 0x00000055, 0x00000056, 0x00000057, 0x00000058, 0x00000059,
    0x0000005A, 0x0000005B, 0x0000005C, 0x0000005D, 0x0000005E, 0x0000005F, 0x00000060, 0x00000061, 0x00000062, 0x00000063,
};
// ��ʼ��һ��32λ�ı��������ڴ��CRCУ��Ľ��
uint32_t uwCRCValue = 0;
int main(void)
{
  // ����ʱ��
  RCC_Configuration();
  // ����SysTick
  LL_Init1msTick(SystemCoreClock);
  UART_Configuration();
  // ����GPIO
  GPIO_Configuration();
  // ����CRC
  CRC_Configuration();
  printf("CRC Check Demo\r\n");
  // ����CRCУ��ֵ
  for (int i = 0; i < 100; i++)
  {
    LL_CRC_FeedData32(CRC, uwCRCData[i]);
  }
  uwCRCValue = LL_CRC_ReadData32(CRC);
  printf("CRC Value:0x%08X\r\n", uwCRCValue);

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
// ����CRC
static void CRC_Configuration(void)
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CRC); // ʹ��CRCʱ��
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
void UART_Configuration(void)
{
  // ʹ��USART2ʱ��
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
  // ����USART2
  LL_USART_SetBaudRate(USART2, SystemCoreClock, LL_USART_OVERSAMPLING_16, 115200); // ���ò�����Ϊ115200
  LL_USART_SetDataWidth(USART2, LL_USART_DATAWIDTH_8B);                            // ��������λΪ8λ
  LL_USART_SetStopBitsLength(USART2, LL_USART_STOPBITS_1);                         // ����ֹͣλΪ1λ
  LL_USART_SetParity(USART2, LL_USART_PARITY_NONE);                                // ����У��λΪ��У��
  LL_USART_SetHWFlowCtrl(USART2, LL_USART_HWCONTROL_NONE);                         // ����Ӳ��������Ϊ��
  LL_USART_SetTransferDirection(USART2, LL_USART_DIRECTION_TX_RX);                 // ���ô��䷽��Ϊ�շ�
  LL_USART_Enable(USART2);                                                         // ʹ��USART2
  LL_USART_ClearFlag_TC(USART2);                                                   // ���������ɱ�־

  // ����USART2��GPIO
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA); // ʹ��GPIOAʱ��

  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);        // ����PA2Ϊ���ù���
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_VERY_HIGH); // ����PA2Ϊ����
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_UP);               // ����PA2Ϊ����
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_4);                // ����PA2Ϊ���ù���4

  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);        // ����PA3Ϊ���ù���
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_VERY_HIGH); // ����PA3Ϊ����
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_UP);               // ����PA3Ϊ����
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_4);                // ����PA3Ϊ���ù���4
}
int fgetc(FILE *f)
{
  int ch;
  while (!LL_USART_IsActiveFlag_RXNE(USART2))
    ;
  ch = LL_USART_ReceiveData8(USART2);
  return (ch);
}
int fputc(int ch, FILE *f)
{
  /* Send a byte to USART */
  LL_USART_TransmitData8(USART2, ch);
  while (!LL_USART_IsActiveFlag_TC(USART2))
    ;
  LL_USART_ClearFlag_TC(USART2);

  return (ch);
}
// ����Ϊ��������
void APP_ErrorHandler(void)
{
  // ����
  while (1)
  {
  }
}
