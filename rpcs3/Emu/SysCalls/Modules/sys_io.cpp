#include "stdafx.h"
#include "Emu/SysCalls/SysCalls.h"
#include "Emu/SysCalls/SC_FUNC.h"

Module sys_io("sys_io", 0x0017);

struct _sys_io_init
{
	_sys_io_init()
	{
		sys_io.AddFunc(0x1cf98800, SC_FUNC_SW_UW<cellPadInit>);
		sys_io.AddFunc(0x4d9b75d5, SC_FUNC_SW<cellPadEnd>);
		sys_io.AddFunc(0x0d5f2c14, SC_FUNC_SW_UW<cellPadClearBuf>);
		sys_io.AddFunc(0x8b72cda1, SC_FUNC_SW_UW_UW<cellPadGetData>);
		sys_io.AddFunc(0x6bc09c61, SC_FUNC_SW_UW_UW_UW<cellPadGetDataExtra>);
		sys_io.AddFunc(0xf65544ee, SC_FUNC_SW_UW_UW<cellPadSetActDirect>);
		sys_io.AddFunc(0xa703a51d, SC_FUNC_SW_UW<cellPadGetInfo2>);
		sys_io.AddFunc(0x578e3c98, SC_FUNC_SW_UW_UW<cellPadSetPortSetting>);
	}
} sys_io_init;