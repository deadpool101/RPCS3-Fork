#pragma once

#include "Emu/Io/PadHandler.h"

class NullPadHandler : public PadHandlerBase
{
public:
	NullPadHandler()
	{
	}

	virtual void Init(const u32 max_connect)
	{
		memset(&m_info, 0, sizeof(PadInfo));
		m_info.max_connect = max_connect;
		m_pads.Clear();
	}

	virtual void Close()
	{
		memset(&m_info, 0, sizeof(PadInfo));
		m_pads.Clear();
	}
};