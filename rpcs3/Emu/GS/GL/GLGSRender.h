#pragma once
#include "Emu/GS/GSRender.h"
#include "wx/glcanvas.h"
#include "GLBuffers.h"
#include "Program.h"
#include "OpenGL.h"
#include "ProgramBuffer.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gl.lib")

void checkForGlError(const char* situation);

class GLTexture
{
	u32 m_width, m_height;
	u32 m_id;
	u32 m_offset;
	bool m_enabled;

	bool m_cubemap;
	u8 m_dimension;
	u32 m_format;
	u16 m_mipmap;
	
public:
	GLTexture()
		: m_width(0), m_height(0),
		m_id(0),
		m_offset(0),
		m_enabled(false),

		m_cubemap(false),
		m_dimension(0),
		m_format(0),
		m_mipmap(0)
	{
	}

	void SetRect(const u32 width, const u32 height)
	{
		m_width = width;
		m_height = height;
	}

	u32 GetOffset() const { return m_offset; }

	void SetFormat(const bool cubemap, const u8 dimension, const u32 format, const u16 mipmap)
	{
		m_cubemap = cubemap;
		m_dimension = dimension;
		m_format = format;
		m_mipmap = mipmap;
	}

	u32 GetFormat() const { return m_format; }

	void SetOffset(const u32 offset)
	{
		m_offset = offset;
	}

	wxSize GetRect() const
	{
		return wxSize(m_width, m_height);
	}

	void Init()
	{
		if(!m_id)
		{
			glGenTextures(1, &m_id);
			checkForGlError("GLTexture::Init() -> glGenTextures");
			glBindTexture(GL_TEXTURE_2D, m_id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, m_id);
		}

		//TODO: safe init
		checkForGlError("GLTexture::Init() -> glBindTexture");

		switch(m_format & ~(0x20 | 0x40))
		{
		case 0x81:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, Memory.GetMemFromAddr(m_offset));
			checkForGlError("GLTexture::Init() -> glTexImage2D");
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);
			checkForGlError("GLTexture::Init() -> glTexParameteri");
		break;

		case 0x85:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, Memory.GetMemFromAddr(m_offset));
			checkForGlError("GLTexture::Init() -> glTexImage2D");
		break;

		default: ConLog.Error("Init tex error: Bad tex format (0x%x)", m_format); break;
		}

		//glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Save(const wxString& name)
	{
		if(!m_id || !m_offset) return;

		ConLog.Write("start");
		u32* alldata = new u32[m_width * m_height];

		glBindTexture(GL_TEXTURE_2D, m_id);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, alldata);

		u8* data = new u8[m_width * m_height * 3];
		u8* alpha = new u8[m_width * m_height];

		u8* src = (u8*)alldata;
		u8* dst_d = data;
		u8* dst_a = alpha;
		for(u32 i=0; i<m_width*m_height;i++)
		{
			*dst_d++ = *src++;
			*dst_d++ = *src++;
			*dst_d++ = *src++;
			*dst_a++ = *src++;
		}

		ConLog.Write("end");
		wxImage out;
		out.Create(m_width, m_height, data, alpha);
		out.SaveFile(name, wxBITMAP_TYPE_PNG);

		free(alldata);
		//free(data);
		//free(alpha);
	}

	void Save()
	{
		static const wxString& dir_path = "textures";
		static const wxString& file_fmt = dir_path + "\\" + "tex[%d].png";

		if(!wxDirExists(dir_path)) wxMkDir(dir_path);

		u32 count = 0;
		while(wxFileExists(wxString::Format(file_fmt, count))) count++;
		Save(wxString::Format(file_fmt, count));
	}

	void Bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void Enable(bool enable) { m_enabled = enable; }
	bool IsEnabled() const { return m_enabled; }
};

struct IndexArrayData
{
	Array<u8> m_data;
	int m_type;
	u32 m_first;
	u32 m_count;
	u32 m_addr;
	u32 index_max;
	u32 index_min;

	IndexArrayData()
	{
		Reset();
	}

	void Reset()
	{
		m_type = 0;
		m_first = ~0;
		m_count = 0;
		m_addr = 0;
		index_min = ~0;
		index_max = 0;
		m_data.Clear();
	}
};

struct GLGSFrame : public GSFrame
{
	wxGLCanvas* canvas;
	GLTexture m_textures[16];
	u32 m_frames;

	GLGSFrame();
	~GLGSFrame() {}

	void Flip();

	wxGLCanvas* GetCanvas() const { return canvas; }
	GLTexture& GetTexture(const u32 index) { return m_textures[index]; }

	virtual void SetViewport(int x, int y, u32 w, u32 h);

private:
	virtual void OnSize(wxSizeEvent& event);
};

extern gcmBuffer gcmBuffers[2];

struct GLRSXThread : public ThreadBase
{
	wxWindow* m_parent;
	Stack<u32> call_stack;

	GLRSXThread(wxWindow* parent);

	virtual void Task();
};

class GLGSRender
	: public wxWindow
	, public GSRender
{
private:
	GLRSXThread* m_rsx_thread;

	IndexArrayData m_indexed_array;

	ShaderProgram m_shader_prog;
	VertexData m_vertex_data[16];
	Array<u8> m_vdata;
	VertexProgram m_vertex_progs[16];
	VertexProgram* m_cur_vertex_prog;
	Program m_program;
	int m_fp_buf_num;
	int m_vp_buf_num;
	ProgramBuffer m_prog_buffer;

	GLvao m_vao;
	GLvbo m_vbo;

public:
	GLGSFrame* m_frame;
	u32 m_draw_frames;
	u32 m_skip_frames;

	GLGSRender();
	~GLGSRender();

private:
	void EnableVertexData(bool indexed_draw=false);
	void DisableVertexData();
	void LoadVertexData(u32 first, u32 count);
	void InitVertexData();

	void Enable(bool enable, const u32 cap);
	virtual void Init(const u32 ioAddress, const u32 ioSize, const u32 ctrlAddress, const u32 localAddress);
	virtual void Draw();
	virtual void Close();

public:
	void DoCmd(const u32 fcmd, const u32 cmd, mem32_t& args, const u32 count);
	void CloseOpenGL();
};