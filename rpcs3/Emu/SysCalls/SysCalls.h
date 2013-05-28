#pragma once
#include "ErrorCodes.h"

//#define SYSCALLS_DEBUG

#define declCPU const PPUThread& CPU = GetCurrentPPUThread

//TODO
struct ModuleFunc
{
	u32 id;
	s64 (*func)();

	ModuleFunc(u32 id, s64 (*func)())
		: id(id)
		, func(func)
	{
	}
};

class Module
{
	const char* m_name;
	const u16 m_id;
	bool m_is_loaded;

public:
	Array<ModuleFunc> m_funcs_list;

	Module(const char* name, u16 id);

	void Load();
	void UnLoad();
	bool Load(u32 id);
	bool UnLoad(u32 id);

	void SetLoaded(bool loaded = true)
	{
		m_is_loaded = loaded;
	}

	bool IsLoaded() const
	{
		return m_is_loaded;
	}

	u16 GetID() const
	{
		return m_id;
	}

	wxString GetName() const
	{
		return m_name;
	}

public:
	void Log(const u32 id, wxString fmt, ...)
	{
#ifdef SYSCALLS_DEBUG
		va_list list;
		va_start(list, fmt);
		ConLog.Write(GetName() + wxString::Format("[%d]: ", id) + wxString::FormatV(fmt, list));
		va_end(list);
#endif
	}

	void Log(wxString fmt, ...)
	{
#ifdef SYSCALLS_DEBUG
		va_list list;
		va_start(list, fmt);
		ConLog.Write(GetName() + ": " + wxString::FormatV(fmt, list));
		va_end(list);
#endif
	}

	void Warning(const u32 id, wxString fmt, ...)
	{
//#ifdef SYSCALLS_DEBUG
		va_list list;
		va_start(list, fmt);
		ConLog.Warning(GetName() + wxString::Format("[%d] warning: ", id) + wxString::FormatV(fmt, list));
		va_end(list);
//#endif
	}

	void Warning(wxString fmt, ...)
	{
//#ifdef SYSCALLS_DEBUG
		va_list list;
		va_start(list, fmt);
		ConLog.Warning(GetName() + " warning: " + wxString::FormatV(fmt, list));
		va_end(list);
//#endif
	}

	void Error(const u32 id, wxString fmt, ...)
	{
		va_list list;
		va_start(list, fmt);
		ConLog.Error(GetName() + wxString::Format("[%d] error: ", id) + wxString::FormatV(fmt, list));
		va_end(list);
	}

	void Error(wxString fmt, ...)
	{
		va_list list;
		va_start(list, fmt);
		ConLog.Error(GetName() + " error: " + wxString::FormatV(fmt, list));
		va_end(list);
	}

	bool CheckId(u32 id) const
	{
		return Emu.GetIdManager().CheckID(id) && !Emu.GetIdManager().GetIDData(id).m_name.Cmp(GetName());
	}

	bool CheckId(u32 id, ID& _id) const
	{
		return Emu.GetIdManager().CheckID(id) && !(_id = Emu.GetIdManager().GetIDData(id)).m_name.Cmp(GetName());
	}

	template<typename T> bool CheckId(u32 id, T*& data)
	{
		ID id_data;

		if(!CheckId(id, id_data)) return false;

		data = (T*)id_data.m_data;

		return true;
	}

	u32 GetNewId(void* data = nullptr, u8 flags = 0)
	{
		return Emu.GetIdManager().GetNewID(GetName(), data, flags);
	}

//protected:
	__forceinline void AddFunc(u32 id, s64 (*func)())
	{
		m_funcs_list.Move(new ModuleFunc(id, func));
	}
};

static s64 null_function() { return 0; }

bool IsLoadedFunc(u32 id);
bool CallFunc(u32 id, s64& ret);
bool UnloadFunc(u32 id);
void UnloadModules();
Module* GetModuleByName(const wxString& name);
Module* GetModuleById(u16 id);

extern ArrayF<ModuleFunc> g_modules_funcs_list;
extern ArrayF<Module> g_modules_list;

class SysCallBase //Module
{
private:
	wxString m_module_name;
	//u32 m_id;

public:
	SysCallBase(const wxString& name/*, u32 id*/)
		: m_module_name(name)
		//, m_id(id)
	{
	}

	const wxString& GetName() const { return m_module_name; }

	void Log(const u32 id, wxString fmt, ...)
	{
#ifdef SYSCALLS_DEBUG
		va_list list;
		va_start(list, fmt);
		ConLog.Write(GetName() + wxString::Format("[%d]: ", id) + wxString::FormatV(fmt, list));
		va_end(list);
#endif
	}

	void Log(wxString fmt, ...)
	{
#ifdef SYSCALLS_DEBUG
		va_list list;
		va_start(list, fmt);
		ConLog.Write(GetName() + ": " + wxString::FormatV(fmt, list));
		va_end(list);
#endif
	}

	void Warning(const u32 id, wxString fmt, ...)
	{
//#ifdef SYSCALLS_DEBUG
		va_list list;
		va_start(list, fmt);
		ConLog.Warning(GetName() + wxString::Format("[%d] warning: ", id) + wxString::FormatV(fmt, list));
		va_end(list);
//#endif
	}

	void Warning(wxString fmt, ...)
	{
//#ifdef SYSCALLS_DEBUG
		va_list list;
		va_start(list, fmt);
		ConLog.Warning(GetName() + " warning: " + wxString::FormatV(fmt, list));
		va_end(list);
//#endif
	}

	void Error(const u32 id, wxString fmt, ...)
	{
		va_list list;
		va_start(list, fmt);
		ConLog.Error(GetName() + wxString::Format("[%d] error: ", id) + wxString::FormatV(fmt, list));
		va_end(list);
	}

	void Error(wxString fmt, ...)
	{
		va_list list;
		va_start(list, fmt);
		ConLog.Error(GetName() + " error: " + wxString::FormatV(fmt, list));
		va_end(list);
	}

	bool CheckId(u32 id) const
	{
		return Emu.GetIdManager().CheckID(id) && !Emu.GetIdManager().GetIDData(id).m_name.Cmp(GetName());
	}

	bool CheckId(u32 id, ID& _id) const
	{
		return Emu.GetIdManager().CheckID(id) && !(_id = Emu.GetIdManager().GetIDData(id)).m_name.Cmp(GetName());
	}

	template<typename T> bool CheckId(u32 id, T*& data)
	{
		ID id_data;

		if(!CheckId(id, id_data)) return false;

		data = (T*)id_data.m_data;

		return true;
	}

	u32 GetNewId(void* data = nullptr, u8 flags = 0)
	{
		return Emu.GetIdManager().GetNewID(GetName(), data, flags);
	}
};

/*
static bool CmpPath(const wxString& path1, const wxString& path2)
{
	return path1.Len() >= path2.Len() && path1(0, path2.Len()).CmpNoCase(path2) == 0;
}

static wxString GetWinPath(const wxString& path)
{
	if(!CmpPath(path, "/") && CmpPath(path(1, 1), ":")) return path;

	wxString ppath = wxFileName(Emu.m_path).GetPath() + '/' + wxFileName(path).GetFullName();
	if(wxFileExists(ppath)) return ppath;

	if (CmpPath(path, "/dev_hdd0/") ||
		CmpPath(path, "/dev_hdd1/") ||
		CmpPath(path, "/dev_bdvd/") ||
		CmpPath(path, "/dev_usb001/") ||
		CmpPath(path, "/ps3_home/") ||
		CmpPath(path, "/app_home/") ||
		CmpPath(path, "/dev_flash/") ||
		CmpPath(path, "/dev_flash2/") ||
		CmpPath(path, "/dev_flash3/")
		) return wxGetCwd() + path;

	return wxFileName(Emu.m_path).GetPath() + (path[0] == '/' ? path : "/" + path);
}
*/

//process
extern int sys_process_getpid();
extern int sys_process_exit(int errorcode);
extern int sys_game_process_exitspawn(u32 path_addr, u32 argv_addr, u32 envp_addr,
								u32 data, u32 data_size, int prio, u64 flags );

//sys_semaphore
extern int sys_semaphore_create(u32 sem_addr, u32 attr_addr, int initial_val, int max_val);
extern int sys_semaphore_destroy(u32 sem);
extern int sys_semaphore_wait(u32 sem, u64 timeout);
extern int sys_semaphore_trywait(u32 sem);
extern int sys_semaphore_post(u32 sem, int count);

//sys_lwmutex
extern int sys_lwmutex_create(u64 lwmutex_addr, u64 lwmutex_attr_addr);
extern int sys_lwmutex_destroy(u64 lwmutex_addr);
extern int sys_lwmutex_lock(u64 lwmutex_addr, u64 timeout);
extern int sys_lwmutex_trylock(u64 lwmutex_addr);
extern int sys_lwmutex_unlock(u64 lwmutex_addr);

//sys_cond
extern int sys_cond_create(u32 cond_addr, u32 mutex_id, u32 attr_addr);
extern int sys_cond_destroy(u32 cond_id);
extern int sys_cond_wait(u32 cond_id, u64 timeout);
extern int sys_cond_signal(u32 cond_id);
extern int sys_cond_signal_all(u32 cond_id);

//sys_mutex
extern int sys_mutex_create(u32 mutex_id_addr, u32 attr_addr);
extern int sys_mutex_destroy(u32 mutex_id);
extern int sys_mutex_lock(u32 mutex_id, u64 timeout);
extern int sys_mutex_trylock(u32 mutex_id);
extern int sys_mutex_unlock(u32 mutex_id);

//ppu_thread
extern int sys_ppu_thread_exit(int errorcode);
extern int sys_ppu_thread_yield();
extern int sys_ppu_thread_join(u32 thread_id, u32 vptr_addr);
extern int sys_ppu_thread_detach(u32 thread_id);
extern void sys_ppu_thread_get_join_state(u32 isjoinable_addr);
extern int sys_ppu_thread_set_priority(u32 thread_id, int prio);
extern int sys_ppu_thread_get_priority(u32 thread_id, u32 prio_addr);
extern int sys_ppu_thread_get_stack_information(u32 info_addr);
extern int sys_ppu_thread_stop(u32 thread_id);
extern int sys_ppu_thread_restart(u32 thread_id);
extern int sys_ppu_thread_create(u32 thread_id_addr, u32 entry, u32 arg, int prio, u32 stacksize, u64 flags, u32 threadname_addr);
extern int sys_ppu_thread_get_id();

//memory
extern int sys_memory_container_create(u32 cid_addr, u32 yield_size);
extern int sys_memory_container_destroy(u32 cid);
extern int sys_memory_allocate(u32 size, u32 flags, u32 alloc_addr_addr);
extern int sys_memory_free(u32 start_addr);
extern int sys_memory_get_user_memory_size(u32 mem_info_addr);
extern int sys_mmapper_allocate_address(u32 size, u64 flags, u32 alignment, u32 alloc_addr);
extern int sys_mmapper_allocate_memory(u32 size, u64 flags, u32 mem_id_addr);
extern int sys_mmapper_map_memory(u32 start_addr, u32 mem_id, u64 flags);

//cellFs
extern int cellFsOpen(u32 path_addr, int flags, u32 fd_addr, u32 arg_addr, u64 size);
extern int cellFsRead(u32 fd, u32 buf_addr, u64 nbytes, u32 nread_addr);
extern int cellFsWrite(u32 fd, u32 buf_addr, u64 nbytes, u32 nwrite_addr);
extern int cellFsClose(u32 fd);
extern int cellFsOpendir(u32 path_addr, u32 fd_addr);
extern int cellFsReaddir(u32 fd, u32 dir_addr, u32 nread_addr);
extern int cellFsClosedir(u32 fd);
extern int cellFsStat(u32 path_addr, u32 sb_addr);
extern int cellFsFstat(u32 fd, u32 sb_addr);
extern int cellFsMkdir(u32 path_addr, u32 mode);
extern int cellFsRename(u32 from_addr, u32 to_addr);
extern int cellFsRmdir(u32 path_addr);
extern int cellFsUnlink(u32 path_addr);
extern int cellFsLseek(u32 fd, s64 offset, u32 whence, u32 pos_addr);

//cellVideo
extern int cellVideoOutGetState(u32 videoOut, u32 deviceIndex, u32 state_addr);
extern int cellVideoOutGetResolution(u32 resolutionId, u32 resolution_addr);
extern int cellVideoOutConfigure(u32 videoOut, u32 config_addr, u32 option_addr, u32 waitForEvent);
extern int cellVideoOutGetConfiguration(u32 videoOut, u32 config_addr, u32 option_addr);
extern int cellVideoOutGetNumberOfDevice(u32 videoOut);
extern int cellVideoOutGetResolutionAvailability(u32 videoOut, u32 resolutionId, u32 aspect, u32 option);

//cellSysutil
extern int cellSysutilCheckCallback();
extern int cellSysutilRegisterCallback(int slot, u64 func_addr, u64 userdata);
extern int cellSysutilUnregisterCallback(int slot);

//cellMsgDialog
extern int cellMsgDialogOpen2(u32 type, u32 msgString_addr, u32 callback_addr, u32 userData, u32 extParam);

//cellPad
extern int cellPadInit(u32 max_connect);
extern int cellPadEnd();
extern int cellPadClearBuf(u32 port_no);
extern int cellPadGetData(u32 port_no, u32 data_addr);
extern int cellPadGetDataExtra(u32 port_no, u32 device_type_addr, u32 data_addr);
extern int cellPadSetActDirect(u32 port_no, u32 param_addr);
extern int cellPadGetInfo2(u32 info_addr);
extern int cellPadSetPortSetting(u32 port_no, u32 port_setting);

//cellGcm
extern int cellGcmInit(u32 context_addr, u32 cmdSize, u32 ioSize, u32 ioAddress);
extern int cellGcmMapMainMemory(u32 address, u32 size, u32 offset_addr);
extern int cellGcmCallback(u32 context_addr, u32 count);
extern int cellGcmGetConfiguration(u32 config_addr);
extern int cellGcmAddressToOffset(u32 address, u32 offset_addr);
extern int cellGcmSetDisplayBuffer(u32 id, u32 offset, u32 pitch, u32 width, u32 height);
extern u32 cellGcmGetLabelAddress(u32 index);
extern u32 cellGcmGetControlRegister();
extern int cellGcmFlush(u32 ctx, u32 id);
extern void cellGcmSetTile(u32 index, u32 location, u32 offset, u32 size, u32 pitch, u32 comp, u32 base, u32 bank);
extern int cellGcmBindTile(u32 index);
extern int cellGcmBindZcull(u32 index, u32 offset, u32 width, u32 height, u32 cullStart, u32 zFormat, u32 aaFormat, u32 zCullDir, u32 zCullFormat, u32 sFunc, u32 sRef, u32 sMask);
extern int cellGcmGetFlipStatus();
extern int cellGcmResetFlipStatus();
extern u32 cellGcmGetTiledPitchSize(u32 size);
extern int cellGcmSetFlipMode(u32 mode);
extern u32 cellGcmGetDefaultCommandWordSize();
extern u32 cellGcmGetDefaultSegmentWordSize();
extern int cellGcmSetDefaultFifoSize(u32 bufferSize, u32 segmentSize);

//sys_tty
extern int sys_tty_read(u32 ch, u64 buf_addr, u32 len, u64 preadlen_addr);
extern int sys_tty_write(u32 ch, u64 buf_addr, u32 len, u64 pwritelen_addr);

//cellResc
extern int cellRescSetSrc(const int idx, const u32 src_addr);
extern int cellRescSetBufferAddress(const u32 colorBuffers_addr, const u32 vertexArray_addr, const u32 fragmentShader_addr);

//sys_heap
extern int sys_heap_create_heap(const u32 heap_addr, const u32 start_addr, const u32 size);
extern int sys_heap_malloc(const u32 heap_addr, const u32 size);

//sys_spu
extern int sys_spu_thread_group_create(u64 id_addr, u32 num, int prio, u64 attr_addr);
extern int sys_spu_thread_create(u64 thread_id_addr, u64 entry_addr, u64 arg, int prio, u32 stacksize, u64 flags, u64 threadname_addr);
extern int sys_raw_spu_create(u32 id_addr, u32 attr_addr);
extern int sys_spu_initialize(u32 max_usable_spu, u32 max_raw_spu);

//sys_time
extern int sys_time_get_current_time(u32 sec_addr, u32 nsec_addr);
extern s64 sys_time_get_system_time();
extern u64 sys_time_get_timebase_frequency();

#define SC_ARGS_1			CPU.GPR[3]
#define SC_ARGS_2 SC_ARGS_1,CPU.GPR[4]
#define SC_ARGS_3 SC_ARGS_2,CPU.GPR[5]
#define SC_ARGS_4 SC_ARGS_3,CPU.GPR[6]
#define SC_ARGS_5 SC_ARGS_4,CPU.GPR[7]
#define SC_ARGS_6 SC_ARGS_5,CPU.GPR[8]
#define SC_ARGS_7 SC_ARGS_6,CPU.GPR[9]
#define SC_ARGS_8 SC_ARGS_7,CPU.GPR[10]
#define SC_ARGS_9 SC_ARGS_8,CPU.GPR[11]
#define SC_ARGS_10 SC_ARGS_9,CPU.GPR[12]
#define SC_ARGS_11 SC_ARGS_10,CPU.GPR[13]
#define SC_ARGS_12 SC_ARGS_11,CPU.GPR[14]

typedef s64(*SC_FUNC)();

extern bool dump_enable;
class PPUThread;

class SysCalls
{
	PPUThread& CPU;

public:
	//process
	int lv2ProcessGetPid(PPUThread& CPU);
	int lv2ProcessWaitForChild(PPUThread& CPU);
	int lv2ProcessGetStatus(PPUThread& CPU);
	int lv2ProcessDetachChild(PPUThread& CPU);
	int lv2ProcessGetNumberOfObject(PPUThread& CPU);
	int lv2ProcessGetId(PPUThread& CPU);
	int lv2ProcessGetPpid(PPUThread& CPU);
	int lv2ProcessKill(PPUThread& CPU);
	int lv2ProcessWaitForChild2(PPUThread& CPU);
	int lv2ProcessGetSdkVersion(PPUThread& CPU);

protected:
	SysCalls(PPUThread& cpu);
	~SysCalls();

public:
	s64 DoSyscall(u32 code);
	s64 DoFunc(const u32 id);
};

//extern SysCalls SysCallsManager;