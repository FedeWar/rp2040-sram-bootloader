/*
 * main.cpp
 *
 *  Created on: 12 set 2026
 *      Author: feder
 */
// Project
// C++
// PICO
#include <hardware/structs/clocks.h>
#include <hardware/structs/pwm.h>
#include <hardware/structs/iobank0.h>
#include <hardware/structs/resets.h>
#include <hardware/structs/sio.h>

void setup_clk()
{
	// clk_sys src = aux, auxsrc = rosc
	clocks_hw->clk[clk_sys].ctrl =
			(CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_ROSC_CLKSRC << CLOCKS_CLK_SYS_CTRL_AUXSRC_LSB) |
			(CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX << CLOCKS_CLK_SYS_CTRL_SRC_LSB);

	// Wait for clock glitchless switch
	while(clocks_hw->clk[clk_sys].selected != 0b10);

	// clk_ref = rosc / 3
	// clocks_hw->clk[clk_ref].div = 0b11 << CLOCKS_CLK_REF_DIV_INT_LSB;
}

void setup_pwm()
{
	// Bring PWM out of reset state
	resets_hw->reset &= ~RESETS_RESET_PWM_BITS;
	while(!(resets_hw->reset_done & RESETS_RESET_DONE_PWM_BITS));

	// clk_sys -> pwm
	// 6.5MHz / (65535 * 127) = 0.78 Hz
	pwm_hw->slice[0].div = (1 << PWM_CH0_DIV_INT_LSB) & PWM_CH0_DIV_INT_BITS;
	pwm_hw->slice[0].top = (1 << 16) - 1;
	pwm_hw->slice[0].cc = (1 << 15) & PWM_CH0_CC_A_BITS;
	pwm_hw->slice[0].csr = (PWM_CH0_CSR_DIVMODE_VALUE_DIV << PWM_CH0_CSR_DIVMODE_LSB) | (1 << PWM_CH0_CSR_EN_LSB);
}

void setup_gpio()
{
	// Bring IO_BANK0 out of reset state
	resets_hw->reset &= ~RESETS_RESET_IO_BANK0_BITS;
	while(!(resets_hw->reset_done & RESETS_RESET_DONE_IO_BANK0_BITS));

	// pwm_a0 -> gpio0
	iobank0_hw->io[0].ctrl = IO_BANK0_GPIO0_CTRL_FUNCSEL_VALUE_PWM_A_0 << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;

	// Set GPIO 25 function to SIO
	iobank0_hw->io[25].ctrl = IO_BANK0_GPIO25_CTRL_FUNCSEL_VALUE_SIO_25 << IO_BANK0_GPIO25_CTRL_FUNCSEL_LSB;

	// Set output enable for GPIO 25 in SIO
	sio_hw->gpio_oe_set |= 1 << 25;
}

extern "C" __attribute__((section(".main"), noreturn)) void _main()
{
	setup_clk();
	setup_gpio();
	setup_pwm();

	while(true)
	{
		// Wait for some time
		for (uint32_t i = 0; i < 100000; ++i);

		// Flip output for GPIO 25
		sio_hw->gpio_togl |= 1 << 25;
	}
}
