/*
*   - Xdimax LTD Confidential and Proprietary -
*
*  Copyright (C) June 2000, Dimax.  ALL RIGHTS RESERVED.
*
*  Except as specifically permitted herein, no portion of the information,
*  including but not limited to object code and source code, may be reproduced,
*  modified, distributed, republished or otherwise exploited in any form or by
*  any means for any purpose without the prior written permission of Xdimax LTD
*  Use of software subject to the terms and conditions detailed in 
*  the file "LICENSE.txt".
*
*/

/*-------------------------------------------------------------------
* line.h
*-------------------------------------------------------------------
* Kholodenko Alex
* $Id: line.h,v 1.135 2016/08/16 09:57:58 xol Exp $
*-------------------------------------------------------------------
*/
#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#undef EXTERN
#ifdef LINE_MODULE
#define EXTERN
#else
#define EXTERN extern
#endif

#define CA_TOOLS
//#ifdef CA_TOOLS
#define LINE_MODULE
//#include <QMetaType>
//#endif

/*
*-----------------------------------------------------------------------------
* Constants
*-----------------------------------------------------------------------------
*/

/* Line Number */
enum
{
	LINE_L1,
	LINE_L2,
	LINE_L3,
#ifdef CA_TOOLS
#define CM5
#define CLNET
#endif
#if defined CM5 || defined CLNET
	LINE_L4,
	LINE_L5,
#if defined CM5
	LINE_L6,
	LINE_L7,
	LINE_L8,
#endif
#endif
	LINE_LAST
};

/* Line Function */
enum
{
	LINE_FUNC_UNUSED,
	LINE_FUNC_BIST_INT_LOOP,
	LINE_FUNC_DAIKIN_D3NET,
    LINE_FUNC_SANYO,
	LINE_FUNC_MELCO_MNET,
	LINE_FUNC_LG,
	LINE_FUNC_KNX,
	LINE_FUNC_TADIRAN_INVERTER,
	LINE_FUNC_SAMSUNG,
    LINE_FUNC_TOSHIBA,
    LINE_FUNC_HEAVY,
    LINE_FUNC_GREE,
    LINE_FUNC_COOL_GATE_4,
	LINE_FUNC_COOL_GATE_5,
	LINE_FUNC_HITACHI,
	LINE_FUNC_M1M2,
    LINE_FUNC_FUJITSU_SPLIT,
	LINE_FUNC_CLNET_MB,
	LINE_FUNC_TADIRAN_CLNET,
	LINE_FUNC_MEITAV,
    LINE_FUNC_SIMULATION,
    LINE_FUNC_FUJITSU,
    LINE_FUNC_P1P2,
    LINE_FUNC_MIDEA,
    LINE_FUNC_H1H2,
	LINE_FUNC_TRANE,
	LINE_FUNC_KENTATSU,
    LINE_FUNC_ALTHERMA,
	LINE_FUNC_AB,
    LINE_FUNC_PLUGBUS,
    LINE_FUNC_PLUGBUS_MASTER,
    LINE_FUNC_PANASONIC,
    LINE_FUNC_MAC,
    LINE_FUNC_GENERAL_FUJITSU,
    LINE_FUNC_KRP,
    LINE_FUNC_HDL,
    LINE_FUNC_G4,
    LINE_FUNC_CHIGO,
    LINE_FUNC_ELECTRA,
	LINE_FUNC_SIESTA,
    LINE_FUNC_HAIER,
	LINE_FUNC_LAST
};

/*
* AC Type
*/
enum LINE_AC_TYPE
{
	LINE_AC_TYPE_DK,
	LINE_AC_TYPE_SA,
	LINE_AC_TYPE_TO,
	LINE_AC_TYPE_ME,
	LINE_AC_TYPE_HT,
	LINE_AC_TYPE_FJ,
	LINE_AC_TYPE_MH,
	LINE_AC_TYPE_LG,
	LINE_AC_TYPE_GR,
	LINE_AC_TYPE_TI,	/* Tadiran Inverter */
	LINE_AC_TYPE_SM,	/* Samsung */
    LINE_AC_TYPE_FUS,   /* Fujitsu Split */
	LINE_AC_TYPE_M1M2,	/* Melco M1M2 */
	LINE_AC_TYPE_TAD,	/* Tadiran CLNET */
    LINE_AC_TYPE_SI,    /* Simulation */
    LINE_AC_TYPE_MT,    /* Meitav CTU4500 */
    LINE_AC_TYPE_EL,    /* Electra */
    LINE_AC_TYPE_P1P2,  /* Daikin P1P2 */
    LINE_AC_TYPE_MD,    /* Midea */
    LINE_AC_TYPE_H1H2,  /* Hitachi thermostat line */
	LINE_AC_TYPE_TR,	/* Trane */
	LINE_AC_TYPE_KT,	/* Kentatsu */
    LINE_AC_TYPE_AL,    /* Altherma */
	LINE_AC_TYPE_AB,	/* AB CLNET */
    LINE_AC_TYPE_PBM,	/* PLUGBUS master (various AC types) */
    LINE_AC_TYPE_PN,    /* Panasonic */
    LINE_AC_TYPE_MAC,   /* Mitsubishi MAC */
    LINE_AC_TYPE_GEN_FJ,/* General Fujitsu */
    LINE_AC_TYPE_KRP,   /* Daikin KRP */
    LINE_AC_TYPE_CG,    /* Chigo */
	LINE_AC_TYPE_SIESTA,/* Daikin Siesta */
    LINE_AC_TYPE_HA,    /* Haier */
	LINE_AC_TYPE_UNUSED,
};
#ifdef CA_TOOLS
#else
/* 
* Line HW 
*/
#define LINE_PIN_UNUSED					((uint8_t)(-1))
#define LINE_IS_GPIO(GPIOx)				(((uint32_t)(GPIOx)) & PERIPH_BASE )
struct line_HW_st
{
	USART_TypeDef*	USARTx;
	IRQn_Type		IRQn;
	uint32_t		IRQ_pri;

    GPIO_TypeDef*	tx_A_GPIO;
	uint8_t			tx_A_PIN;
    GPIO_TypeDef*	tx_B_GPIO;
	uint8_t			tx_B_PIN;

    GPIO_TypeDef*	rx_GPIO;
	uint8_t			rx_PIN;

    GPIO_TypeDef*	tx_EN_GPIO;
	uint8_t			tx_EN_PIN;
    GPIO_TypeDef*	rx_EN_GPIO;
	uint8_t			rx_EN_PIN;

    GPIO_TypeDef*	led_GPIO;
	uint8_t			led_PIN;
};
#endif
EXTERN const struct line_HW_st line_HW[LINE_LAST]
#ifdef CA_TOOLS
#else
#ifdef LINE_MODULE
={
	#ifdef CM5
	{ 	/* L1: MM1007 1 */
		USARTx		:UART5,		/* Shared L1/L5 */
		IRQn		:UART5_IRQn,
		IRQ_pri		:IRQ_PRI_UART5,
		tx_A_GPIO	:GPIOE,		tx_A_PIN	:5,
		tx_B_GPIO	:GPIOE, 	tx_B_PIN	:6,
		rx_GPIO		:GPIOD,		rx_PIN		:2,
		tx_EN_GPIO	:0,			tx_EN_PIN	:LINE_PIN_UNUSED,
		rx_EN_GPIO	:0,      	rx_EN_PIN	:6,		/* I2C */
		led_GPIO	:GPIOE,		led_PIN		:0
	},
	{ 	/* L2: MM1007 2 */
		USARTx		:USART2,	/* Shared L2/L6 */
		IRQn		:USART2_IRQn,
		IRQ_pri		:IRQ_PRI_USART2,
		tx_A_GPIO	:GPIOE,	tx_A_PIN	:2,
		tx_B_GPIO	:GPIOE, tx_B_PIN	:4,
		rx_GPIO		:GPIOD,	rx_PIN		:6,
		tx_EN_GPIO	:0,		tx_EN_PIN	:LINE_PIN_UNUSED,
		rx_EN_GPIO	:0,		rx_EN_PIN	:LINE_PIN_UNUSED,
	   	led_GPIO	:GPIOE,	led_PIN		:1
	},

	{ 	/* L3: MODBUS */
		USARTx		:USART3,
		IRQn		:USART3_IRQn,
		IRQ_pri		:IRQ_PRI_USART3,
		tx_A_GPIO	:GPIOD,	tx_A_PIN	:8,
		tx_B_GPIO	:0, 	tx_B_PIN	:LINE_PIN_UNUSED,
		rx_GPIO		:GPIOD,	rx_PIN		:9,
		tx_EN_GPIO	:GPIOB,	tx_EN_PIN	:2,
		rx_EN_GPIO	:GPIOC,	rx_EN_PIN	:13,
	   	led_GPIO	:GPIOB,	led_PIN		:9
	},

	{ 	/* L4: RS485_1 */
		USARTx		:USART1,
		IRQn		:USART1_IRQn,
		IRQ_pri		:IRQ_PRI_USART1,
		tx_A_GPIO	:GPIOB,	tx_A_PIN	:6,
		tx_B_GPIO	:0,     tx_B_PIN	:LINE_PIN_UNUSED,
		rx_GPIO		:GPIOB,	rx_PIN		:7,
		tx_EN_GPIO	:GPIOD,	tx_EN_PIN	:13,
		rx_EN_GPIO	:0	,	rx_EN_PIN	:LINE_PIN_UNUSED,
	   	led_GPIO	:GPIOC,	led_PIN		:8
	},

	{ 	/* L5: RS485_2 */
		USARTx		:UART5,		/* Shared L1/L5 */
		IRQn		:UART5_IRQn,
		IRQ_pri		:IRQ_PRI_UART5,
		tx_A_GPIO	:GPIOC,	tx_A_PIN	:12,
		tx_B_GPIO	:0, 	tx_B_PIN	:LINE_PIN_UNUSED,
		rx_GPIO		:GPIOD,	rx_PIN		:2,
		tx_EN_GPIO	:GPIOD,	tx_EN_PIN	:10,
		rx_EN_GPIO	:0,  	rx_EN_PIN	:6,	/* via I2C Extender */
		led_GPIO	:0,		led_PIN		:2  /* via I2C Extender */
	},

	{ 	/* L6: RS485_3 */
		USARTx		:USART2,	/* Shared L2/L6 */
		IRQn		:USART2_IRQn,
		IRQ_pri		:IRQ_PRI_USART2,
		tx_A_GPIO	:0,		tx_A_PIN	:LINE_PIN_UNUSED,
		tx_B_GPIO	:GPIOD, tx_B_PIN	:3,
		rx_GPIO		:GPIOD,	rx_PIN		:6,
		tx_EN_GPIO	:GPIOD,	tx_EN_PIN	:11,
		rx_EN_GPIO	:0,  	rx_EN_PIN	:5,	/* via I2C Extender */
		led_GPIO	:0,		led_PIN		:1  /* via I2C Extender */
	},

	{ 	/* L7: RS485_4 */
		USARTx		:USART6,
		IRQn		:USART6_IRQn,
		IRQ_pri		:IRQ_PRI_USART6,
		tx_A_GPIO	:GPIOC,	tx_A_PIN	:6,
		tx_B_GPIO	:0, 	tx_B_PIN	:LINE_PIN_UNUSED,
		rx_GPIO		:GPIOC,	rx_PIN		:7,
		tx_EN_GPIO	:GPIOD,	tx_EN_PIN	:12,
		rx_EN_GPIO	:0,  	rx_EN_PIN	:7,	/* via I2C Extender */
		led_GPIO	:0,		led_PIN		:0  /* via I2C Extender */
	},

	{ 	/* L8: USB */
		USARTx		:0,
		IRQn		:0,
		IRQ_pri		:0,
		tx_A_GPIO	:0,		tx_A_PIN	:LINE_PIN_UNUSED,
		tx_B_GPIO	:0, 	tx_B_PIN	:LINE_PIN_UNUSED,
		rx_GPIO		:0,		rx_PIN		:LINE_PIN_UNUSED,
		tx_EN_GPIO	:0,		tx_EN_PIN	:LINE_PIN_UNUSED,
		rx_EN_GPIO	:0,  	rx_EN_PIN	:LINE_PIN_UNUSED,
		led_GPIO	:0,		led_PIN		:LINE_PIN_UNUSED
	},
	#endif/*CM5*/
	#ifdef CLNET
	{	/* L1 via GPIO */
		USARTx		:0,
	},
	{	/* L2: RS485 Tadiran Inverter or Meitav CTU4500 */
		USARTx		:USART2,
		IRQn		:USART2_IRQn,
		IRQ_pri		:IRQ_PRI_USART2,
		tx_A_GPIO	:GPIOD,	tx_A_PIN	:5,
		tx_B_GPIO	:0,     tx_B_PIN	:LINE_PIN_UNUSED,
		rx_GPIO		:GPIOD,	rx_PIN		:6,
		tx_EN_GPIO	:GPIOD,	tx_EN_PIN	:10,
		rx_EN_GPIO	:GPIOD,	rx_EN_PIN	:11,
							led_PIN		:LINE_PIN_UNUSED,
	},
	{	/* L3: MODBUS */
		USARTx		:USART1,
		IRQn		:USART1_IRQn,
		IRQ_pri		:IRQ_PRI_USART1,
		tx_A_GPIO	:GPIOB,	tx_A_PIN	:6,
		tx_B_GPIO	:0, 	tx_B_PIN	:LINE_PIN_UNUSED,
		rx_GPIO		:GPIOB,	rx_PIN		:7,
		tx_EN_GPIO	:GPIOD,	tx_EN_PIN	:12,
		rx_EN_GPIO	:GPIOD,	rx_EN_PIN	:13,
		led_GPIO	:0,		led_PIN		:LINE_PIN_UNUSED,
	},
	{	/* L4: P1P2/AB/M1M2/Tadiran */
		USARTx		:UART5,
		IRQn		:UART5_IRQn,
		IRQ_pri		:IRQ_PRI_UART5,
		tx_A_GPIO	:GPIOC,	tx_A_PIN	:12,
		tx_B_GPIO	:0,     tx_B_PIN	:LINE_PIN_UNUSED,
		rx_GPIO		:GPIOD,	rx_PIN		:2,
		tx_EN_GPIO	:GPIOD,	tx_EN_PIN	:13,
		rx_EN_GPIO	:0,		rx_EN_PIN	:LINE_PIN_UNUSED,
							led_PIN		:LINE_PIN_UNUSED,
	},
    { 	/* L5: KNX */
		USARTx		:USART3,
		IRQn		:USART3_IRQn,
		IRQ_pri		:IRQ_PRI_USART3,
		tx_A_GPIO	:GPIOD,	tx_A_PIN	:8,
		tx_B_GPIO	:0, 	tx_B_PIN	:LINE_PIN_UNUSED,
		rx_GPIO		:GPIOD,	rx_PIN		:9,
		tx_EN_GPIO	:0,	    tx_EN_PIN	:LINE_PIN_UNUSED,
		rx_EN_GPIO	:0,	    rx_EN_PIN	:LINE_PIN_UNUSED,
	   	led_GPIO	:0,	    led_PIN		:LINE_PIN_UNUSED
	},
	#endif/*CLNET*/

#endif
    #ifdef COOLPLUG
    { 	/* L1: HVAC Line */
		USARTx		:USART2,
		IRQn		:USART2_IRQn,
		IRQ_pri		:IRQ_PRI_USART2,
		tx_A_GPIO	:GPIOA,		tx_A_PIN	:2,
		tx_B_GPIO	:0, 	    tx_B_PIN	:LINE_PIN_UNUSED,
		rx_GPIO		:GPIOA,		rx_PIN		:3,
		tx_EN_GPIO	:0,			tx_EN_PIN	:LINE_PIN_UNUSED,
		rx_EN_GPIO	:0,      	rx_EN_PIN	:LINE_PIN_UNUSED,
		led_GPIO	:0,		    led_PIN		:LINE_PIN_UNUSED
	},
	{ 	/* L2: ModBus */
		USARTx		:USART3,
       #if defined STM32F103
        IRQn		:USART3_IRQn,
       #elif defined STM32F0XX
        IRQn		:USART3_4_IRQn,
       #endif
        IRQ_pri		:IRQ_PRI_USART3,
		tx_A_GPIO	:GPIOB,	    tx_A_PIN	:10,
		tx_B_GPIO	:0,         tx_B_PIN	:LINE_PIN_UNUSED,
		rx_GPIO		:GPIOB,	    rx_PIN		:11,
		tx_EN_GPIO	:GPIOB,	    tx_EN_PIN	:15,
		rx_EN_GPIO	:GPIOB,		rx_EN_PIN	:14,
	   	led_GPIO	:0,	        led_PIN		:LINE_PIN_UNUSED
	},
    { 	/* L3: GEN_FJ only second line */
		USARTx		:0,
		IRQ_pri		:IRQ_PRI_USART2,    /* Same as L1 */
		tx_A_GPIO	:GPIOB,	    tx_A_PIN	:5,
		tx_B_GPIO	:0,         tx_B_PIN	:LINE_PIN_UNUSED,
		rx_GPIO		:GPIOB,	    rx_PIN		:9,
		tx_EN_GPIO	:0,			tx_EN_PIN	:LINE_PIN_UNUSED,
		rx_EN_GPIO	:0,      	rx_EN_PIN	:LINE_PIN_UNUSED,
	   	led_GPIO	:0,	        led_PIN		:LINE_PIN_UNUSED
	},
    #endif
}
#endif
;

/*
* Line Flags in VPD
*/
#define	__LDF_RS485				0x00000001
#define	__LDF_RS232				0x00000002
#define __LDF_SCAN				0x00000004
#define __LDF_DUAL_L7_AB		0x00000008
#define __LDF_MASTER        	0x00000010
#define __LDF_16v_OUT			0x00000020
#define __LDF_OLD_SLINK         0x00000040  /* Heavy: Old SuperLink */
#define __LDF_SM_DVM            0x00000040  /* Samsung old model (DVM) */
#define __LDF_AUTO_MA_SL        0x00000080  /* Auto master/slave detect */
#define __LDF_USART_9BIT		0x00000100
#define __LDF_USART_E_PARITY	0x00000200
#define __LDF_USART_O_PARITY	0x00000400
#define __LDF_USART_2_STOP		0x00000800
#define __LDF_USB_HOST			0x00001000
#define __LDF_SIMUL_NO_FLR      0x00002000  /* Simulate without failures */
#define __LDF_SIMUL_VAM         0x00004000  /* Simulate vams/heaters */
#define __LDF_SIMUL_IO          0x00008000  /* Simulate digital/analog I/O */
#define __LDF_SLAVE_AMB_INDOOR	0x00010000	/* CLNET: take amdient temp from indoor */
#define __LDF_NO_UART			0x00020000
#define __LDF_HEAT_AS_HAUX      0x00040000  /* Heat treated as Haux */
#define __LDF_PRO               0x00080000  /* PRO line */
/* General */
#define __LDF_TTM               0x40000000
#define __LDF_TTM_XTR           0x80000000  /* TTM through xtrace */

/*
* Line Flags
*/
#define	__LINE_ACK				0x00000001	/* Got ACK */
#define __LINE_I2C_ERROR		0x00000002
#define __LINE_DUAL_L7			0x00000004	/* L7 in Dual Polarity Mode */
#define __LINE_POLARITY_LOCK    0x00000008
#define __LINE_TTM_TX485		0x00000010	/* TTM on lines without RX task */
#define __LINE_SW_UART_TX		0x00000020
#define __LINE_SCAN_DONE        0x00000040
#define __LINE_TOKEN	        0x00000080
#define __LINE_LG_SLAVE_VENT	0x00000100
#define __LINE_FEED_SUPPORT     0x00000200  /* RT can be set */
#define __LINE_USE_NVDB         0x00000400
#define __LINE_FAILURE          0x00000800
#define __LINE_EMULATE_ACK		0x00001000	/* Emulate ACK after message TX */
/* CLNET specific flags */
#define __LINE_SET_SETTING      0x00100000
#define __LINE_GET_SETTING      0x00200000
#define __LINE_CONNECTED        0x00400000  /* Connected to Indoor */
#define __LINE_CONNECTING       0x00800000
#define __LINE_SLAVE_PRESENT    0x01000000
#define __LINE_MAC_SPECIAL      0x02000000
#define __LINE_TTY              0x04000000

/*
* Line CIF Commands
*/
/* Commands that require uid */
#define LINE_CIF_ON				0x0001
#define LINE_CIF_OFF			0x0002
#define LINE_CIF_TEMP			0x0003
#define LINE_CIF_DT				0x0004	/* +|-<set_temp> */
#define LINE_CIF_FSPEED			0x0005
#define LINE_CIF_COOL			0x0006
#define LINE_CIF_HEAT			0x0007
#define LINE_CIF_FAN 			0x0008
#define LINE_CIF_DRY 			0x0009
#define LINE_CIF_AUTO			0x000A
#define LINE_CIF_FILTER			0x000B
#define LINE_CIF_SWING			0x000C
#define LINE_CIF_REPREQ     	0x000D
#define LINE_CIF_VAM_MODE		0x000E
#define LINE_CIF_VAM_FSPEED		0x000F
#define	LINE_CIF_LOCK			0x0010
#define LINE_CIF_DIG            0x0011
#define LINE_CIF_WH_MODE		0x0012	/* Water Heater mode */
#define LINE_CIF_FEED           0x0013
#define LINE_CIF_FEED_DT        0x0014
#define LINE_CIF_MAIN_RC        0x0015  /* Daikin main RC */
#define LINE_CIF_TANK           0x0016  /* Daikin altherma tank control */
#define LINE_CIF_BOOSTER        0x0017  /* Daikin altherma tank booster */
#define LINE_CIF_HAUX			0x0018
#define LINE_CIF_CHANGE_NOTIF   0x0019  /* Indoor status changed */
#define LINE_CIF_VAM_ONOFF      0x001A
#define LINE_CIF_ASCII_IF_CMD   0x001B
#define LINE_CIF_MAC_AND_TYPE   0x001C
/* CLNET specific commands */
#define LINE_CIF_GET_SETTING	0x0080
#define LINE_CIF_SET_SETTING	0x0081
#define LINE_CIF_ECO            0x0082	/* Economic mode */
/* Commands that don't require uid */
#define LINE_CIF_NONE_UID		0x0100
#define LINE_CIF_SIGNAL			0x0101	/* Signal Queue to wake up task */
#define LINE_CIF_TTY			0x0102
#define LINE_CIF_TTM			0x0103
#define LINE_CIF_POWERUP        0x0104
#define LINE_CIF_DISTRIBUTE_CA  0x0105
#define LINE_CIF_READ_HREG      0x0106  /* ModBus read holding registers */
#define LINE_CIF_WRITE_HREG     0x0107  /* ModBus write holding registers */
#define LINE_CIF_ROTX			0x0108
#define LINE_CIF_WRITE_FL_REC   0x0109  /* ModBus write file record */
#define LINE_CIF_SUPPRESS_NOTIF 0x010A  /* Melco suppress nitifications */
/* KNX commands */
#define LINE_CIF_KNX_SET_ADDR	0x0200
#define LINE_CIF_KNX_CHANGE		0x0201

/*
*-----------------------------------------------------------------------------
* Types
*-----------------------------------------------------------------------------
*/

/* Line type descriptor */
typedef struct
{

#ifdef CA_TOOLS
    char            func_str[12];
#else
    char            func_str[9];
#endif
    char            func_char;
    uint8_t         ac_type;
    char            ac_type_str[7];
#ifdef CA_TOOLS
    uint32_t
#else
    uint16_t
#endif
                    lines;
#ifdef CA_TOOLS
    void            (*task)(void*);
#else
    TaskFunction_t  task;
#endif
    uint8_t         stack_sz;
}line_type_descr_t;
//#ifdef CA_TOOLS
// Q_DECLARE_METATYPE(line_type_descr_t)
//#endif

/* Error/Data Counter */
typedef union
{
	uint32_t		c32;
	struct
	{
		uint16_t	recent;		/* After last read */
		uint16_t	total;		/* Total */
	};
}line_cntr_t;

/* 
* Line Descriptor 
*/
struct pro_db_st;   /* Forward declaration */
typedef struct line_descr_st
{
    struct line_VPD_st*			pVPD;
	const struct line_HW_st*	pHW;

#ifdef CA_TOOLS
#else
    QueueHandle_t				CIF;
#endif
	volatile uint32_t			b2b;			/* Byte-2-Byte timeout */
	uint32_t					msg_2_msg_to;	/* MSG_2_MSG timeout */

	uint8_t			ldi;		/* Line Index */
	char			name[3];	/* Line Name */

    uint16_t        crnt_type;  /* Current line type */

	uint32_t		flags;

	uint8_t			G_cntr;		/* Indoor groups */
	uint8_t			U_cntr;		/* Indoor units */

	line_cntr_t		rx_cntr;
	line_cntr_t		tx_cntr;
	line_cntr_t		to_cntr;
	line_cntr_t		cs_cntr;	
	line_cntr_t		co_cntr;	/* Collision */
	line_cntr_t		nak_cntr;	/* NAK */

    uint32_t        polarity_ref_tick;  /* Tick counter on good rx msg */
    uint32_t        polarity_sw_to_sec;

	char* (*stat)( char* dst, int uid, int mode );
	int (*cfg_stat)( char* dst, struct line_descr_st* ld );
					#define __CFG_STAT_NO_MYID	    0x01
					#define __CFG_STAT_MS_SL	    0x02
                    #define __CFG_STAT_CONNECT      0x04
                    #define __CFG_STAT_AUTO_MS_SL   0x08
					#define __CFG_STAT_SCAN			0x10
#if defined CM5
    char* (*istat)( char* dst, struct pro_db_st* prec, char mode, int stage );
    char* (*ostat)( char* dst, struct pro_db_st* prec, char mode, int stage );
   #ifdef MODBUS_H_INCLUDED
    int (*cg4_pro)( uint8_t ldi, mbl_t* mbl, uint16_t addr, uint16_t val );
   #else
    void *cg4_pro;
   #endif
    uint16_t (*cool_if_pro_stat)( uint8_t* buf, struct pro_db_st* prec, 
                                  uint8_t stat );
#endif

#if defined CLNET || defined COOLPLUG
    struct
    {
        uint8_t     uid;
        uint8_t     num;
        uint8_t     val;
    }settings;
#endif

	/* Line Type Specific */
	union
	{
	/*
	* CM5
    */
	#ifdef CM5
		struct
		{
			uint8_t		seq_n;	/* Message sequencial number */
		}samsung;

		struct
		{
			uint8_t*	rx_buf;
			uint8_t		rx_sz;
		}heavy;

		struct
		{
			uint16_t	outdoors;
		}hitachi;

		/* Fujitsu */
		struct
		{
			uint8_t*	rx_buf;
			uint8_t*	tx_buf;
			uint8_t		rx_sz;
			uint8_t		tx_sz;
		}fujitsu;

        struct
        {
#define UNITS_DB_SZ     16  /* bytes */
            uint8_t*    units_db;
            uint8_t     rx_byte;
            uint8_t     flags;
                #define __DAIKIN_CA_SEQ_RESP    0x01
        }daikin;

		struct
		{
			uint8_t*	tmp_buf;
			uint8_t		flags;
				#define __SANYO_RX_SCAN_RESP	0x01
			uint8_t		last_out_addr;		/* Last outdoor scan reply */
		}sanyo;

        struct
        {
            uint8_t     flags;
                #define __MELCO_NO_REPREQ       0x01
        }melco;

        struct
        {
            uint8_t     power_up    :1;
            uint8_t                 :7;
        }haier;
	#endif/*CM5*/

	/*
	* CLNET
    */
    #if defined CLNET || defined COOLPLUG
		/* FUS */
        struct
        {
            uint16_t    flags           :4;
                #define __FUS_IND_REQ           0x01
                #define __FUS_INIT              0x02
                #define __FUS_INIT_DONE         0x04
                #define __FUS_DETECTD           0x08
            uint16_t    init_msg_cntr   :2;
            uint16_t    ms_sl_cntr      :4;
            uint16_t    good_cntr       :3;
            uint16_t                    :3;
        }fus;

		/* Melco M1M2 */
		struct
		{
			uint8_t		flags;
				#define M1M2_TTM           	0x01
				#define M1M2_TX_RTR        	0x02	/* TX retry required */
				#define M1M2_NEW_CONN1     	0x04    /* Got Connected to Indoor*/
				#define M1M2_NEW_CONN2     	0x08    /* Got Connected to Indoor*/

			uint8_t     rx_flags;
				#define __MRX_STATUS0   0x01    /* Got Status0 */
				#define __MRX_REPREQ1   0x02
				#define __MRX_REPREQ2   0x04
				#define __MRX_CMD       0x08
				#define __MRX_STATUS1   0x10    /* Got Status1 */
				#define __MRX_POWERON0  0x20
                #define __MRX_IN_PROG   0x40

		    uint8_t     change_flags;
			uint8_t     change_armed;
				#define __MCF_ONOF   	0x01
				#define __MCF_MODE      0x02
				#define __MCF_FAN       0x04
				#define __MCF_STEMP     0x08
			#define __MCF_MASK	(__MCF_MODE|__MCF_ONOF|__MCF_FAN|__MCF_STEMP)
				#define __MCF_FILTER    0x10	/* Filter Reset request */
				#define __MCF_FS_MODE   0x20	/* Function Setting Mode */

			uint8_t     fs_flags;
				#define __MFS_GET_DONE	0x01
				#define __MFS_SET_MODE  0x02
				#define __MFS_GET_MODE  0x04

			uint8_t		settings[6];
			uint16_t	last_RT_x10;
		}m1m2;
		
		/* Tadiran */
		struct
		{
            int			uid;						/* Single indoor UID */ 
            uint8_t		flags;
			    #define TAD_IGNORE_INDR	    0x01    /* Ignore message from Indoor */
			    #define TAD_RX_MSG			0x02 	/* RX in progress */
			    #define	TAD_RX_PREAMBLE		0x04
			    #define	TAD_RXD_24BIT		0x10    /* 24 bits RX Done */
			    #define	TAD_RXD_64BIT		0x20    /* 64 bits RX Done */
		}tad_clnt;

		/* Siesta */
		struct
		{
			int			uid;			/* Single indoor UID */ 
			uint8_t		flags;
				#define SIESTA_RX_DONE_AS_TS			0x01
				#define SIESTA_RX_PREAMBLE_AS_TS		0x02
				/* Indoor simulation for original thermostat connection */
				#define SIESTA_TX_AS_INDOOR				0x04	
				#define SIESTA_RX_DONE_AS_INDOOR		0x08
				#define SIESTA_RX_PREAMBLE_AS_INDOOR	0x10
				#define SIESTA_GOT_POWERUP_AS_INDOOR	0x20
				#define SIESTA_POWERUP_UPDATE			0x40
				#define SIESTA_TX_DONE					0x80

            uint8_t     exe     :3;     /* EXE counter */
            uint8_t     exe_as_i:3;     /* EXE counter as Indoor */
            uint8_t             :2;
		}siesta;

        /* P1P2 */
		struct
		{
            uint16_t    rx_flags;
                #define	__P1P2_ACK_03	    0x0001
		        #define	__P1P2_ACK_04	    0x0002
		        #define	__P1P2_REP_20	    0x0004
		        #define __P1P2_REP_16	    0x0008
		        #define	__P1P2_REP_21	    0x0010
		        #define __P1P2_REP_10	    0x0020
		        #define __P1P2_REP_11	    0x0040
		        #define __P1P2_RESP4		0x0080
		        #define __P1P2_ACK_00		0x0100
		        #define __P1P2_CS_ERR		0x0200
                #define __P1P2_REP_14       0x0400
                #define __P1P2_TYPE_80      0x0800
                /* From Slave */
		        #define __P1P2_SLAVE_30	    0x1000
		        #define __P1P2_SLAVE_38	    0x2000
		        #define __P1P2_SLAVE_39	    0x4000
                /* From Master */
		        #define __P1P2_MASTER_30	0x1000
		        #define __P1P2_MASTER_38	0x2000
		        #define __P1P2_MASTER_39	0x4000
		        #define __P1P2_MASTER_3X	0x8000

            uint8_t     main_RC     :2;
                #define P1P2_NOT_MAIN_RC	0x00
		        #define	P1P2_MAIN_RC		0x01
		        #define P1P2_NO_MAIN_RC	    0x03
            uint8_t                 :6;
            int         ff_addr;    /* Indoor address with forced fan */
		}p1p2;

        /* H1H2 */
        struct
        {
            uint8_t     flags;
                #define __H1H2_MSG_RECEIVED         0x01
                #define __H1H2_MASTER_DETECT        0x02
                #define __H1H2_SLAVE_DETECT         0x04
                #define __H1H2_MASTER_PRESENT       0x08
                #define __H1H2_MASTER_DETECT_DONE   0x10
                #define __H1H2_MSG_TRANSMITTED      0x20
                #define __H1H2_MASTER_REDETECT      0x40
                #define __H1H2_SETTINGS_COPIED      0x80
            uint8_t     ms_sl_cntr;
        }h1h2;

        /* AB */
        struct
        {
            uint8_t     flags;
                #define __AB_ACK                0x01
            uint8_t     indoor_master;
            uint8_t     sensor;
            uint8_t     ca_addr;
        }ab;

        /* General(Fujitsu) */
        struct
        {
            uint8_t         flags       :5;
                #define __GEN_FJ_RX_KEEP_ALIVE 	0x01
                #define __GEN_FJ_RX_STAT     	0x02
                #define __GEN_FJ_TX             0x04
                #define __GEN_FJ_DETECTED       0x08
            uint8_t         rtr_cntr    :2; /* Retry counter */
            uint8_t         req_rtr     :1; /* Retry received (while indoor simulating) */

            struct gen_fj_irq_args_st *args;
        }gen_fj;

        /* MAC */
        struct
        {
            uint8_t     to_cntr;
        }mac;

        /* KRP */
        struct
        {
            uint8_t     nak_cntr;
        }krp;

        /* PlugBus */
        struct
        {
            uint8_t     flags;
                #define __PB_ADDR_DISTR     0x01    /* Address distribution */
        }plugbus;
    #endif/*CLNET*/
	};
}line_descr_t;

/*
* Line CIF
*/
#ifdef SEMAPHORE_H
typedef struct
{
	int					cmd;
	int*				p_rc;			/* Return code pointer */
	int					uid;
	int					param;
	SemaphoreHandle_t	wait_semaphore;
}line_CIF_msg_t;
#endif


/*
*-----------------------------------------------------------------------------
* Global Variables
*-----------------------------------------------------------------------------
*/
EXTERN line_descr_t line_descr[LINE_LAST];

#ifdef LINE_MODULE
 /*
 * Line Tasks declaration
 */
 void bist_internal_port_loop_task( void* );
 void daikin_task( void* ); 
 void sanyo_task( void* );
 void melco_task( void* ); 
 void lg_task( void* ); 
 void knx_task( void* ); 
 void samsung_task( void* );
 void heavy_task( void* );
 void gree_task( void* );
 void coolgate_task( void* );
 void cg5_task( void* );
 void hitachi_task( void* );
 void tadiran_inverter_task( void* );
 void meitav_task( void* );
 void electra_task( void* );
 void simul_task( void* );
 void fujitsu_task( void* );
 void midea_task( void* );
 void m1m2_task( void* );
 void tad_clnet_task( void* ); 
 void fus_task( void* );
 void clnet_mb_task( void* );
 void p1p2_task( void* );
 void altherma_task( void* );
 void h1h2_task( void* );
 void ab_task( void* );
 void plugbus_task( void* );
 void plugbusm_task( void* );
 void mac_task( void* );
 void krp_task( void* );
 void gen_fj_task( void* );
 void hdl_task( void* );
 void siesta_task( void * );
 void haier_task( void * );

#ifdef CA_TOOLS
#define CM5_TASK( task )      0
#define CLNET_TASK( task )    0
#define COOLPLUG_TASK( task ) 0
 #elif defined CM5
  #define CM5_TASK( task )      task
  #define CLNET_TASK( task )    0
  #define COOLPLUG_TASK( task ) 0
 #elif defined CLNET
  #define CM5_TASK( task )      0
  #define CLNET_TASK( task )    task
  #define COOLPLUG_TASK( task ) 0
 #else

  #define CM5_TASK( task )      0
  #define CLNET_TASK( task )    0
  #define COOLPLUG_TASK( task ) task

#endif

#endif/*LINE_MODULE*/

#define HVAC_L_EN( L )          (0x0001 << (L))

#if defined CM5
 #define CM5_L1_EN              HVAC_L_EN( LINE_L1 )
 #define CM5_L2_EN              HVAC_L_EN( LINE_L2 )
 #define CM5_L3_EN              HVAC_L_EN( LINE_L3 )
 #define CM5_L4_EN              HVAC_L_EN( LINE_L4 )
 #define CM5_L5_EN              HVAC_L_EN( LINE_L5 )
 #define CM5_L6_EN              HVAC_L_EN( LINE_L6 )
 #define CM5_L7_EN              HVAC_L_EN( LINE_L7 )
 #define CM5_L8_EN              HVAC_L_EN( LINE_L8 )
#else
 #define CM5_L1_EN              0x0000
 #define CM5_L2_EN              0x0000
 #define CM5_L3_EN              0x0000
 #define CM5_L4_EN              0x0000
 #define CM5_L5_EN              0x0000
 #define CM5_L6_EN              0x0000
 #define CM5_L7_EN              0x0000
 #define CM5_L8_EN              0x0000
#endif
#define CM5_L_EN_ALL            CM5_L1_EN|CM5_L2_EN|CM5_L3_EN|CM5_L4_EN|    \
                                CM5_L5_EN|CM5_L6_EN|CM5_L7_EN|CM5_L8_EN
#ifdef CA_TOOLS
 #undef CLNET
 #define CLNET_L1_EN            HVAC_L_EN( LINE_L1 )<<8
 #define CLNET_L2_EN            HVAC_L_EN( LINE_L2 )<<8
 #define CLNET_L3_EN            HVAC_L_EN( LINE_L3 )<<8
 #define CLNET_L4_EN            HVAC_L_EN( LINE_L4 )<<8
 #define CLNET_L5_EN            HVAC_L_EN( LINE_L5 )<<8

#else
 #if defined CLNET
  #define CLNET_L1_EN            HVAC_L_EN( LINE_L1 )
  #define CLNET_L2_EN            HVAC_L_EN( LINE_L2 )
  #define CLNET_L3_EN            HVAC_L_EN( LINE_L3 )
  #define CLNET_L4_EN            HVAC_L_EN( LINE_L4 )
  #define CLNET_L5_EN            HVAC_L_EN( LINE_L5 )
 #else
  #define CLNET_L1_EN            0x0000
  #define CLNET_L2_EN            0x0000
  #define CLNET_L3_EN            0x0000
  #define CLNET_L4_EN            0x0000
  #define CLNET_L5_EN            0x0000
 #endif
#endif
 #define CLNET_L_EN_ALL          CLNET_L1_EN|CLNET_L2_EN|CLNET_L3_EN|        \
                                CLNET_L4_EN|CLNET_L5_EN
#ifdef CA_TOOLS
 #undef COOLPLUG
 #define COOLPLUG_L1_EN         HVAC_L_EN( LINE_L1 )<<16
 #define COOLPLUG_L2_EN         HVAC_L_EN( LINE_L2 )<<16
 #define COOLPLUG_L3_EN         HVAC_L_EN( LINE_L3 )<<16
#else
  #if defined COOLPLUG
   #define COOLPLUG_L1_EN         HVAC_L_EN( LINE_L1 )
   #define COOLPLUG_L2_EN         HVAC_L_EN( LINE_L2 )
   #define COOLPLUG_L3_EN         HVAC_L_EN( LINE_L3 )
  #else
   #define COOLPLUG_L1_EN         0x0000
   #define COOLPLUG_L2_EN         0x0000
   #define COOLPLUG_L3_EN         0x0000
 #endif
#endif

#define COOLPLUG_L_EN_ALL       COOLPLUG_L1_EN|COOLPLUG_L2_EN|COOLPLUG_L3_EN

EXTERN const line_type_descr_t line_types[]


#ifdef LINE_MODULE
={
    /*
    * LINE_FUNC enum match line_types index for backward compatibility
    */
    {   /* LINE_FUNC_UNUSED */
        "Unused",       'U',        LINE_AC_TYPE_UNUSED,        "Unused",
        CM5_L_EN_ALL|CLNET_L_EN_ALL|COOLPLUG_L_EN_ALL,
        NULL,                                                   0
    },
    {   /* LINE_FUNC_BIST_INT_LOOP */
        "BIST",         0,          LINE_AC_TYPE_UNUSED,        "Unused", 
        CM5_L_EN_ALL,
        CM5_TASK( bist_internal_port_loop_task ),               128
    },
    {   /* LINE_FUNC_DAIKIN_D3NET */
        "Daikin",       0,          LINE_AC_TYPE_DK,            "DK", 
        CM5_L1_EN|CM5_L2_EN,
        CM5_TASK( daikin_task ),                                128
    },
    {   /* LINE_FUNC_SANYO */
        "Sanyo",        0,          LINE_AC_TYPE_SA,            "SA", 
        CM5_L1_EN|CM5_L2_EN,
        CM5_TASK( sanyo_task ),                                 128
    },
    {   /* LINE_FUNC_MELCO_MNET */
        "Melco",        0,          LINE_AC_TYPE_ME,            "ME", 
        CM5_L1_EN|CM5_L2_EN,
        CM5_TASK( melco_task ),                                 128
    },
    {   /* LINE_FUNC_LG */
        "LG",           0,          LINE_AC_TYPE_LG,            "LG", 
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN,
        CM5_TASK( lg_task ),                                    128
    },
    {   /* LINE_FUNC_KNX */
        "KNX",          0,          LINE_AC_TYPE_UNUSED,        "KNX", 
        CM5_L3_EN|CLNET_L5_EN,
        CM5_TASK( knx_task )+CLNET_TASK( knx_task ),            128
    },
    {   /* LINE_FUNC_TADIRAN_INVERTER */
        "TI",           0,          LINE_AC_TYPE_TI,            "TI", 
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN|CLNET_L2_EN|CLNET_L3_EN,
        CM5_TASK( tadiran_inverter_task )+CLNET_TASK( tadiran_inverter_task ),128
    },
    {   /* LINE_FUNC_SAMSUNG */
        "Samsung",      0,          LINE_AC_TYPE_SM,            "SM", 
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN,
        CM5_TASK( samsung_task ),                               128
    },
    {   /* LINE_FUNC_TOSHIBA */
        "Toshiba",      0,          LINE_AC_TYPE_TO,            "TO", 
        CM5_L1_EN|CM5_L2_EN,
        CM5_TASK( sanyo_task ),                                 128
    },
    {   /* LINE_FUNC_HEAVY */
        "Heavy",        0,          LINE_AC_TYPE_MH,            "MH", 
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN,
        CM5_TASK( heavy_task ),                                 128
    },
    {   /* LINE_FUNC_GREE */
        "Gree",         0,          LINE_AC_TYPE_GR,            "GR", 
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN,
        CM5_TASK( gree_task ),                                  128
    },
    {   /* LINE_FUNC_COOL_GATE_4 */
        "CG4",          0,          LINE_AC_TYPE_UNUSED,        "CG4", 
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN,
        CM5_TASK( coolgate_task ),                              128
    },
    {   /* LINE_FUNC_COOL_GATE_5 */
        "CG5",          0,          LINE_AC_TYPE_UNUSED,        "CG5", 
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN,
        CM5_TASK( cg5_task ),                                   128
    },
    {   /* LINE_FUNC_HITACHI */
        "Hitachi",      0,          LINE_AC_TYPE_HT,            "HT", 
        CM5_L1_EN|CM5_L2_EN,
        CM5_TASK( hitachi_task ),                               128
    },
    {   /* LINE_FUNC_M1M2 */
        "M1M2",         'M',        LINE_AC_TYPE_M1M2,          "M1M2", 
        CLNET_L4_EN|COOLPLUG_L1_EN,
        CLNET_TASK( m1m2_task )+COOLPLUG_TASK( m1m2_task ),     128
    },
    {   /* LINE_FUNC_FUJITSU_SPLIT */
        "FUS",          'F',        LINE_AC_TYPE_FUS,           "FUS", 
        CLNET_L4_EN|COOLPLUG_L1_EN,
        CLNET_TASK( fus_task )+COOLPLUG_TASK( fus_task ),       128
    },
    {   /* LINE_FUNC_CLNET_MB */
        "CLMB",         0,          LINE_AC_TYPE_UNUSED,        "CLMB", 
        CLNET_L2_EN|CLNET_L3_EN,
        CLNET_TASK( clnet_mb_task ),                            128
    },
    {   /* LINE_FUNC_TADIRAN_CLNET */
        "TAD",          0,          LINE_AC_TYPE_TAD,           "TAD", 
        CLNET_L4_EN,
        CLNET_TASK( tad_clnet_task ),                           128
    },
    {   /* LINE_FUNC_MEITAV */
        "Meitav",       0,          LINE_AC_TYPE_MT,            "MT", 
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN|CLNET_L2_EN|CLNET_L3_EN,
        CM5_TASK( meitav_task )+CLNET_TASK( meitav_task ),      128
    },
    {   /* LINE_FUNC_SIMULATION */
        "Simul",        0,          LINE_AC_TYPE_SI,            "SI", 
        CM5_L_EN_ALL,
        CM5_TASK( simul_task ),                                 128
    },
    {   /* LINE_FUNC_FUJITSU */
        "Fujitsu",      0,          LINE_AC_TYPE_FJ,            "FJ", 
        CM5_L8_EN,
        CM5_TASK( fujitsu_task ),                               128
    },
    {   /* LINE_FUNC_P1P2 */
        "P1P2",         'P',        LINE_AC_TYPE_P1P2,          "P1P2", 
        CLNET_L4_EN|COOLPLUG_L1_EN,
        CLNET_TASK( p1p2_task )+COOLPLUG_TASK( p1p2_task ),     128
    },
    {   /* LINE_FUNC_MIDEA */
        "Midea",        0,          LINE_AC_TYPE_MD,            "MD", 
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN,
        CM5_TASK( midea_task ),                                 128
    },
    {   /* LINE_FUNC_H1H2 */
        "H1H2",         'H',        LINE_AC_TYPE_H1H2,          "H1H2", 
        CLNET_L4_EN,
        CLNET_TASK( h1h2_task ),                                128
    },
    {   /* LINE_FUNC_TRANE */
        "Trane",        0,          LINE_AC_TYPE_TR,            "TR", 
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN,
        CM5_TASK( midea_task ),                                 128
    },
    {   /* LINE_FUNC_KENTATSU */
        "Kentatsu",     0,          LINE_AC_TYPE_KT,            "KT", 
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN|CLNET_L2_EN|CLNET_L3_EN,
        CM5_TASK( midea_task )+CLNET_TASK( midea_task ),		128
    },
    {   /* LINE_FUNC_ALTHERMA */
        "Altherma",     0,          LINE_AC_TYPE_H1H2,          "AL", 
        CLNET_L4_EN,
        CLNET_TASK( altherma_task ),                            128
    },
    {   /* LINE_FUNC_AB */
        "AB",           0,          LINE_AC_TYPE_AB,            "AB", 
        CLNET_L4_EN,
        CLNET_TASK( ab_task ),                                  128
    },
    {   /* LINE_FUNC_PLUGBUS */
        "PB",           'B',        LINE_AC_TYPE_UNUSED,        "PB", 
        COOLPLUG_L2_EN,
        COOLPLUG_TASK( plugbus_task ),                          128
    },
    {   /* LINE_FUNC_PLUGBUS_MASTER */
        "PBM",          0,          LINE_AC_TYPE_PBM,           "CH", /* CoolHub */
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN|CLNET_L2_EN|CLNET_L3_EN,
        CM5_TASK( plugbusm_task )+CLNET_TASK( plugbusm_task ),  160
    },
    {   /* LINE_FUNC_PANASONIC */
        "Panasonic",    0,          LINE_AC_TYPE_PN,            "PN", 
        CM5_L1_EN|CM5_L2_EN,
        CM5_TASK( sanyo_task ),                                 128
    },
    {   /* LINE_FUNC_MAC */
        "MAC",          'C',        LINE_AC_TYPE_MAC,           "MAC", 
        COOLPLUG_L1_EN|CLNET_L4_EN/*Rev<3.0*/|CLNET_L2_EN/*Rev>=3.0*/,
        CLNET_TASK( mac_task )+COOLPLUG_TASK( mac_task ),		128
    },
    {   /* LINE_FUNC_GENERAL_FUJITSU */
        "General",      'G',        LINE_AC_TYPE_GEN_FJ,        "GEN",
        CLNET_L4_EN|COOLPLUG_L1_EN,
        CLNET_TASK( gen_fj_task )+COOLPLUG_TASK( gen_fj_task ), 128
    },
    {   /* LINE_FUNC_KRP */
        "KRP",          'K',        LINE_AC_TYPE_KRP,           "KRP", 
        COOLPLUG_L1_EN|CLNET_L4_EN/*Rev<3.0*/|CLNET_L2_EN/*Rev>=3.0*/,
        COOLPLUG_TASK( krp_task )+CLNET_TASK( krp_task ),       128
    },
    {   /* LINE_FUNC_HDL */
        "HDL",          0,          LINE_AC_TYPE_UNUSED,        "HDL", 
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN|CLNET_L2_EN|CLNET_L3_EN,
        CM5_TASK( hdl_task )+CLNET_TASK( hdl_task ),            128
    },
    {   /* LINE_FUNC_G4 */
        "G4",           0,          LINE_AC_TYPE_UNUSED,        "G4", 
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN|CLNET_L2_EN|CLNET_L3_EN,
        CM5_TASK( hdl_task )+CLNET_TASK( hdl_task ),            128
    },
    {   /* LINE_FUNC_CHIGO */
        "Chigo",        0,          LINE_AC_TYPE_KT,            "CG", 
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN,
        CM5_TASK( midea_task ),		                            128
    },
    {   /* LINE_FUNC_ELECTRA */
        "Electra",      0,          LINE_AC_TYPE_EL,           "EL", 
        CM5_L3_EN|CM5_L4_EN|CM5_L5_EN|CM5_L6_EN|CM5_L7_EN|CLNET_L2_EN|CLNET_L3_EN,
        CM5_TASK( electra_task )+CLNET_TASK( electra_task ),    128
    },
    {   /* LINE_FUNC_SIESTA */
        "Siesta",       0,          LINE_AC_TYPE_SIESTA,		"ST", 
        CLNET_L4_EN,
        CLNET_TASK( siesta_task ),    							128
    },
#if 0
    {   /* LINE_FUNC_HAIER */
        "Haier",        0,          LINE_AC_TYPE_HA,            "HA", 
        CM5_L1_EN|CM5_L2_EN,
        CM5_TASK( haier_task ),                                 128
    },
#endif
}
#endif/*LINE_MODULE*/
;

/*
*-----------------------------------------------------------------------------
* Global Function Definition
*-----------------------------------------------------------------------------
*/
void line_run_task( int ldi );
int line_type_config_check( uint16_t type, uint8_t ldi );

void line_config_detection( void );
void line_dual_L7_detect( void );
void line_polarity( line_descr_t* ld );
void line_polarity_lock( line_descr_t* ld );

int line_init( line_descr_t* ld );
void line_prepare_rx_task_name( line_descr_t* ld, uint8_t* buf );
int line_failure_check( line_descr_t* ld );
void line_tx_enable ( line_descr_t* ld );
void line_tx_disable( line_descr_t* ld );
void line_rx_enable ( line_descr_t* ld );
void line_rx_disable( line_descr_t* ld );

void line_led_on  ( line_descr_t* ld );
void line_led_on2 ( line_descr_t* ld, int tx_led );
void line_led_off ( line_descr_t* ld );
void line_led_off2( line_descr_t* ld, int tx_led );

void line_RS232_tx( line_descr_t* ld, uint8_t* buf, uint8_t sz );
void line_RS485_tx( line_descr_t* ld, uint8_t* buf, uint8_t sz );
void line_sw_uart_tx( line_descr_t* ld, uint8_t* buf, uint8_t sz );
void sw_uart_set_baud( uint32_t baud );
#ifdef MODBUS_H_INCLUDED
 void line_modbus_cmd( mbl_t* mbl, int cmd );
#endif

#ifdef CA_TOOLS

#else
void line_MM1007_tx( line_descr_t* ld, uint8_t* buf, uint8_t sz );
int line_MM1007_tx_arb( line_descr_t* ld, uint8_t* buf, uint8_t sz );
int line_MM1007_tx_arb_rtr(line_descr_t* ld, uint8_t* buf, uint8_t sz, int rtr);

int line_CIF_exec( line_descr_t* ld, line_CIF_msg_t* p_CIF_msg );
void line_CIF_done( line_CIF_msg_t* p_CIF_msg, int rc );
void line_CIF_mode2cmd( line_CIF_msg_t* p_CIF_msg, uint8_t mode );

void line_tty( line_descr_t* ld );
void line_ttm( line_descr_t* ld, line_CIF_msg_t* CIF_msg );
void line_ttm_put_byte( line_descr_t* ld, uint16_t byte );
void line_ttm_put_buffer( line_descr_t* ld, uint8_t* buf, uint8_t sz );

uint8_t get_first_used_line( void );
int line_cfg_stat( char* dst, line_descr_t* ld, uint8_t flags );

uint8_t dip_switch_p_state( void );
uint8_t dip_switch_q_state( void );
uint8_t dip_switch_r_state( void );
uint8_t dip_switch_s_state( void );
#endif
/*
*-----------------------------------------------------------------------------
* MACRO
*-----------------------------------------------------------------------------
*/
#define LINE_CNTR_INC( CNTR )	(CNTR.c32) += 0x00010001
#define LINE_ERROR_CNTR( ld )   ((ld)->to_cntr.total+(ld)->cs_cntr.total+   \
                                 (ld)->co_cntr.total+(ld)->nak_cntr.total)
#define LINE_USART_CFG( ld, line_usart_flags )								\
	ld->pVPD->flags &= ~( __LDF_USART_O_PARITY | __LDF_USART_E_PARITY |		\
						  __LDF_USART_9BIT     | __LDF_USART_2_STOP );		\
	ld->pVPD->flags |= line_usart_flags

#define GET_LINE_TYPE_BY_LDI( ldi ) line_descr[ldi].pVPD->line_type
#define GET_LINE_TYPE_BY_UID( uid ) GET_LINE_TYPE_BY_LDI(indoor_db[uid].CA.ldi)
#define GET_LINE_TYPE_BY_LD( ld )   (ld)->pVPD->line_type

#define GET_AC_TYPE_BY_UID( uid )                                   \
        (GET_LINE_TYPE_BY_UID(uid) == LINE_FUNC_PLUGBUS_MASTER) ?   \
        line_types[indoor_db[uid].plugs.line_type].ac_type :        \
        line_types[GET_LINE_TYPE_BY_UID(uid)].ac_type

#define GET_AC_TYPE_BY_LDI( ldi )   line_types[GET_LINE_TYPE_BY_LDI(ldi)].ac_type
#define GET_AC_TYPE_BY_LD( ld )     line_types[GET_LINE_TYPE_BY_LD(ld)].ac_type

#endif /*_H_INCLUDED*/

