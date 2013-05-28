#include "stdafx.h"
#include "SysCalls.h"
#include "SC_FUNC.h"

ArrayF<ModuleFunc> g_modules_funcs_list;
ArrayF<Module> g_modules_list;

bool IsLoadedFunc(u32 id)
{
	for(u32 i=0; i<g_modules_funcs_list.GetCount(); ++i)
	{
		if(g_modules_funcs_list[i].id == id) return true;
	}

	return false;
}

bool CallFunc(u32 id, s64& ret)
{
	for(u32 i=0; i<g_modules_funcs_list.GetCount(); ++i)
	{
		if(g_modules_funcs_list[i].id == id)
		{
			ret = g_modules_funcs_list[i].func();

			return true;
		}
	}

	return false;
}

bool UnloadFunc(u32 id)
{
	for(u32 i=0; i<g_modules_funcs_list.GetCount(); ++i)
	{
		if(g_modules_funcs_list[i].id == id)
		{
			g_modules_funcs_list.RemoveAt(i);

			return true;
		}
	}

	return false;
}

void UnloadModules()
{
	for(u32 i=0; i<g_modules_list.GetCount(); ++i)
	{
		g_modules_list[i].SetLoaded(false);
	}

	g_modules_funcs_list.Clear();
}

Module* GetModuleByName(const wxString& name)
{
	for(u32 i=0; i<g_modules_list.GetCount(); ++i)
	{
		if(g_modules_list[i].GetName().Cmp(name) == 0) return &g_modules_list[i];
	}

	return nullptr;
}

Module* GetModuleById(u16 id)
{
	for(u32 i=0; i<g_modules_list.GetCount(); ++i)
	{
		if(g_modules_list[i].GetID() == id) return &g_modules_list[i];
	}

	return nullptr;
}

Module::Module(const char* name, u16 id)
	: m_is_loaded(false)
	, m_name(name)
	, m_id(id)
{
	g_modules_list.Add(this);
}

void Module::Load()
{
	for(u32 i=0; i<m_funcs_list.GetCount(); ++i)
	{
		if(IsLoadedFunc(m_funcs_list[i].id)) continue;

		g_modules_funcs_list.Add(m_funcs_list[i]);
	}
}

void Module::UnLoad()
{
	for(u32 i=0; i<m_funcs_list.GetCount(); ++i)
	{
		UnloadFunc(m_funcs_list[i].id);
	}
}

bool Module::Load(u32 id)
{
	if(IsLoadedFunc(id)) return false;

	for(u32 i=0; i<m_funcs_list.GetCount(); ++i)
	{
		if(m_funcs_list[i].id == id)
		{
			g_modules_funcs_list.Add(m_funcs_list[i]);

			return true;
		}
	}

	return false;
}

bool Module::UnLoad(u32 id)
{
	return UnloadFunc(id);
}

static const SC_FUNC sc_table[1024] = 
{
	0, SC_FUNC_SW<sys_process_getpid>, 0, SC_FUNC_SW_SW<sys_process_exit>, 0, //4
	0, 0, 0, 0, 0, //9
	0, 0, 0, 0, 0, //14
	0, 0, 0, 0, 0, //19
	0, 0, SC_FUNC_SW_SW<sys_process_exit>, 0, 0, //24
	0, 0, 0, 0, 0, //29
	0, 0, 0, 0, 0, //34
	0, 0, 0, 0, 0, //39
	0, SC_FUNC_SW_SW<sys_ppu_thread_exit>, 0, SC_FUNC_SW<sys_ppu_thread_yield>, SC_FUNC_SW_UW_UW<sys_ppu_thread_join>, //44
	SC_FUNC_SW_UW<sys_ppu_thread_detach>, SC_FUNC_SW_UW<sys_ppu_thread_detach>, SC_FUNC_V_UW<sys_ppu_thread_get_join_state>, SC_FUNC_SW_UW_SW<sys_ppu_thread_set_priority>, SC_FUNC_SW_UW_UW<sys_ppu_thread_get_priority>, //49
	SC_FUNC_SW_UW<sys_ppu_thread_stop>, SC_FUNC_SW_UW<sys_ppu_thread_restart>, SC_FUNC_SW_UW_UW_UW_SW_UW_UD_UW<sys_ppu_thread_create>, 0, 0, //54
	0, 0, 0, 0, 0, //59
	0, 0, 0, 0, 0, //64
	0, 0, 0, 0, 0, //69
	0, 0, 0, 0, 0, //74
	0, 0, 0, 0, 0, //49
	0, 0, 0, 0, 0, //84
	0, 0, 0, 0, 0, //89
	SC_FUNC_SW_UW_UW_SW_SW<sys_semaphore_create>, SC_FUNC_SW_UW<sys_semaphore_destroy>, SC_FUNC_SW_UW_UD<sys_semaphore_wait>, SC_FUNC_SW_UW<sys_semaphore_trywait>, SC_FUNC_SW_UW_SW<sys_semaphore_post>, //94
	SC_FUNC_SW_UD_UD<sys_lwmutex_create>, SC_FUNC_SW_UD<sys_lwmutex_destroy>, SC_FUNC_SW_UD_UD<sys_lwmutex_lock>, SC_FUNC_SW_UD<sys_lwmutex_trylock>, SC_FUNC_SW_UD<sys_lwmutex_unlock>, //99
	SC_FUNC_SW_UW_UW<sys_mutex_create>, SC_FUNC_SW_UW<sys_mutex_destroy>, SC_FUNC_SW_UW_UD<sys_mutex_lock>, SC_FUNC_SW_UW<sys_mutex_trylock>, SC_FUNC_SW_UW<sys_mutex_unlock>, //104
	SC_FUNC_SW_UW_UW_UW<sys_cond_create>, SC_FUNC_SW_UW<sys_cond_destroy>, SC_FUNC_SW_UW_UD<sys_cond_wait>, SC_FUNC_SW_UW<sys_cond_signal>, SC_FUNC_SW_UW<sys_cond_signal_all>, //109
	0, 0, 0, 0, 0, //114
	0, 0, 0, 0, 0, //119
	0, 0, 0, 0, 0, //124
	0, 0, 0, 0, 0, //129
	0, 0, 0, 0, 0, //134
	0, 0, 0, 0, 0, //139
	0, 0, 0, 0, 0, //144
	SC_FUNC_SW_UW_UW<sys_time_get_current_time>, SC_FUNC_SD<sys_time_get_system_time>, SC_FUNC_UD<sys_time_get_timebase_frequency>, 0, 0, //149
	0, 0, 0, 0, 0, //154
	0, 0, 0, 0, 0, //159
	SC_FUNC_SW_UW_UW<sys_raw_spu_create>, 0, 0, 0, 0, //164
	0, 0, 0, 0, SC_FUNC_SW_UW_UW<sys_spu_initialize>, //169
	SC_FUNC_SW_UD_UW_SW_UD<sys_spu_thread_group_create>, 0, 0, 0, 0, //174
	0, 0, 0, 0, 0, //179
	0, 0, 0, 0, 0, //184
	0, 0, 0, 0, 0, //189
	0, 0, 0, 0, 0, //194
	0, 0, 0, 0, 0, //199
	0, 0, 0, 0, 0, //204
	0, 0, 0, 0, 0, //209
	0, 0, 0, 0, 0, //214
	0, 0, 0, 0, 0, //219
	0, 0, 0, 0, 0, //224
	0, 0, 0, 0, 0, //229
	0, 0, 0, 0, 0, //234
	0, 0, 0, 0, 0, //239
	0, 0, 0, 0, 0, //244
	0, 0, 0, 0, 0, //249
	0, 0, 0, 0, 0, //254
	0, 0, 0, 0, 0, //259
	0, 0, 0, 0, 0, //264
	0, 0, 0, 0, 0, //269
	0, 0, 0, 0, 0, //274
	0, 0, 0, 0, 0, //279
	0, 0, 0, 0, 0, //284
	0, 0, 0, 0, 0, //289
	0, 0, 0, 0, 0, //294
	0, 0, 0, 0, 0, //299
	0, 0, 0, 0, 0, //304
	0, 0, 0, 0, 0, //309
	0, 0, 0, 0, 0, //314
	0, 0, 0, 0, 0, //319
	0, 0, 0, 0, SC_FUNC_SW_UW_UW<sys_memory_container_create>, //324
	SC_FUNC_SW_UW<sys_memory_container_destroy>, 0, 0, 0, 0, //329
	SC_FUNC_SW_UW_UD_UW_UW<sys_mmapper_allocate_address>, 0, 0, 0, 0, //334
	0, 0, 0, 0, 0, //339
	0, 0, 0, 0, 0, //344
	0, 0, 0, SC_FUNC_SW_UW_UW_UW<sys_memory_allocate>, SC_FUNC_SW_UW<sys_memory_free>, //349
	0, 0, SC_FUNC_SW_UW<sys_memory_get_user_memory_size>, 0, 0, //354
	0, 0, 0, 0, 0, //359
	0, 0, 0, 0, 0, //364
	0, 0, 0, 0, 0, //369
	0, 0, 0, 0, 0, //374
	0, 0, 0, 0, 0, //379
	0, 0, 0, 0, 0, //384
	0, 0, 0, 0, 0, //389
	0, 0, 0, 0, 0, //394
	0, 0, 0, 0, 0, //399
	0, 0, SC_FUNC_SW_UW_UD_UW_UD<sys_tty_read>, SC_FUNC_SW_UW_UD_UW_UD<sys_tty_write>, 0, //404
	0, 0, 0, 0, 0, //409
	0, 0, 0, 0, 0, //414
	0, 0, 0, 0, 0, //419
	0, 0, 0, 0, 0, //424
	0, 0, 0, 0, 0, //429
	0, 0, 0, 0, 0, //434
	0, 0, 0, 0, 0, //439
	0, 0, 0, 0, 0, //444
	0, 0, 0, 0, 0, //449
	0, 0, 0, 0, 0, //454
	0, 0, 0, 0, 0, //459
	0, 0, 0, 0, 0, //464
	0, 0, 0, 0, 0, //469
	0, 0, 0, 0, 0, //474
	0, 0, 0, 0, 0, //479
	0, 0, 0, 0, 0, //484
	0, 0, 0, 0, 0, //489
	0, 0, 0, 0, 0, //494
	0, 0, 0, 0, 0, //499
	0, 0, 0, 0, 0, //504
	0, 0, 0, 0, 0, //509
	0, 0, 0, 0, 0, //514
	0, 0, 0, 0, 0, //519
	0, 0, 0, 0, 0, //524
	0, 0, 0, 0, 0, //529
	0, 0, 0, 0, 0, //534
	0, 0, 0, 0, 0, //539
	0, 0, 0, 0, 0, //544
	0, 0, 0, 0, 0, //549
	0, 0, 0, 0, 0, //554
	0, 0, 0, 0, 0, //559
	0, 0, 0, 0, 0, //564
	0, 0, 0, 0, 0, //569
	0, 0, 0, 0, 0, //574
	0, 0, 0, 0, 0, //579
	0, 0, 0, 0, 0, //584
	0, 0, 0, 0, 0, //589
	0, 0, 0, 0, 0, //594
	0, 0, 0, 0, 0, //599
	0, 0, 0, 0, 0, //604
	0, 0, 0, 0, 0, //609
	0, 0, 0, 0, 0, //614
	0, 0, 0, 0, 0, //619
	0, 0, 0, 0, 0, //624
	0, 0, 0, 0, 0, //629
	0, 0, 0, 0, 0, //634
	0, 0, 0, 0, 0, //639
	0, 0, 0, 0, 0, //644
	0, 0, 0, 0, 0, //649
	0, 0, 0, 0, 0, //654
	0, 0, 0, 0, 0, //659
	0, 0, 0, 0, 0, //664
	0, 0, 0, 0, 0, //669
	0, 0, 0, 0, 0, //674
	0, 0, 0, 0, 0, //679
	0, 0, 0, 0, 0, //684
	0, 0, 0, 0, 0, //689
	0, 0, 0, 0, 0, //694
	0, 0, 0, 0, 0, //699
	0, 0, 0, 0, 0, //704
	0, 0, 0, 0, 0, //709
	0, 0, 0, 0, 0, //714
	0, 0, 0, 0, 0, //719
	0, 0, 0, 0, 0, //724
	0, 0, 0, 0, 0, //729
	0, 0, 0, 0, 0, //734
	0, 0, 0, 0, 0, //739
	0, 0, 0, 0, 0, //744
	0, 0, 0, 0, 0, //749
	0, 0, 0, 0, 0, //754
	0, 0, 0, 0, 0, //759
	0, 0, 0, 0, 0, //764
	0, 0, 0, 0, 0, //769
	0, 0, 0, 0, 0, //774
	0, 0, 0, 0, 0, //779
	0, 0, 0, 0, 0, //784
	0, 0, 0, 0, 0, //789
	0, 0, 0, 0, 0, //794
	0, 0, 0, 0, 0, //799
	0, SC_FUNC_SW_UW_SW_UW_UW_UD<cellFsOpen>, SC_FUNC_SW_UW_UW_UD_UW<cellFsRead>, SC_FUNC_SW_UW_UW_UD_UW<cellFsWrite>, SC_FUNC_SW_UW<cellFsClose>, //804
	SC_FUNC_SW_UW_UW<cellFsOpendir>, SC_FUNC_SW_UW_UW_UW<cellFsReaddir>, SC_FUNC_SW_UW<cellFsClosedir>, 0, SC_FUNC_SW_UW_UW<cellFsFstat>, //809
	0, SC_FUNC_SW_UW_UW<cellFsMkdir>, SC_FUNC_SW_UW_UW<cellFsRename>, SC_FUNC_SW_UW<cellFsRmdir>, 0, //814
	0, 0, 0, SC_FUNC_SW_UW_SD_UW_UW<cellFsLseek>, 0, //819
	0, 0, 0, 0, 0, //824
	0, 0, 0, 0, 0, //829
	0, 0, 0, 0, 0, //834
	0, 0, 0, 0, 0, //839
	0, 0, 0, 0, 0, //844
	0, 0, 0, 0, 0, //849
	0, 0, 0, 0, 0, //854
	0, 0, 0, 0, 0, //859
	0, 0, 0, 0, 0, //864
	0, 0, 0, 0, 0, //869
	0, 0, 0, 0, 0, //874
	0, 0, 0, 0, 0, //879
	0, 0, 0, 0, 0, //884
	0, 0, 0, 0, 0, //889
	0, 0, 0, 0, 0, //894
	0, 0, 0, 0, 0, //899
	0, 0, 0, 0, 0, //904
	0, 0, 0, 0, 0, //909
	0, 0, 0, 0, 0, //914
	0, 0, 0, 0, 0, //919
	0, 0, 0, 0, 0, //924
	0, 0, 0, 0, 0, //929
	0, 0, 0, 0, 0, //934
	0, 0, 0, 0, 0, //939
	0, 0, 0, 0, 0, //944
	0, 0, 0, 0, 0, //949
	0, 0, 0, 0, 0, //954
	0, 0, 0, 0, 0, //959
	0, 0, 0, 0, 0, //964
	0, 0, 0, 0, 0, //969
	0, 0, 0, 0, 0, //974
	0, 0, 0, 0, 0, //979
	0, 0, 0, 0, 0, //984
	0, 0, 0, 0, 0, //989
	0, 0, 0, 0, 0, //994
	0, 0, 0, 0, 0, //999
	0, 0, 0, 0, 0, //1004
	0, 0, 0, 0, 0, //1009
	0, 0, 0, 0, 0, //1014
	0, 0, 0, 0, 0, //1019
	0, 0, 0, SC_FUNC_SW_UW_UW<cellGcmCallback>,    //1024
};

SysCalls::SysCalls(PPUThread& cpu) : CPU(cpu)
{
}

SysCalls::~SysCalls()
{
}

s64 SysCalls::DoSyscall(u32 code)
{
	if(code < 0x400)
	{
		if(sc_table[code]) return sc_table[code]();

		//TODO: remove this
		switch(code)
		{
			//process
			case 2: return lv2ProcessWaitForChild(CPU);
			case 4: return lv2ProcessGetStatus(CPU);
			case 5: return lv2ProcessDetachChild(CPU);
			case 12: return lv2ProcessGetNumberOfObject(CPU);
			case 13: return lv2ProcessGetId(CPU);
			case 18: return lv2ProcessGetPpid(CPU);
			case 19: return lv2ProcessKill(CPU);
			case 23: return lv2ProcessWaitForChild2(CPU);
			case 25: return lv2ProcessGetSdkVersion(CPU);
			//timer
			case 141:
			case 142:
				Sleep(SC_ARGS_1 / (1000 * 1000));
			return 0;

			//tty
			case 988:
				ConLog.Warning("SysCall 988! r3: 0x%llx, r4: 0x%llx, pc: 0x%llx",
					CPU.GPR[3], CPU.GPR[4], CPU.PC);
			return 0;

			case 999:
				dump_enable = !dump_enable;
				ConLog.Warning("Dump %s", dump_enable ? "enabled" : "disabled");
			return 0;
		}
		ConLog.Error("Unknown syscall: %d - %08x", code, code);
		return 0;
	}
	
	s64 ret;
	if(CallFunc(code, ret)) return ret;

	//ConLog.Error("Unknown function 0x%08x", code);
	//return 0;

	//TODO: remove this
	return DoFunc(code);
}