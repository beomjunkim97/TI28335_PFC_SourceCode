/***************************************************************
    easy2833x_sci_v7.3.h
	copyright (c) 2008 by Dae-Woong Chung
	All Rights Reserved.
****************************************************************/
#ifndef _EASY2833X_SCI_H__
#define _EASY2833X_SCI_H__

extern void	easyDSP_SCI_Init(void);
extern interrupt void easy_RXINT_ISR(void);
extern interrupt void easy_TXINT_ISR(void);

#endif	// of _EASY2833X_SCI_H__
