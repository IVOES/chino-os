//
// Kernel Diagnostic
//
#include <kernel/utils.hpp>
#include <libbsp/bsp.hpp>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_i2c.h>

using namespace Chino;

void RCC_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure USARTx_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USARTx_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void USART_Configuration(u32 BaudRate)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART1, &USART_InitStructure);

	USART_Cmd(USART1, ENABLE);

}

void I2C1_Init()
{
	I2C_InitTypeDef I2C_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x10;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
}
u8 I2C_Read(u8 nAddr)
{
	I2C_AcknowledgeConfig(I2C1, ENABLE); //ʹ��Ӧ��
	I2C_GenerateSTART(I2C1, ENABLE); //����һ����ʼλ
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) { ; } //�ȴ�EV5
	I2C_Send7bitAddress(I2C1, 0xA0, I2C_Direction_Transmitter); //����һ��αдָ��
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) { ; }//�ȴ�EV6
	I2C_SendData(I2C1, nAddr);//���Ͷ���ַ
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; } //�ȴ�EV8

	I2C_GenerateSTART(I2C1, ENABLE); //����һ����ʼλ
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) { ; } //�ȴ�EV5
	I2C_Send7bitAddress(I2C1, 0xA0, I2C_Direction_Receiver); //����һ����ָ��
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) { ; } //�ȴ�EV6
	I2C_AcknowledgeConfig(I2C1, DISABLE); //Ӧ��ʹ�ܹر�
	I2C_GenerateSTOP(I2C1, ENABLE); //����һ��ֹͣλ
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) { ; } //�ȴ�EV7
	return I2C_ReceiveData(I2C1); //���ض���������
}
static int8_t I2C_Write(uint8_t writeAddr, uint8_t value)
{
	/* ���I2C�����Ƿ�æµ */
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	/* ������ʼ�ź� */
	I2C_GenerateSTART(I2C1, ENABLE);

	/* ���EV5�����Ƿ�����Ϊ����ģʽ */
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/* ����д������ַ */
	I2C_Send7bitAddress(I2C1, 0xA0, I2C_Direction_Transmitter);//����ģʽ

	/* ���EV6�������*/
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/* ����д��ַ */
	I2C_SendData(I2C1, writeAddr);

	/* ���EV8 */
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	/* �������� */
	I2C_SendData(I2C1, value);
	/* ���EV8 */
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTOP(I2C1, ENABLE);

	return 0;
}

int uart_putc(int ch)
{
	USART1->SR;
	USART_SendData(USART1, (unsigned char)ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);

	return(ch);
}

void Chino::Diagnostic::BSPInitializeDebug(const BootParameters& bootParams)
{
	RCC_Configuration();
	GPIO_Configuration();
	USART_Configuration(115200);

	//I2C1_Init();
	//BSPDebugPutChar('1'); I2C_Write(0x10, 1);
	//Threading::BSPSleepMs(10);
	//BSPDebugPutChar('2'); I2C_Write(0x10, 1);
	//Threading::BSPSleepMs(10);
	//BSPDebugPutChar('3'); I2C_Read(0x10);
	//BSPDebugPutChar('4');
}

void Chino::Diagnostic::BSPDebugPutChar(wchar_t chr)
{
	uart_putc((uint8_t)chr);
}

void Chino::Diagnostic::BSPDebugBlueScreen()
{
}

void Chino::Diagnostic::BSPDebugClearScreen()
{
}
