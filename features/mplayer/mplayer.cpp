#define _CRT_SECURE_NO_WARNINGS
#include "mplayer.h"
#include "../../sdk/structs/crc32/crc32.h"

mPlayer mplayer;

char* alloc_wcstcs(winrt::hstring source)
{
	char* string_alloc = (char*)malloc((source.size() + 1) * sizeof(char));
	wcstombs(string_alloc, source.c_str(), source.size() + 1);
	return string_alloc;
}

std::string wstring_to_utf8(std::wstring_view wstr) {
	if (wstr.empty())
		return {};

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
	std::string result(size_needed, 0);

	WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), result.data(), size_needed, nullptr, nullptr);

	return result;
}

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

		this->Title = wstring_to_utf8(info.Title().c_str());
		this->Artist = wstring_to_utf8(info.Artist().c_str());
		this->AlbumArtist = pool_.allocate(info.AlbumArtist());
		this->AlbumTitle = pool_.allocate(info.AlbumTitle());
		this->TrackNumber = info.TrackNumber();
		this->AlbumTrackCount = info.AlbumTrackCount();

		if (info.Thumbnail()) {
			auto thumbnail_stream = info.Thumbnail().OpenReadAsync().get();
			this->Thumbnail_type = pool_.allocate(thumbnail_stream.ContentType());

			Buffer buffer = Buffer(thumbnail_stream.Size());
			thumbnail_stream.ReadAsync(buffer, buffer.Capacity(), InputStreamOptions::ReadAhead).get();

			unsigned int new_hash = crc32::process_single_buffer(buffer.data(), static_cast<int>(buffer.Length()));
			bool data_changed = false;
			if (this->Thumbnail_buffer == nullptr || this->Thumbnail_size != buffer.Length() || new_hash != this->Thumbnail_hash) {
				data_changed = true;
			}

			if (data_changed) {
				free(this->Thumbnail_buffer);
				if (this->thumb) {
					this->thumb->Release();
					this->thumb = nullptr;
				}

				this->Thumbnail_buffer = malloc(buffer.Length());
				memcpy(this->Thumbnail_buffer, buffer.data(), buffer.Length());
				this->Thumbnail_size = buffer.Length();
				this->Thumbnail_hash = new_hash;

				D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, this->Thumbnail_buffer, this->Thumbnail_size, 30, 30, D3DX_DEFAULT, 1,
					D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &this->thumb);
			}
		}
		else {
			free(this->Thumbnail_buffer);
			this->Thumbnail_buffer = nullptr;
			this->Thumbnail_size = 0;
			this->Thumbnail_hash = 0;
			if (this->thumb) {
				this->thumb->Release();
				this->thumb = nullptr;
			}
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