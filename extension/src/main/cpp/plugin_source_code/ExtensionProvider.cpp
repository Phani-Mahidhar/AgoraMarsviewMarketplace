//
// Created by 张涛 on 2020/4/26.
//

#include "ExtensionProvider.h"
#include "../logutils.h"
#include "JniHelper.h"

namespace agora {
    namespace extension {
        ExtensionProvider::ExtensionProvider() {
            PRINTF_INFO("ExtensionProvider create");
            audioProcessor_ = new agora::RefCountedObject<AudioTranscriptProvider>();
        }

        ExtensionProvider::~ExtensionProvider() {
            PRINTF_INFO("ExtensionProvider destroy");
            audioProcessor_.reset();
        }

        // Provide information about all plug-ins that support packaging.
        // When the SDK loads the plug-in, it will call this method to send a callback to the plug-in.
        // You need to provide information about all plug-ins that support encapsulation.
        void ExtensionProvider::enumerateExtensions(ExtensionMetaInfo* extension_list,
                                                           int& extension_count) {
            extension_count = 1;

            //Declare an Audio Filter, and IExtensionProvider::createAudioFilter will be called
            ExtensionMetaInfo i;
            i.type = EXTENSION_TYPE::AUDIO_FILTER;
            i.extension_name = agora::extension::AUDIO_FILTER_NAME;
            extension_list[0] = i;
        }


        agora_refptr<agora::rtc::IAudioFilter> ExtensionProvider::createAudioFilter(const char* name) {
            PRINTF_INFO("ExtensionProvider::createAudioFilter %s", name);
            auto audioFilter = new agora::RefCountedObject<agora::extension::ExtensionAudioFilter>(name, audioProcessor_);
            return audioFilter;
        }
        void ExtensionProvider::setExtensionControl(rtc::IExtensionControl* control){
            audioProcessor_->setExtensionControl((control));
        }
    }
}
