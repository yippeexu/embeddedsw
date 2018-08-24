/******************************************************************************
 *
 * Copyright (C) 2018 Xilinx, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of the Xilinx shall not be used
 * in advertising or otherwise to promote the sale, use or other dealings in
 * this Software without prior written authorization from Xilinx.
 *
 ******************************************************************************/

/*****************************************************************************/
/**
 *
 * @file xmt_write_eye.c
 *
 * This is the file containing code for DDR Write Eye Tests. This measures
 * Right Eye Edge, Left Eye Edge, Write Eye Width, Taps per Cycle and Write Eye
 * Center.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date        Changes
 * ----- ---- -------- -------------------------------------------------------
 * 1.0   mn   08/17/18 Initial release
 *
 * </pre>
 *
 * @note
 *
 ******************************************************************************/

/***************************** Include Files *********************************/

#include "xmt_common.h"

/************************** Constant Definitions *****************************/

#define XMT_LCDLR0_BASE		XMT_DDR_PHY_DX0LCDLR0
#define XMT_LCDLR1_BASE		XMT_DDR_PHY_DX0LCDLR1
#define XMT_GTR0_BASE		XMT_DDR_PHY_DX0GTR0
#define XMT_MDLR0_BASE		XMT_DDR_PHY_DX0MDLR0

#define XMT_WDQSL_MASK		0xF8FFFFFF

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/*****************************************************************************/
/**
 * This function is used to get the Write Data Delay
 *
 * @param Addr is the address of Write Data Delay register
 *
 * @return WDQD value
 *
 * @note none
 *****************************************************************************/
static INLINE u32 XMt_GetWrWdqd(u32 Addr)
{
	return XMt_GetRegValue(Addr, XMT_DDR_PHY_DX0LCDLR1_WDQD_MASK,
			       XMT_DDR_PHY_DX0LCDLR1_WDQD_SHIFT);
}

/*****************************************************************************/
/**
 * This function is used to set the Write Data Delay
 *
 * @param Addr is the address of Write Data Delay register
 * @param Val is the value to be written
 *
 * @return none
 *
 * @note none
 *****************************************************************************/
static INLINE void XMt_SetWrWdqd(u32 Addr, u32 Val)
{
	Xil_Out32(Addr, Val);
}

/*****************************************************************************/
/**
 * This function is used to get the Write Path System Latency
 *
 * @param Addr is the address of WDQSL register
 *
 * @return WDQSL value
 *
 * @note none
 *****************************************************************************/
static INLINE u32 XMt_GetWrWdqsl(u32 Addr)
{
	return XMt_GetRegValue(Addr, XMT_DDR_PHY_DX0GTR0_WDQSL_MASK,
			       XMT_DDR_PHY_DX0GTR0_WDQSL_SHIFT);
}

/*****************************************************************************/
/**
 * This function is used to set the Write Path System Latency
 *
 * @param Addr is the address of WDQSL register
 * @param Val is the Wdqsl value
 *
 * @return none
 *
 * @note none
 *****************************************************************************/
static void XMt_SetWrWdqsl(u32 Addr, u32 Val)
{
	u32 RdVal = 0;
	u32 WrVal = 0;

	RdVal = Xil_In32(Addr);
	RdVal = RdVal & XMT_WDQSL_MASK;
	WrVal =  RdVal | (Val << XMT_DDR_PHY_DX0GTR0_WDQSL_SHIFT);
	Xil_Out32(Addr, WrVal);
}

/*****************************************************************************/
/**
 * This function is used to get the Write Initial Period
 *
 * @param Addr is the address of MDL register
 *
 * @return IPRD value
 *
 * @note none
 *****************************************************************************/
static INLINE u32 XMt_GetWrIprd(u32 Addr)
{
	return XMt_GetRegValue(Addr, XMT_DDR_PHY_DX0MDLR0_IPRD_MASK,
			       XMT_DDR_PHY_DX0MDLR0_IPRD_SHIFT);
}

/*****************************************************************************/
/**
 * This function is used to get the Write Target Period
 *
 * @param Addr is the address of MDL register
 *
 * @return TPRD value
 *
 * @note none
 *****************************************************************************/
static INLINE u32 XMt_GetWrTprd(u32 Addr)
{
	return XMt_GetRegValue(Addr, XMT_DDR_PHY_DX0MDLR0_TPRD_MASK,
			       XMT_DDR_PHY_DX0MDLR0_TPRD_SHIFT);
}

/*****************************************************************************/
/**
 * This function is used to get the Write Leveling System Latency
 *
 * @param Addr is the address of GTR register
 *
 * @return WLSL value
 *
 * @note none
 *****************************************************************************/
static INLINE u32 XMt_GetWrWlsl(u32 Addr)
{
	return XMt_GetRegValue(Addr, XMT_DDR_PHY_DX0GTR0_WLSL_MASK,
			       XMT_DDR_PHY_DX0GTR0_WLSL_SHIFT);
}

/*****************************************************************************/
/**
 * This function is used to get the Write Leveling Delay
 *
 * @param Addr is the address of LCDL register
 *
 * @return WLD value
 *
 * @note none
 *****************************************************************************/
static INLINE u32 XMt_GetWrWld(u32 Addr)
{
	return XMt_GetRegValue(Addr, XMT_DDR_PHY_DX0LCDLR0_WLD_MASK,
			       XMT_DDR_PHY_DX0LCDLR0_WLD_SHIFT);
}

/*****************************************************************************/
/**
 * This function is used to get the DQS Gating System Latency
 *
 * @param Addr is the address of GTR register
 *
 * @return DGSL value
 *
 * @note none
 *****************************************************************************/
static INLINE u32 XMt_GetWrDgsl(u32 Addr)
{
	return XMt_GetRegValue(Addr, XMT_DDR_PHY_DX0GTR0_DGSL_MASK,
			       XMT_DDR_PHY_DX0GTR0_DGSL_SHIFT);
}

/*****************************************************************************/
/**
 * This function is used to clear the Tap Count
 *
 * @param XMtPtr is the pointer to the Memtest Data Structure
 *
 * @return none
 *
 * @note none
 *****************************************************************************/
static void XMt_ClearTapCount(XMt_CfgData *XMtPtr)
{
	s32 Index;

	for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++)
		XMtPtr->TapCount[Index] = 0;
}

/*****************************************************************************/
/**
 * This function is used to set the WDQD value
 *
 * @param XMtPtr is the pointer to the Memtest Data Structure
 * @param WdqdVal Initial WDQD value
 * @param WdqslVal Initial WDQSL value
 *
 * @return none
 *
 * @note none
 *****************************************************************************/
static void XMt_SetInitWdqd(XMt_CfgData *XMtPtr, u32 WdqdVal, u32 WdqslVal)
{
	s32 Index;

	for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++) {
		XMt_SetWrWdqd(XMT_LCDLR1_BASE + (XMT_LANE_OFFSET*Index),
				WdqdVal);
		XMt_SetWrWdqsl(XMT_GTR0_BASE + (XMT_LANE_OFFSET*Index),
				WdqslVal);
	}
}

/*****************************************************************************/
/**
 * This function is used to set the the Write Eye Center values
 *
 * @param XMtPtr is the pointer to the Memtest Data Structure
 *
 * @return XST_SUCCESS on success, XST_FAILURE on failure
 *
 * @note none
 *****************************************************************************/
static u32 XMt_GetWrCenter(XMt_CfgData *XMtPtr)
{
	s32 Index;

	for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++) {
		XMtPtr->WrCenter[Index].Wdqd = XMt_GetWrWdqd(XMT_LCDLR1_BASE +
				(XMT_LANE_OFFSET*Index));
		XMtPtr->WrCenter[Index].Wdqsl = XMt_GetWrWdqsl(XMT_GTR0_BASE +
				(XMT_LANE_OFFSET*Index));
		XMtPtr->WrCenter[Index].Dgsl = XMt_GetWrDgsl(XMT_GTR0_BASE +
				(XMT_LANE_OFFSET*Index));
		XMtPtr->WrCenter[Index].Iprd = XMt_GetWrIprd(XMT_MDLR0_BASE +
				(XMT_LANE_OFFSET*Index));
		XMtPtr->WrCenter[Index].Tprd = XMt_GetWrTprd(XMT_MDLR0_BASE +
				(XMT_LANE_OFFSET*Index));
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
 * This function is used to reset the the Write Eye Center values
 *
 * @param XMtPtr is the pointer to the Memtest Data Structure
 *
 * @return XST_SUCCESS on success, XST_FAILURE on failure
 *
 * @note none
 *****************************************************************************/
static u32 XMt_ResetWrCenter(XMt_CfgData *XMtPtr)
{
	s32 Index;

	for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++) {
		XMt_SetWrWdqd(XMT_LCDLR1_BASE + (XMT_LANE_OFFSET*Index),
				XMtPtr->WrCenter[Index].Wdqd);
		XMt_SetWrWdqsl(XMT_GTR0_BASE + (XMT_LANE_OFFSET*Index),
				XMtPtr->WrCenter[Index].Wdqsl);
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
 * This function is used to force re-initialize the DDR-PHY
 *
 * @param none
 *
 * @return none
 *
 * @note none
 *****************************************************************************/
static void XMt_ForceReinit(void)
{
	u32 RdVal;
	u32 WrVal;

	RdVal = Xil_In32(XMT_DDR_PHY_PIR);
	WrVal = RdVal | (1 << XMT_DDR_PHY_PIR_INIT_SHIFT);
	Xil_Out32(XMT_DDR_PHY_PIR, WrVal);
}

/*****************************************************************************/
/**
 * This function is used to populate the Write Delays
 *
 * @param XMtPtr is the pointer to the Memtest Data Structure
 *
 * @return none
 *
 * @note none
 *****************************************************************************/
static void XMt_PopulateWrDs(XMt_CfgData *XMtPtr)
{
	s32 Index;

	for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++) {
		XMtPtr->WrDs[Index].Wdqd = XMt_GetWrWdqd(XMT_LCDLR1_BASE +
				(XMT_LANE_OFFSET*Index));
		XMtPtr->WrDs[Index].Wdqsl = XMt_GetWrWdqsl(XMT_GTR0_BASE +
				(XMT_LANE_OFFSET*Index));
		XMtPtr->WrDs[Index].Wlsl = XMt_GetWrWlsl(XMT_GTR0_BASE +
				(XMT_LANE_OFFSET*Index));
		XMtPtr->WrDs[Index].Wld = XMt_GetWrWld(XMT_LCDLR0_BASE +
				(XMT_LANE_OFFSET*Index));

	}
}

/*****************************************************************************/
/**
 * This function is used to print the Tap Counts
 *
 * @param XMtPtr is the pointer to the Memtest Data Structure
 *
 * @return none
 *
 * @note none
 *****************************************************************************/
static void XMt_PrintTapCounts(XMt_CfgData *XMtPtr)
{
	s32 Index;

	xil_printf(" TAPS/cycle:\r\n");
	XMt_PrintLine(XMtPtr, 3);
	for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++)
		xil_printf("    %d   |", XMtPtr->TapCount[Index]);
}

/*****************************************************************************/
/**
 * This function is used to poll for the Training Done bits to set
 *
 * @param none
 *
 * @return none
 *
 * @note none
 *****************************************************************************/
static void XMt_PollTrainingDone(void)
{
	u32 RdVal;
	u32 IDone = 0;
	u32 ReDone = 0;


	while (!IDone && !ReDone) {
		RdVal = Xil_In32(XMT_DDR_PHY_PGSR0);
		IDone = RdVal & (1 << XMT_DDR_PHY_PGSR0_IDONE_SHIFT);
		ReDone = RdVal & (1 << XMT_DDR_PHY_PGSR0_REDONE_SHIFT);
	}
#ifdef XMT_DEBUG
	xil_printf("Training DONE\r\n");
#endif
}

/*****************************************************************************/
/**
 * This function is used to set the initial values in DDR-PHY registers
 *
 * @param XMtPtr is the pointer to the Memtest Data Structure
 *
 * @return XST_SUCCESS on success, XST_FAILURE on failure
 *
 * @note none
 *****************************************************************************/
static u32 XMt_SetWdqdSw(XMt_CfgData *XMtPtr)
{
	s32 Index;
	u32 InitIprd;
	u32 Iprd;

	#ifdef XMT_DEBUG
	xil_printf("\r\n");
	xil_printf("WR_CENTER (WDQD/WDQSL): ");
	for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++)
		xil_printf("L%d(%u/%u), ", Index, XMtPtr->WrCenter[Index].Wdqd,
				XMtPtr->WrCenter[Index].Wdqsl);
	xil_printf("\r\n");
	#endif

	XMt_ClearTapCount(XMtPtr);
	Iprd = XMt_GetWrIprd(XMT_MDLR0_BASE);
	InitIprd = Iprd + 20;
	#ifdef XMT_DEBUG
	xil_printf("Init IPRD: %u+20=%u\r\n\r\n", Iprd, InitIprd);
	#endif

	XMt_SetInitWdqd(XMtPtr, InitIprd, 0);
	XMt_ForceReinit();
	XMt_PollTrainingDone();

	XMt_PopulateWrDs(XMtPtr);

#ifdef XMT_DEBUG
	xil_printf("XMt_WriteDs (WDQD/WDQSL) after re-training: ");
#endif

	for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++) {
#ifdef XMT_DEBUG
		xil_printf("L%d(%u/%u), ", Index, XMtPtr->WrDs[Index].Wdqd,
				XMtPtr->WrDs[Index].Wdqsl);
#endif
		XMtPtr->TapCount[Index] = InitIprd - XMtPtr->WrDs[Index].Wdqd;
		XMt_SetWrWdqd(XMT_LCDLR1_BASE + (XMT_LANE_OFFSET*Index),
				XMtPtr->WrCenter[Index].Wdqd);
		XMt_SetWrWdqsl(XMT_GTR0_BASE + (XMT_LANE_OFFSET*Index),
				XMtPtr->WrCenter[Index].Wdqsl);
	}
#ifdef XMT_DEBUG
	xil_printf("\r\n");
#endif

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
 * This function is used to print the Write Eye Center values
 *
 * @param XMtPtr is the pointer to the Memtest Data Structure
 *
 * @return none
 *
 * @note none
 *****************************************************************************/
static void XMt_PrintWrCenter(XMt_CfgData *XMtPtr)
{
	s32 Index;

	xil_printf(" AUTO CENTER:\r\n");
	XMt_PrintLine(XMtPtr, 3);
	for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++)
		xil_printf("  %2d,%2d  |", XMtPtr->WrCenter[Index].Wdqsl,
				XMtPtr->WrCenter[Index].Wdqd);
}

/*****************************************************************************/
/**
 * This function is used to set the WDQD and WDQSL values in registers
 *
 * @param XMtPtr is the pointer to the Memtest Data Structure
 * @param Position is the iterative distance from center
 *
 * @return none
 *
 * @note none
 *****************************************************************************/
static void XMt_SetWdqdWithIncr(XMt_CfgData *XMtPtr, s32 Position)
{
	s32 Index;
	s32 XFine;
	s32 Xc;
	s32 Tc;

	for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++) {
		Tc = XMtPtr->TapCount[Index];
		XFine = XMtPtr->WrCenter[Index].Wdqd + Position;
		Xc = XMtPtr->WrCenter[Index].Wdqsl;

		if (Tc > 0) {
			while (XFine >= Tc) {
				XFine = XFine - Tc;
				Xc++;
			}
			while (XFine < 0) {
				XFine = XFine + Tc;
				Xc--;
			}
			if (Xc < 0) {
				xil_printf("Underflow on WR_Lane%d\r\n", Index);
				XFine = 0;
				Xc = 0;
			}
			#ifdef XMT_DEBUG
			xil_printf("L%d(WDQD=%u,WDQSL=%u), ", Index, XFine, Xc);
			#endif
			XMt_SetWrWdqd(XMT_LCDLR1_BASE + (XMT_LANE_OFFSET*Index),
					XFine);
			XMt_SetWrWdqsl(XMT_GTR0_BASE + (XMT_LANE_OFFSET*Index),
					Xc);
		}
	}
	#ifdef XMT_DEBUG
	xil_printf("\r\n");
	#endif
}

/*****************************************************************************/
/**
 * This function is used to measure the Write Eye Edge values of the DDR.
 *
 * @param XMtPtr is the pointer to the Memtest Data Structure
 * @param TestAddr is the Starting Address
 * @param Len is the length of the memory to be tested
 * @param Mode is the flag indication whether to test right eye or left eye
 *
 * @return XST_SUCCESS on success, XST_FAILURE on failure
 *
 * @note none
 *****************************************************************************/
static u32 XMt_MeasureWrEyeEdge(XMt_CfgData *XMtPtr, u64 TestAddr, u32 Len, u8 Mode)
{
	u32 Index;
	s32 Done;
	s32 Position;

	Done = 0;
	Position = 0;

	while (!Done) {
		if (Mode == XMT_RIGHT_EYE_TEST) {
			/* Move towards right edge */
			Position = Position + 1;
		} else {
			/* Move towards left edge */
			Position = Position - 1;
		}

		/* Clear system registers */
		XMt_ClearResults(XMtPtr, XMT_RESULTS_BASE);

		xil_printf("%3d    |", Position);

		/* Set the WDQD and WDQSL register values based on position */
		XMt_SetWdqdWithIncr(XMtPtr, Position);

		/* Do the Write/Read test on Address Range */
		XMt_RunEyeMemtest(XMtPtr, TestAddr, Len);

		/* Print the lane wise results for this Position */
		XMt_PrintResults(XMtPtr);

		/* Calculate the Eye Start/End position values */
		for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++) {
			if (Mode == XMT_RIGHT_EYE_TEST) {
				if (Xil_In32(XMT_RESULTS_BASE+(Index*4)) != 0 &&
						XMtPtr->EyeEnd[Index] == 0) {
					XMtPtr->EyeEnd[Index] = Position-1;
				}
			} else {
				if (Xil_In32(XMT_RESULTS_BASE +
						(Index * 4)) != 0 &&
						XMtPtr->EyeStart[Index] == 0) {
					XMtPtr->EyeStart[Index] = Position+1;
				}
			}
		}

		/* Once all Eye Start/End values are non-zero, End the test */
		Done = 1;
		for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++) {
			if (Mode == XMT_RIGHT_EYE_TEST) {
				if (XMtPtr->EyeEnd[Index] == 0)
					Done = 0;
			} else {
				if (XMtPtr->EyeStart[Index] == 0)
					Done = 0;
			}
		}
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
 * This function is used to print the Write Eye Test Results
 *
 * @param XMtPtr is the pointer to the Memtest Data Structure
 *
 * @return none
 *
 * @note none
 *****************************************************************************/
static void XMt_PrintWriteEyeResults(XMt_CfgData *XMtPtr)
{
	u32 Index;
	u32 RawCenters[8];
	float TempVal;

	XMt_PrintLine(XMtPtr, 2);
	/* calculate the average */
	xil_printf("\r\nWrite Eye Test Results:\r\n");

	XMt_PrintEyeResultsHeader(XMtPtr);
	XMt_PrintWrCenter(XMtPtr);
	xil_printf("\r\n");

	XMt_PrintLine(XMtPtr, 3);
	xil_printf(" TAP Value (ps):\r\n");
	XMt_PrintLine(XMtPtr, 3);

	for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++) {
		TempVal = (float)((1000000 / (XMtPtr->DdrFreq * 4)) /
				(float) XMtPtr->TapCount[Index]);
		xil_printf("   %d.%02d  |", (int)TempVal, (int)((TempVal - (int)TempVal)*100.0f));
	}
	xil_printf("\r\n");

	XMt_PrintLine(XMtPtr, 3);

	XMt_PrintTapCounts(XMtPtr);
	xil_printf("\r\n");

	XMt_PrintLine(XMtPtr, 3);
	xil_printf(" EYE WIDTH (ps):\r\n");
	XMt_PrintLine(XMtPtr, 3);

	for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++) {
		XMtPtr->TapPs = XMt_CalcPerTapDelay(XMtPtr, Index);
		TempVal = (float) (XMtPtr->TapPs * (XMtPtr->EyeEnd[Index] -
				XMtPtr->EyeStart[Index]));
		xil_printf(" %d.%02d  |", (int)TempVal, (int)((TempVal - (int)TempVal)*100.0f));
	}
	xil_printf("\r\n");

	XMt_PrintLine(XMtPtr, 3);
	xil_printf(" EYE WIDTH (%%):\r\n");
	XMt_PrintLine(XMtPtr, 3);

	for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++) {
		XMtPtr->TapPs = XMt_CalcPerTapDelay(XMtPtr, Index);
		TempVal = 100 * (XMtPtr->TapPs * (XMtPtr->EyeEnd[Index] -
				XMtPtr->EyeStart[Index])) / (1000000 / (XMtPtr->DdrFreq * 4));
		xil_printf("  %d.%02d  |", (int)TempVal, (int)((TempVal - (int)TempVal)*100.0f));
	}
	xil_printf("\r\n");

	XMt_PrintLine(XMtPtr, 3);
	xil_printf(" EYE CENTER:\r\n");
	XMt_PrintLine(XMtPtr, 3);

	for (Index = 0; Index < XMtPtr->DdrConfigLanes; Index++) {
		RawCenters[Index] = (XMtPtr->WrCenter[Index].Wdqsl * XMtPtr->TapCount[Index]) +
				XMtPtr->WrCenter[Index].Wdqd;
		RawCenters[Index] = ((RawCenters[Index] + XMtPtr->EyeStart[Index]) +
				(RawCenters[Index] + XMtPtr->EyeEnd[Index])) / 2;
		xil_printf("  %2d,%2d  |", RawCenters[Index] / XMtPtr->TapCount[Index],
				RawCenters[Index] % XMtPtr->TapCount[Index]);
	}
	xil_printf("\r\n");

	XMt_PrintLine(XMtPtr, 3);
}

/*****************************************************************************/
/**
 * This function is used for exception handling while running this test
 *
 * @param none
 *
 * @return none
 *
 * @note none
 *****************************************************************************/
static void XMt_WrEyeSyncAbortHandler(void)
{
	u64 RetAddr;

	RetAddr = mfelrel3();
	RetAddr = RetAddr + 4;
	mtelrel3(RetAddr);
}

/*****************************************************************************/
/**
 * This function is used for exception handling while running this test
 *
 * @param none
 *
 * @return none
 *
 * @note none
 *****************************************************************************/
static void XMt_WrEyeSerrorAbortHandler(void)
{
	;
}

/*****************************************************************************/
/**
 * This function is used to measure the Write Eye of the DDR.
 *
 * @param XMtPtr is the pointer to the Memtest Data Structure
 * @param TestAddr is the Starting Address
 * @param Len is the length of the memory to be tested
 *
 * @return XST_SUCCESS on success, XST_FAILURE on failure
 *
 * @note none
 *****************************************************************************/
u32 XMt_MeasureWrEye(XMt_CfgData *XMtPtr, u64 TestAddr, u32 Len)
{
	Xil_ExceptionHandler SyncHandler;
	Xil_ExceptionHandler SerrorHandler;
	void *SyncData;
	void *SerrorData;
	u32 Status;

	xil_printf("\r\nRunning Write Eye Tests\r\n");

	/* Get the system handlers for Sync and SError exceptions */
	Xil_GetExceptionRegisterHandler(XIL_EXCEPTION_ID_SYNC_INT,
			&SyncHandler, &SyncData);
	Xil_GetExceptionRegisterHandler(XIL_EXCEPTION_ID_SERROR_ABORT_INT,
			&SerrorHandler, &SerrorData);

	/* Register the Exception Handlers for Sync and Serror exceptions */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_SYNC_INT,
			(Xil_ExceptionHandler)XMt_WrEyeSyncAbortHandler,
			(void *) 0);
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_SERROR_ABORT_INT,
			(Xil_ExceptionHandler)XMt_WrEyeSerrorAbortHandler,
			(void *) 0);


	/* Initialize Eye Parameters with zero */
	XMt_ClearEye(XMtPtr, (u32 *)&XMtPtr->EyeStart[0]);
	XMt_ClearEye(XMtPtr, (u32 *)&XMtPtr->EyeEnd[0]);

	/* Clear system registers */
	XMt_ClearResults(XMtPtr, XMT_RESULTS_BASE);

	/* Disable VT compensation */
	XMt_DisableVtcomp();

	/* Get the Write Eye Center Values */
	Status = XMt_GetWrCenter(XMtPtr);
	if (Status != XST_SUCCESS) {
		Status = XST_FAILURE;
		goto RETURN_PATH;
	}

	/* Set the Initial WDQD via software method */
	Status = XMt_SetWdqdSw(XMtPtr);
	if (Status != XST_SUCCESS) {
		Status = XST_FAILURE;
		goto RETURN_PATH;
	}


	/* Print the Eye Test Header */
	XMt_PrintEyeHeader(XMtPtr);

	/* Measure the Right Edge of the Eye */
	Status = XMt_MeasureWrEyeEdge(XMtPtr, TestAddr, Len, XMT_RIGHT_EYE_TEST);
	if (Status != XST_SUCCESS) {
		Status = XST_FAILURE;
		goto RETURN_PATH;
	}

	/* Measure the Left Edge of the Eye */
	Status = XMt_MeasureWrEyeEdge(XMtPtr, TestAddr, Len, XMT_LEFT_EYE_TEST);
	if (Status != XST_SUCCESS) {
		Status = XST_FAILURE;
		goto RETURN_PATH;
	}

	/* Print the Write Eye Test Results */
	XMt_PrintWriteEyeResults(XMtPtr);

	/* Reset the Write Eye Center values to Registers */
	Status = XMt_ResetWrCenter(XMtPtr);
	if (Status != XST_SUCCESS) {
		Status = XST_FAILURE;
		goto RETURN_PATH;
	}

	/* Give back Exception Handling to the system defined handlers */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_SYNC_INT,
			SyncHandler, SyncData);
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_SERROR_ABORT_INT,
			SerrorHandler, SerrorData);

RETURN_PATH:
	return Status;
}