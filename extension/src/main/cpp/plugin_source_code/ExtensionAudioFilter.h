//
// Created by 张涛 on 2020/4/26.
//

#ifndef AGORAMARSVIEWMARKETPLACE_EXTENSIONAUDIOFILTER_H
#define AGORAMARSVIEWMARKETPLACE_EXTENSIONAUDIOFILTER_H

#include "../AgoraRtcKit/NGIAgoraMediaNode.h"
#include "../AgoraRtcKit/AgoraRefCountedObject.h"
#include "../AgoraRtcKit/AgoraRefPtr.h"
#include "AudioTranscriptProvider.h"

namespace agora {
    namespace extension {
        class ExtensionAudioFilter : public agora::rtc::IAudioFilter {
        public:
            ExtensionAudioFilter(const char* name, agora_refptr<AudioTranscriptProvider> transcriptProvider);
            ~ExtensionAudioFilter();
            bool adaptAudioFrame(const media::base::AudioPcmFrame& inAudioPcmFrame,
                                 media::base::AudioPcmFrame& adaptedPcmFrame) override;
            void setEnabled(bool enable) override ;
            bool isEnabled() const override { return enabled_; }
            int setProperty(const char* key, const void* buf, int buf_size) override;
            int getProperty(const char* key, void* buf, int buf_size) const override { return ERR_OK; }
            const char* getName() const override { return filterName_.c_str(); }
        private:
            std::atomic_bool enabled_ = {true};
            std::string filterName_;
            agora_refptr<AudioTranscriptProvider> audioProcessor_;
        protected:
            ExtensionAudioFilter() = default;
        };
    }
}


#endif //AGORAWITHBYTEDANCE_EXTENSIONAUDIOFILTER_H
