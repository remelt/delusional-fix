#define _CRT_SECURE_NO_WARNINGS
#include "mplayer.h"

mPlayer mplayer;

char* alloc_wcstcs(winrt::hstring source)
{
	char* string_alloc = (char*)malloc((source.size() + 1) * sizeof(char));
	wcstombs(string_alloc, source.c_str(), source.size() + 1);
	return string_alloc;
}

//TODO: UNDERSTAND WHY SOME SYMBOLS ARENT DECODING PROPERLY

std::string wstring_to_utf8(const std::wstring& wstr)
{
	if (wstr.empty()) {
		return std::string();
	}

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);

	std::string strTo(size_needed, 0);

	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);

	return strTo;
}
void* LastThumb;
std::string PreviousTitle;
std::string PreviousArtist;

concurrency::task< void > mPlayer::Update(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	pool_.clear();

	static auto sessions = GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();
	auto currentSession = sessions.GetCurrentSession();
	mplayer.session = currentSession;

	if (this->session != nullptr) {
		this->HasMedia = true;
		this->SourceAppUserModeId = pool_.allocate(this->session->SourceAppUserModelId());

		auto info = this->session->TryGetMediaPropertiesAsync().get();

		std::string currentTitle = wstring_to_utf8(info.Title().c_str());
		std::string currentArtist = wstring_to_utf8(info.Artist().c_str());
		bool trackChanged = (currentTitle != PreviousTitle) || (currentArtist != PreviousArtist);

		this->Title = wstring_to_utf8(info.Title().c_str());
		this->Artist = wstring_to_utf8(info.Artist().c_str());
		this->AlbumArtist = pool_.allocate(info.AlbumArtist());
		this->AlbumTitle = pool_.allocate(info.AlbumTitle());
		this->TrackNumber = info.TrackNumber();
		this->AlbumTrackCount = info.AlbumTrackCount();

		if (info.Thumbnail() && trackChanged) {
			auto thumbnail_stream = info.Thumbnail().OpenReadAsync().get();
			this->Thumbnail_type = pool_.allocate(thumbnail_stream.ContentType());

			Buffer buffer = Buffer(thumbnail_stream.Size());
			thumbnail_stream.ReadAsync(buffer, buffer.Capacity(), InputStreamOptions::ReadAhead).get();

			if (this->Thumbnail_buffer != LastThumb) {
				free(this->Thumbnail_buffer);
				//this->thumb->Release();
				//this->thumb = nullptr;
			}

			this->Thumbnail_buffer = malloc(buffer.Length());
			memcpy(this->Thumbnail_buffer, buffer.data(), buffer.Length());
			this->Thumbnail_size = buffer.Length();
			if (this->Thumbnail_buffer != LastThumb) {
				D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, this->Thumbnail_buffer, this->Thumbnail_size, 30, 30, D3DX_DEFAULT, 1,
					D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &this->thumb);
				LastThumb = this->Thumbnail_buffer;
			}
			PreviousTitle = currentTitle;
			PreviousArtist = currentArtist;
		}
		else if (!info.Thumbnail()) {
			this->Thumbnail_size = 0;
			this->thumb = nullptr;
		}

		auto timelineProperties = this->session->GetTimelineProperties();
		auto duration = timelineProperties.EndTime() - timelineProperties.StartTime();
		this->TotalTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

		auto currentPos = timelineProperties.Position() - timelineProperties.StartTime();
		this->CurrentTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentPos).count();

		auto playbackInfo = this->session->GetPlaybackInfo();
		if (playbackInfo) {
			auto playbackStatus = playbackInfo.PlaybackStatus();
			this->isPlaying = (playbackStatus == GlobalSystemMediaTransportControlsSessionPlaybackStatus::Playing);
		}
	}
	else {
		this->HasMedia = false;
	}
	co_return;
}