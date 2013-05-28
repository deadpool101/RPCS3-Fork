#include "stdafx.h"
#include "Emu/SysCalls/SysCalls.h"
#include "Emu/SysCalls/SC_FUNC.h"

Module cellGcmSys("cellGcmSys", 0x0010);

s64 cellGcmFunc15()
{
	cellGcmSys.Error("cellGcmFunc15()");
	return 0;
}

s64 cellGcmSetFlipCommandWithWaitLabel()
{
	cellGcmSys.Error("cellGcmSetFlipCommandWithWaitLabel()");
	return 0;
}

struct _cellGcmSys_init
{
	_cellGcmSys_init()
	{
		cellGcmSys.AddFunc(0x055bd74d, SC_FUNC_UW_UW<cellGcmGetTiledPitchSize>);
		cellGcmSys.AddFunc(0x15bae46b, SC_FUNC_SW_UW_UW_UW_UW<cellGcmInit>);
		cellGcmSys.AddFunc(0x21397818, SC_FUNC_SW_UW_UW<cellGcmFlush>);
		cellGcmSys.AddFunc(0x21ac3697, SC_FUNC_SW_UW_UW<cellGcmAddressToOffset>);
		cellGcmSys.AddFunc(0x3a33c1fd, cellGcmFunc15);
		cellGcmSys.AddFunc(0x4ae8d215, SC_FUNC_SW_UW<cellGcmSetFlipMode>);
		cellGcmSys.AddFunc(0x5e2ee0f0, SC_FUNC_UW<cellGcmGetDefaultCommandWordSize>);
		cellGcmSys.AddFunc(0x72a577ce, SC_FUNC_SW<cellGcmGetFlipStatus>);
		cellGcmSys.AddFunc(0x8cdf8c70, SC_FUNC_UW<cellGcmGetDefaultSegmentWordSize>);
		cellGcmSys.AddFunc(0x9ba451e4, SC_FUNC_SW_UW_UW<cellGcmSetDefaultFifoSize>);
		cellGcmSys.AddFunc(0xa53d12ae, SC_FUNC_SW_UW_UW_UW_UW_UW<cellGcmSetDisplayBuffer>);
		cellGcmSys.AddFunc(0xa547adde, SC_FUNC_UW<cellGcmGetControlRegister>);
		cellGcmSys.AddFunc(0xb2e761d4, SC_FUNC_SW<cellGcmResetFlipStatus>);
		cellGcmSys.AddFunc(0xd8f88e1a, cellGcmSetFlipCommandWithWaitLabel);
		cellGcmSys.AddFunc(0xe315a0b2, SC_FUNC_SW_UW<cellGcmGetConfiguration>);
		cellGcmSys.AddFunc(0x9dc04436, SC_FUNC_SW_12UW<cellGcmBindZcull>);
	}
} _cellGcmSys_init;
