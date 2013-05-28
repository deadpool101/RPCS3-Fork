#include "stdafx.h"
#include "GameViewer.h"
#include "Loader/PSF.h"

static const wxString m_class_name = "GameViewer";
GameViewer::GameViewer(wxWindow* parent) : wxListView(parent)
{
	LoadSettings();
	m_columns.Show(this);

	m_path = wxGetCwd(); //TODO

	Connect(GetId(), wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(GameViewer::DClick));

	Refresh();
}

GameViewer::~GameViewer()
{
	SaveSettings();
}

void GameViewer::DoResize(wxSize size)
{
	SetSize(size);
}

void GameViewer::LoadGames()
{
	if(!wxDirExists(m_path)) return;

	m_games.Clear();
	wxDir dir(m_path);
	if(!dir.HasSubDirs()) return;

	wxString buf;
	if(!dir.GetFirst(&buf)) return;
	if(wxDirExists(buf)) m_games.Add(buf);

	for(;;)
	{
		if(!dir.GetNext(&buf)) break;
		if(wxDirExists(buf)) m_games.Add(buf);
	}

	//ConLog.Write("path: %s", m_path);
	//ConLog.Write("folders count: %d", m_games.GetCount());
}

void GameViewer::LoadPSF()
{
	m_game_data.Clear();
	for(uint i=0; i<m_games.GetCount(); ++i)
	{
		const wxString& path = m_games[i] + "\\" + "PARAM.SFO";
		if(!wxFileExists(path)) continue;
		vfsLocalFile f(path);
		PSFLoader psf(f);
		if(!psf.Load(false)) continue;
		psf.m_info.root = m_games[i];
		m_game_data.Add(new GameInfo(psf.m_info));
	}

	m_columns.Update(m_game_data);
}

void GameViewer::ShowData()
{
	m_columns.ShowData(this);
}

void GameViewer::Refresh()
{
	LoadGames();
	LoadPSF();
	ShowData();
}

void GameViewer::SaveSettings()
{
	m_columns.LoadSave(false, m_class_name, this);
}

void GameViewer::LoadSettings()
{
	m_columns.LoadSave(true, m_class_name);
}

void GameViewer::DClick(wxListEvent& event)
{
	long i = GetFirstSelected();
	if(i < 0) return;

	const wxString& path = m_path + "\\" + m_game_data[i].root + "\\" + "USRDIR" + "\\" + "BOOT.BIN";
	if(!wxFileExists(path))
	{
		ConLog.Error("Boot error: elf not found! [%s]", path);
		return;
	}

	Emu.Stop();
	Emu.SetElf(path);
	Emu.Run();
}