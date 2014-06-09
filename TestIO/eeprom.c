#ifndef eeprom2_h
#define eeprom2_h

#include"eeprom.h"

void wyswietl_gwiazdki(int ile)
{
	LCD_GoTo(0,1);
	int i;
	for (i= 0; i < ile; ++i)
	{
		LCD_WriteText((unsigned char *)"*");
	}
}
int porownaj_id(int *a, int *b)
{
	int i=0;
	int licznik=0;
	for(;i<4;i++)
	{
		if(a[i]==b[i])
		{
			licznik++;
		}
	}
	if(licznik == 4)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int adres(int numer_rekordu)
{
	return (numer_rekordu ) * 8;
}

int znajdz_id(int num)//num jest szukanym id
{
	// falsz=0, jesli >0 to jest to numer rekordu(0-15), blad odczytu eeprom =-1, jesli id nie istnieje=-2
	int znaleziono=-2;
	int i=0;
	uint16_t adres_id=0;/////??
	//
	char tab[5]={0,0,0,0,0};
	uint8_t dane=0xff;

	int j=0;
	for (; i < 16  ; i++)
	{
		adres_id=adres(i);
		for(;j<4;j++)
		{
			delay_1ms();
			delay_1ms();
			dane=(Read_24Cxx(adres_id,M2404));
			if(dane==0xff)//blad odczytu eeprom
			{
				return -1;
			}
			else
			{
				tab[j]=dane;
			}
			adres_id++;
		}
		j=0;
		tab[4]=0;
		int id_odczytany=atoi(tab);
		if(num==id_odczytany)
		{
			znaleziono=i;
			break;
		}
	}//for
	return znaleziono;
}

void wczytaj_haslo(int indeks)
{
	unsigned char Tekst[2];
	int i=0;
	char tab[5]={0,0,0,0,0};

	Tekst[0]=0;
	Tekst[1]=0;
	while (i<4)
	{
		Tekst[0]=KBD_ReadKey();
		if (Tekst[0] !=0 )
		{
			tab[i]=Tekst[0];
			i++;
			wyswietl_gwiazdki(i);
			Tekst[0]=0;
		}
		delay_10us();
	}
	tab[4]=0;

	i=0;
	int dobrze=0;
	char dane=0;
	uint16_t adres_hasla=(adres(indeks)+4);
	for(;i<4;i++)
	{
		dane=(Read_24Cxx(adres_hasla,M2404));
		if(tab[i]==dane)
		{
			dobrze++;
		}
		adres_hasla++;
	}

	LCD_WriteCommand(HD44780_CLEAR);
	LCD_GoTo(0,0);

	if(dobrze==4)
	{
		LCD_WriteText((unsigned char *)"Otwieram drzwi");
		Zamek1_On();
	}
	else
	{
		LCD_WriteText((unsigned char *)"Bledne haslo");
	}
 }

int wczytaj_id()
{
	unsigned char Tekst[2];
	int i=0;
	char tab[5]={0,0,0,0,0};

	Tekst[0]=0;
	Tekst[1]=0;
	while (i<4)
	{
		Tekst[0]=KBD_ReadKey();
		if (Tekst[0] !=0 )
		{
			tab[i]=Tekst[0];
			i++;
			wyswietl_gwiazdki(i);
			Tekst[0]=0;
		}
		delay_10us();
	}
	tab[4]=0;

	return (atoi((const char *)tab));
}
void init_I2C1(void){

		/* This is a concept that has to do with the libraries provided by ST
		* to make development easier the have made up something similar to
		* classes, called TypeDefs, which actually just define the common
		* parameters that every peripheral needs to work correctly
		*
		* They make our life easier because we don't have to mess around with
		* the low level stuff of setting bits in the correct registers
		*/
		GPIO_InitTypeDef GPIO_InitStruct; // this is for the GPIO pins used as I2C1SDA and I2C1SCL
		GPIO_InitTypeDef GPIO_Output;
		I2C_InitTypeDef I2C_InitStruct; // this is for the I2C1 initilization

		/* enable APB1 peripheral clock for I2C1 */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);//zmianaRCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

		/* enable the peripheral clock for the pins used by
		* I2C1, PB6 for I2C SCL and PB9 for I2C1_SDL
		*/
		/* GPIOB clock enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

		/* This sequence sets up the I2C1SDA and I2C1SCL pins
		* so they work correctly with the I2C1 peripheral
		*/
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8; // Pins 6 (I2C1_SCL) and 9 (I2C1_SDA) are used
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;		// this defines the IO speed and has nothing to do with the baudrate!
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			// this defines the output type as open drain mode (as opposed to push pull)
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
		GPIO_Init(GPIOA, &GPIO_InitStruct);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers


		//nowe
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

		/* This sequence sets up the I2C1SDA and I2C1SCL pins
		* so they work correctly with the I2C1 peripheral
		*/
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; // Pins 6 (I2C1_SCL) and 9 (I2C1_SDA) are used
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;		// this defines the IO speed and has nothing to do with the baudrate!
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			// this defines the output type as open drain mode (as opposed to push pull)
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
		GPIO_Init(GPIOC, &GPIO_InitStruct);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers

		//nowe
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
		GPIO_Output.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
		GPIO_Output.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_Output.GPIO_OType = GPIO_OType_PP;
		GPIO_Output.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Output.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &GPIO_Output);


		/* The I2C1_SCL and I2C1_SDA pins are now connected to their AF
		* so that the USART1 can take over control of the
		* pins
		*/
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C3); //
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C3);

		/* Configure I2C1 */
		/* I2C DeInit */
		I2C_DeInit(I2C3);

		/* Enable the I2C peripheral */
		I2C_Cmd(I2C3, ENABLE);

		/* Set the I2C structure parameters */
		I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStruct.I2C_OwnAddress1 = 0xEE;
		I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_InitStruct.I2C_ClockSpeed = 30000;

		/* Initialize the I2C peripheral w/ selected parameters */
		I2C_Init(I2C3, &I2C_InitStruct);
}

uint8_t Write_24Cxx(uint16_t Addr, uint8_t Data, uint8_t Mem_Type)
{

		uint32_t timeout = I2C_TIMEOUT_MAX;
		uint8_t upper_addr,lower_addr;
		lower_addr = (uint8_t)((0x00FF)&Addr);

		if(Mem_Type==M24512)
		{
			Addr = Addr>>8;
			upper_addr = (uint8_t)((0x00FF)&Addr);
		}
		/* Generate the Start Condition */
		I2C_GenerateSTART(I2C3, ENABLE);

		/* Test on I2C1 EV5, Start trnsmitted successfully and clear it */
		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT))
		{
			/* If the timeout delay is exeeded, exit with error code */
			if ((timeout--) == 0) return 0xFF;
		}

		/* Send Memory device slave Address for write */
		I2C_Send7bitAddress(I2C3, MEM_DEVICE_WRITE_ADDR, I2C_Direction_Transmitter);

		/* Test on I2C1 EV6 and clear it */
		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
			/* If the timeout delay is exeeded, exit with error code */
			if ((timeout--) == 0) return 0xFF;
		}

		if(Mem_Type==M24512)
		{
			/* Send I2C1 location address LSB */
			I2C_SendData(I2C3, upper_addr);
			/* Test on I2C1 EV8 and clear it */
			timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
			while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
			{
				/* If the timeout delay is exeeded, exit with error code */
				if ((timeout--) == 0) return 0xFF;
			}
		}
		/* Send I2C1 location address LSB */
		I2C_SendData(I2C3, lower_addr);

		/* Test on I2C1 EV8 and clear it */
		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			/* If the timeout delay is exeeded, exit with error code */
			if ((timeout--) == 0) return 0xFF;
		}

		/* Send Data */
		I2C_SendData(I2C3, Data);

		/* Test on I2C1 EV8 and clear it */
		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			/* If the timeout delay is exeeded, exit with error code */
			if ((timeout--) == 0) return 0xFF;
		}

		/* Send I2C1 STOP Condition */
		I2C_GenerateSTOP(I2C3, ENABLE);

		/* If operation is OK, return 0 */
		return 0;
}

uint8_t Read_24Cxx(uint16_t Addr, uint8_t Mem_Type)
{
		uint32_t timeout = I2C_TIMEOUT_MAX;
		uint8_t Data = 0;
		uint8_t upper_addr,lower_addr;

		lower_addr = (uint8_t)((0x00FF)&Addr);

		if(Mem_Type==M24512)
		{
			Addr = Addr>>8;
			upper_addr = (uint8_t)((0x00FF)&Addr);
	}
  /* Generate the Start Condition */
  I2C_GenerateSTART(I2C3, ENABLE);

  /* Test on I2C1 EV5 and clear it */
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT))
  {
    /* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
  }

  /* Send DCMI selcted device slave Address for write */
  I2C_Send7bitAddress(I2C3, MEM_DEVICE_WRITE_ADDR, I2C_Direction_Transmitter);

  /* Test on I2C1 EV6 and clear it */
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
  }

	if(Mem_Type==M24512)
	{
		/* Send I2C1 location address LSB */
		I2C_SendData(I2C3,upper_addr);

		/* Test on I2C1 EV8 and clear it */
		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			/* If the timeout delay is exeeded, exit with error code */
			if ((timeout--) == 0) return 0xFF;
		}
	}

	  /* Send I2C1 location address LSB */
  I2C_SendData(I2C3, lower_addr);

  /* Test on I2C1 EV8 and clear it */
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
  }

  /* Clear AF flag if arised */
  //I2C1->SR1 |= (uint16_t)0x0400;

  /* Generate the Start Condition */
  I2C_GenerateSTART(I2C3, ENABLE);

  /* Test on I2C1 EV6 and clear it */
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT))
  {
    /* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
  }

  /* Send DCMI selcted device slave Address for write */
  I2C_Send7bitAddress(I2C3, MEM_DEVICE_READ_ADDR, I2C_Direction_Receiver);

  /* Test on I2C1 EV6 and clear it */
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
  }

	/* Prepare an NACK for the next data received */
  I2C_AcknowledgeConfig(I2C3, DISABLE);

  /* Test on I2C1 EV7 and clear it */
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {
    /* If the timeout delay is exeeded, exit with error code */
		//while(1);
    if ((timeout--) == 0) return 0xFF;
  }



  /* Prepare Stop after receiving data */
  I2C_GenerateSTOP(I2C3, ENABLE);

  /* Receive the Data */
  Data = I2C_ReceiveData(I2C3);

  /* return the read data */
  return Data;
}
#endif
