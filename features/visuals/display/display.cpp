#include "display.hpp"

Player player;

void Player::Initialize()
{
    this->sessionManager = GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();
}

char* alloc_wcstcs(winrt::hstring source) {
    char* string_alloc = (char*)malloc((source.size() + 1) * sizeof(char));
    wcstombs(string_alloc, source.c_str(), source.size() + 1);
    return string_alloc;
}


void* LastThumb;
concurrency::task<void> Player::Update(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
    this->session = this->sessionManager->GetCurrentSession();

    if (this->session != nullptr)
    {
        this->HasMedia = true;

        this->SourceAppUserModeId = alloc_wcstcs(this->session->SourceAppUserModelId());

        auto info = this->session->TryGetMediaPropertiesAsync().get();
        this->Title = alloc_wcstcs(info.Title());
        this->Artist = alloc_wcstcs(info.Artist());
        this->AlbumArtist = alloc_wcstcs(info.AlbumArtist());
        this->AlbumTitle = alloc_wcstcs(info.AlbumTitle());
        this->TrackNumber = info.TrackNumber();
        this->AlbumTrackCount = info.AlbumTrackCount();

        if (info.Thumbnail())
        {
            auto thumbnail_stream = co_await info.Thumbnail().OpenReadAsync();
            this->Thumbnail_type = alloc_wcstcs(thumbnail_stream.ContentType());
            Buffer buffer = Buffer(thumbnail_stream.Size());
            co_await thumbnail_stream.ReadAsync(buffer, buffer.Capacity(), InputStreamOptions::ReadAhead);

            this->Thumbnail_buffer = malloc(buffer.Length());
            memcpy(this->Thumbnail_buffer, buffer.data(), buffer.Length());
            this->Thumbnail_size = buffer.Length();
            if (this->Thumbnail_buffer != LastThumb)
            {
                D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, this->Thumbnail_buffer, this->Thumbnail_size, 30, 30, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &this->thumb);
                LastThumb = this->Thumbnail_buffer;
            }
        }
        else
        {
            this->Thumbnail_size = 0;
            this->thumb = nullptr;
        }

        auto timelinepropierties = this->session->GetTimelineProperties();

        this->TotalTime = timelinepropierties.EndTime().count();
        this->CurrentTime = timelinepropierties.Position().count();
    }
    else
    {
        this->HasMedia = false;
    }
}