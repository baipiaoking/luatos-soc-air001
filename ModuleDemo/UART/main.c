#include "air001xx_ll_gpio.h"
#include "air001xx_ll_usart.h"
#include "main.h"
static void RCC_Configuration(void);               // ����ʱ��
static void UART_Configuration(uint32_t bandrate); // ����UART
LL_UTILS_ClkInitTypeDef UTILS_ClkInitStruct;       // ϵͳʱ�ӽṹ��
static USART_TypeDef *USARTx = USART1;

int main(void)
{
  // ����ʱ��
  RCC_Configuration();
  // ����SysTick
  LL_Init1msTick(SystemCoreClock);
  // ����UART
  UART_Configuration(115200);

  while (1)
  {
    APP_UartSendString("uart test!\r\n");
    LL_mDelay(1000);
  }
}
// ����ʱ��
static void RCC_Configuration(void)
{
  LL_RCC_HSI_Enable();                                  // ʹ��HSI
  LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_24MHz); // ����HSIΪ24MHz
  while (LL_RCC_HSI_IsReady() != 1);                     // �ȴ�HSI����

  LL_PLL_ConfigSystemClock_HSI(&UTILS_ClkInitStruct); // ����HSIΪϵͳʱ��

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1); // ����AHB��Ƶ

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);                   // ����ϵͳʱ��ԴΪPLL
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL); // �ȴ�ϵͳʱ��Դ�л�ΪPLL

  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1); // ����APB1��Ƶ

  SystemCoreClockUpdate(); // ����ϵͳʱ�ӣ����º��ϵͳʱ�Ӵ����SystemCoreClock��
}

// ��ʼ��GPIO
static void UART_Configuration(uint32_t bandrate)
{
  // ʹ��GPIOAʱ��
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  // ʹ��USART1ʱ��
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);

  // GPIOA����
  LL_GPIO_InitTypeDef GPIO_InitStruct;
  // ѡ��2������
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  // ѡ����ģʽ
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  // ѡ������ٶ�
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  // ѡ�����ģʽ
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  // ѡ������
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  // ����ΪUSART1����
  GPIO_InitStruct.Alternate = LL_GPIO_AF1_USART1;
  // GPIOA��ʼ��
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // PA3
  GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
  // ����ΪUSART1����
  GPIO_InitStruct.Alternate = LL_GPIO_AF1_USART1;
  // GPIOA��ʼ��
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // ����USART1�ж����ȼ�
  NVIC_SetPriority(USART1_IRQn, 0);
  // ʹ��USART1�ж�
  NVIC_EnableIRQ(USART1_IRQn);

  // ����USART����
  LL_USART_InitTypeDef USART_InitStruct;
  // ���ò�����
  USART_InitStruct.BaudRate = bandrate;
  // �������ݳ���
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  // ֹͣλ
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  // ����У��λ
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  // USART��ʼ��
  LL_USART_Init(USARTx, &USART_InitStruct);

  // ����Ϊȫ˫���첽ģʽ
  LL_USART_ConfigAsyncMode(USARTx);

  // ʹ��UARTģ��
  LL_USART_Enable(USARTx);

  // ʹ�ܽ�����żУ������ж�
  LL_USART_EnableIT_PE(USARTx);
  // ʹ�ܽ��մ����ж�
  LL_USART_EnableIT_ERROR(USARTx);
  // ʹ�ܽ������ݼĴ����ǿ��ж�
  LL_USART_EnableIT_RXNE(USARTx);
}

// ����һ���ֽ�
void APP_UartSendByte(unsigned char d)
{
  LL_USART_TransmitData8(USARTx, d);
  while (!LL_USART_IsActiveFlag_TXE(USARTx))
    ;
}
// �����ַ���
void APP_UartSendString(char *s)
{
  while (*s)
    APP_UartSendByte(*s++);
}

// �����ж�
void APP_UsartIRQCallback()
{
  uint32_t errorflags = (LL_USART_IsActiveFlag_PE(USARTx) | LL_USART_IsActiveFlag_FE(USARTx) |
                         LL_USART_IsActiveFlag_ORE(USARTx) | LL_USART_IsActiveFlag_NE(USARTx));
  if (errorflags == RESET)
  {
    // �������ݼĴ�����Ϊ��
    if ((LL_USART_IsActiveFlag_RXNE(USARTx) != RESET) && (LL_USART_IsEnabledIT_RXNE(USARTx) != RESET))
    {
      // ���յ�����
      uint8_t RxBuff = LL_USART_ReceiveData8(USARTx);
      APP_UartSendByte(RxBuff); // ����ȥ
      return;
    }
  }

  // ���մ���
  if (errorflags != RESET)
  {
    APP_ErrorHandler();
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
