#define T9ON TIM9 -> CR1 |= 1;
#define T9OFF TIM9 -> CR1 &= 0xFFFFFFFE;
#define T10ON TIM10 -> CR1 |= 1;
#define T10OFF TIM10 -> CR1 &= 0xFFFFFFFE;

void Freq_Init();
uint32_t Get_freq();

uint8_t data_ready = 0;
uint16_t overflows = 0;

void TIM1_BRK_TIM9_IRQHandler(){
	//clear the flag
	TIM9 -> SR = 0;
	overflows ++;
	return;
}

void TIM1_UP_TIM10_IRQHandler(){
	T9OFF
	T10OFF
	data_ready = 1;
	//Eliminar la bandera
	TIM10 -> SR = 0;
	return;
}

uint32_t Get_freq(){
	uint32_t events = 0;
	//Start the timers
	T9ON
	T10ON
	//Wait for get the count
	while(data_ready != 1);
	//Get the value
	events = overflows * 65536;
	events += TIM9 -> CNT;
	//Clear the timers and overflows count
	TIM10 -> CNT = 0;
	TIM9 -> CNT = 0;
	overflows = 0;
	data_ready = 0;
	return events;
}

void Freq_Init(){
	data_ready = 0;
	//Configurar el pin para source del tmr9
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	GPIOE -> MODER |= GPIO_MODER_MODER6_1;
	GPIOE -> OSPEEDR |= GPIO_OSPEEDR_OSPEED6_1;
	GPIOE -> PUPDR |= GPIO_PUPDR_PUPD6_1;
	GPIOE -> AFR[0] |= 0x03000000;
	//Configuracion del timer 9
	RCC -> APB2ENR |= RCC_APB2ENR_TIM9EN;
	TIM9 -> CR1 |= TIM_CR1_URS;
	TIM9 -> SMCR |= 0x67;
	TIM9 -> DIER |= TIM_DIER_UIE;
	TIM9 -> EGR = 0;
	TIM9 -> CCMR1 |= 0x0100;
	NVIC_SetPriority(TIM1_BRK_TIM9_IRQn,2);
	NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
	//Configuracion del timer 10
	RCC -> APB2ENR |= RCC_APB2ENR_TIM10EN;
	TIM10 ->CR1 |= TIM_CR1_URS;
	TIM10 ->DIER |= TIM_DIER_UIE;
	TIM10 -> PSC = ((uint16_t) (XTAL_FREQ / 65536));
	TIM10 -> DIER |= TIM_DIER_UIE;
	NVIC_SetPriority(TIM1_UP_TIM10_IRQn,1);
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	return;
}
