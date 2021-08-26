//
// Created by 张涛 on 2020/4/26.
//

#include "ExtensionAudioFilter.h"
#include "../logutils.h"
#include <string>

namespace agora {
    namespace extension {
        ExtensionAudioFilter::ExtensionAudioFilter(const char* name, agora_refptr<AudioTranscriptProvider> audioProcessor) {
            filterName_ = name;
            audioProcessor_ = audioProcessor;
        }

        ExtensionAudioFilter::~ExtensionAudioFilter() {
            audioProcessor_.reset();
        }

        bool ExtensionAudioFilter::adaptAudioFrame(const media::base::AudioPcmFrame& inAudioPcmFrame,
                                                      media::base::AudioPcmFrame& adaptedPcmFrame) {
            return audioProcessor_->processFrame(inAudioPcmFrame, adaptedPcmFrame) == 0;
        }

        int ExtensionAudioFilter::setProperty(const char* key, const void* buf, int buf_size) {
            std::string key_string(key);
            std::string value_string(static_cast<const char*>(buf), buf_size);

            audioProcessor_->setUserKeys(key_string, value_string);
            return ERR_OK;
        }

        void ExtensionAudioFilter::setEnabled(bool enable) {
            enabled_ = enable;
            if(enable){
                audioProcessor_->startStreaming();
            }
            else{
                audioProcessor_->endStreaming();
            }
        }
    }
}
