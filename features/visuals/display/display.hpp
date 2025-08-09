#pragma once

#pragma comment(lib, "windowsapp")

#include <winrt/base.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.Control.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.Imaging.h>

#include <chrono>
#include <functional>
#include <sstream>

#include <ppltasks.h>
#include <pplawait.h>

#include <sdkddkver.h>
#include <d3dx9tex.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using namespace winrt;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Media::Control;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation::Collections;
using namespace winrt::Windows::Media::Control;


class Player
{
private:
    std::optional<GlobalSystemMediaTransportControlsSessionManager> sessionManager;
    std::optional<IRandomAccessStreamWithContentType> thumbnail;
public:
    std::optional<GlobalSystemMediaTransportControlsSession> session;
    bool HasMedia = false;
    char* SourceAppUserModeId;
    char* Title;
    char* Artist;
    char* AlbumArtist;
    char* AlbumTitle;
    int TrackNumber;
    int AlbumTrackCount;
    char* Thumbnail_type;
    void* Thumbnail_buffer;
    int Thumbnail_size;
    bool hasthumnnail = false;

    int TotalTime;
    int CurrentTime;

    LPDIRECT3DTEXTURE9 thumb;

    void Initialize();
    concurrency::task<void> Update(LPDIRECT3DDEVICE9 g_pd3dDevice);
};

extern Player player;