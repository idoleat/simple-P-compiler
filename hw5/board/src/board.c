#include "lcd/lcd.h"
#include <string.h>

#define PASS_ALL 511

int pass = 0;
char test_name[9][40] =
{   
    "local variable",
    "global variable",
    "global constant",
    "expression",
    "function declaration and invocation",
    "expression and function invocation",
    "conditional statement",
    "for statement",
    "while statement"
};

void init_uart0(void)
{	
	/* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

	/* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);

    usart_interrupt_enable(USART0, USART_INT_RBNE);
}

void init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1|GPIO_PIN_2);

    init_uart0();

    Lcd_Init();			// init OLED
    LCD_Clear(WHITE);
    BACK_COLOR=WHITE;
}

void showResult()
{
    LCD_Clear(WHITE);

    if (pass == PASS_ALL)
    {
        LCD_ShowString(10,  0, (u8 *)("Compiler hw5 pass!"), RED);
        delay_1ms(200);

        LCD_ShowString(10, 32, (u8 *)("Congradulation!"), GBLUE);
        delay_1ms(200);

        LCD_ShowString(10, 64, (u8 *)("You are the best!"), BLUE);
        delay_1ms(200);

    }
    else 
    {
        while (1)
        {
            int pass_tmp = pass;
            for (int i = 0; i < 9; ++i)
            {
                LCD_Clear(WHITE);

                if (!(pass_tmp & 1))
                {
                    LCD_ShowString(10, 10, (u8 *)test_name[i], RED);
                    LCD_ShowString(10, 60, (u8 *)("test fail!"), RED);
                    
                    delay_1ms(800);
                }
                pass_tmp = pass_tmp >> 1;
            }
        }  
    }
}

void test(int answer, int pass_in, char* name)
{
    LCD_Clear(WHITE);

    if(answer == pass_in)
    {
        LCD_ShowString(10,  10, (u8 *)name, BLUE);
        LCD_ShowString(10,  60, (u8 *)("test successful!"), BLUE);

        pass = (pass << 1) | 1;
    }
    else 
    {
        LCD_ShowString(10,  10, (u8 *)name, RED);
        LCD_ShowString(10,  60, (u8 *)("test fail!"), RED);
        
        pass = pass << 1;    
    }    

    delay_1ms(2000);
}

void testLocal(int pass_in)
{
    test(1, pass_in, test_name[0]);
}

void testGlobal(int pass_in)
{
    test(2, pass_in, test_name[1]);
}

void testGlobalConst(int pass_in)
{
    test(3, pass_in, test_name[2]);
}

void testExpression(int pass_in)
{
    test(4, pass_in, test_name[3]);
}

void testFunction(int pass_in)
{
    test(5, pass_in, test_name[4]);
}

void testExprFunc(int pass_in)
{
    test(6, pass_in, test_name[5]);
}

void testCondition(int pass_in)
{
    test(7, pass_in, test_name[6]);
}

void testFor(int pass_in)
{
    test(8, pass_in, test_name[7]);
}

void testWhile(int pass_in)
{
    test(9, pass_in, test_name[8]);
}
