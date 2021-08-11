//
// Created by 张涛 on 2020/4/26.
//

#ifndef AGORAMARSVIEWMARKETPLACE_EXTENSION_PROVIDER_H
#define AGORAMARSVIEWMARKETPLACE_EXTENSION_PROVIDER_H

#include "../AgoraRtcKit/NGIAgoraExtensionProvider.h"
#include "ExtensionAudioFilter.h"

namespace agora {
    namespace extension {
        static const char* AUDIO_FILTER_NAME = "TranscriptProvider";

        class ExtensionProvider : public agora::rtc::IExtensionProvider {
        private:
            agora_refptr<AudioTranscriptProvider> audioProcessor_;
        public:
            ExtensionProvider();
            ~ExtensionProvider();

            void setExtensionControl(rtc::IExtensionControl* control) override;
            void enumerateExtensions(ExtensionMetaInfo* extension_list, int& extension_count) override;
            agora_refptr<rtc::IAudioFilter> createAudioFilter(const char* name) override;

        };
    }
}
#endif /AGORAMARSVIEWMARKETPLACE_EXTENSION_PROVIDER_H
