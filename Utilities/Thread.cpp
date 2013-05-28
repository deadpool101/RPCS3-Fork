#include "stdafx.h"
#include "Thread.h"

ThreadBase* GetCurrentNamedThread()
{
	ThreadExec* thr = (ThreadExec*)::wxThread::This();
	return thr ? thr->m_parent : nullptr;
}

ThreadBase::ThreadBase(bool detached, const wxString& name)
	: m_detached(detached)
	, m_name(name)
	, m_executor(nullptr)
{
}

void ThreadBase::Start()
{
	if(m_executor) return;

	m_executor = new ThreadExec(m_detached, this);
}

void ThreadBase::Stop(bool wait)
{
	if(!m_executor) return;
	ThreadExec* exec = m_executor;
	m_executor = nullptr;
	exec->Stop(wait);
}

bool ThreadBase::IsAlive() const
{
	return m_executor != nullptr;
}

bool ThreadBase::TestDestroy() const
{
	if(!m_executor || !m_executor->m_parent) return true;

	return m_executor->TestDestroy();
}

wxString ThreadBase::GetThreadName() const
{
	return m_name;
}

void ThreadBase::SetThreadName(const wxString& name)
{
	m_name = name;
}