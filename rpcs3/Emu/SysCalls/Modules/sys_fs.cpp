#include "stdafx.h"
#include "Emu/SysCalls/SysCalls.h"
#include "Emu/SysCalls/SC_FUNC.h"

Module sys_fs("sys_fs", 0x000e);

struct _sys_fs_init
{
	_sys_fs_init()
	{
		sys_fs.AddFunc(0x718bf5f8, SC_FUNC_SW_UW_SW_UW_UW_UD<cellFsOpen>);
		sys_fs.AddFunc(0x4d5ff8e2, SC_FUNC_SW_UW_UW_UD_UW<cellFsRead>);
		sys_fs.AddFunc(0xecdcf2ab, SC_FUNC_SW_UW_UW_UD_UW<cellFsWrite>);
		sys_fs.AddFunc(0x2cb51f0d, SC_FUNC_SW_UW<cellFsClose>);
		sys_fs.AddFunc(0x3f61245c, SC_FUNC_SW_UW_UW<cellFsOpendir>);
		sys_fs.AddFunc(0x5c74903d, SC_FUNC_SW_UW_UW_UW<cellFsReaddir>);
		sys_fs.AddFunc(0xff42dcc3, SC_FUNC_SW_UW<cellFsClosedir>);
		sys_fs.AddFunc(0x7de6dced, SC_FUNC_SW_UW_UW<cellFsStat>);
		sys_fs.AddFunc(0xef3efa34, SC_FUNC_SW_UW_UW<cellFsFstat>);
		sys_fs.AddFunc(0xba901fe6, SC_FUNC_SW_UW_UW<cellFsMkdir>);
		sys_fs.AddFunc(0xf12eecc8, SC_FUNC_SW_UW_UW<cellFsRename>);
		sys_fs.AddFunc(0x2796fdf3, SC_FUNC_SW_UW<cellFsRmdir>);
		sys_fs.AddFunc(0x7f4677a8, SC_FUNC_SW_UW<cellFsUnlink>);
		sys_fs.AddFunc(0xa397d042, SC_FUNC_SW_UW_SD_UW_UW<cellFsLseek>);
	}
} sys_fs_init;