#pragma once

#pragma comment( lib, "windowsapp" )

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.Control.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/base.h>

#include <chrono>
#include <functional>
#include <sstream>

#include <pplawait.h>
#include <ppltasks.h>

#include <d3d9.h>
#include <../includes/june2010/Include/d3dx9tex.h>
#include <sdkddkver.h>
#include "optimization.h"
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

using namespace winrt;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Media::Control;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation::Collections;
using namespace winrt::Windows::Media::Control;

class mPlayer
{
private:
	StringPool pool_;

	char* SourceAppUserModeId{ nullptr };
	char* AlbumArtist{ nullptr };
	char* AlbumTitle{ nullptr };
	char* Thumbnail_type{ nullptr };

public:
	std::optional< GlobalSystemMediaTransportControlsSessionManager > sessionManager;
	std::optional< IRandomAccessStreamWithContentType > thumbnail;
	std::optional< GlobalSystemMediaTransportControlsSession > session;
	bool HasMedia = false;
	std::string Title;
	std::string Artist;
	int TrackNumber;
	int AlbumTrackCount;
	void* Thumbnail_buffer;
	int Thumbnail_size;
	bool hasthumnnail = false;
	bool isPlaying;
	int TotalTime;
	int CurrentTime;

	LPDIRECT3DTEXTURE9 thumb;

	concurrency::task< void > Update(LPDIRECT3DDEVICE9 g_pd3dDevice);
};

extern mPlayer mplayer;