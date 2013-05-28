#include "stdafx.h"
#include "Emu/SysCalls/SysCalls.h"
#include "Emu/SysCalls/SC_FUNC.h"

Module sysPrxForUser("sysPrxForUser", -1);

void sys_initialize_tls()
{
	sysPrxForUser.Log("sys_initialize_tls()");
}

s64 sys_process_atexitspawn()
{
	sysPrxForUser.Log("sys_process_atexitspawn()");
	return 0;
}

s64 sys_process_at_Exitspawn()
{
	sysPrxForUser.Log("sys_process_at_Exitspawn");
	return 0;
}

s64 sys_prx_register_library()
{
	sysPrxForUser.Error("sys_prx_register_library()");
	return 0;
}

s64 sys_prx_exitspawn_with_level()
{
	sysPrxForUser.Log("sys_prx_exitspawn_with_level()");
	return 0;
}

s64 sys_strlen(u32 addr)
{
	const wxString& str = Memory.ReadString(addr);
	sysPrxForUser.Log("sys_strlen(\"%s\")", str);
	return str.Len();
}

struct _sysPrxForUser_init
{
	_sysPrxForUser_init()
	{
		sysPrxForUser.AddFunc(0x744680a2, SC_FUNC_V<sys_initialize_tls>);

		sysPrxForUser.AddFunc(0x2f85c0ef, SC_FUNC_SW_UD_UD<sys_lwmutex_create>);
		sysPrxForUser.AddFunc(0xc3476d0c, SC_FUNC_SW_UD<sys_lwmutex_destroy>);
		sysPrxForUser.AddFunc(0x1573dc3f, SC_FUNC_SW_UD_UD<sys_lwmutex_lock>);
		sysPrxForUser.AddFunc(0xaeb78725, SC_FUNC_SW_UD<sys_lwmutex_trylock>);
		sysPrxForUser.AddFunc(0x1bc200f4, SC_FUNC_SW_UD<sys_lwmutex_unlock>);

		sysPrxForUser.AddFunc(0x8461e528, sys_time_get_system_time);

		sysPrxForUser.AddFunc(0xe6f2c1e7, SC_FUNC_SW_SW<sys_process_exit>);
		sysPrxForUser.AddFunc(0x2c847572, sys_process_atexitspawn);
		sysPrxForUser.AddFunc(0x96328741, sys_process_at_Exitspawn);

		sysPrxForUser.AddFunc(0x24a1ea07, SC_FUNC_SW_UW_UW_UW_SW_UW_UD_UW<sys_ppu_thread_create>);
		sysPrxForUser.AddFunc(0x350d454e, SC_FUNC_SW<sys_ppu_thread_get_id>);
		sysPrxForUser.AddFunc(0xaff080a4, SC_FUNC_SW_SW<sys_ppu_thread_exit>);

		sysPrxForUser.AddFunc(0x42b23552, sys_prx_register_library);
		sysPrxForUser.AddFunc(0xa2c7ba64, sys_prx_exitspawn_with_level);

		sysPrxForUser.AddFunc(0x2d36462b, SC_FUNC_SD_UW<sys_strlen>);

		sysPrxForUser.AddFunc(0x35168520, SC_FUNC_SW_UW_UW<sys_heap_malloc>);
		//sysPrxForUser.AddFunc(0xaede4b03, SC_FUNC_SW_UW<sys_heap_free>);
		//sysPrxForUser.AddFunc(0x8a561d92, SC_FUNC_SW_UW<sys_heap_delete_heap>);
		sysPrxForUser.AddFunc(0xb2fcf2c8, SC_FUNC_SW_UW_UW_UW<sys_heap_create_heap>);
	}
} sysPrxForUser_init;