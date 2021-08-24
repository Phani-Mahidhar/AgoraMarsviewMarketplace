//
// Created by kphan on 7/30/2021.
//

#ifndef AGORAMARSVIEWMARKETPLACE_AUDIOTRANSCRIPTPROVIDER_H
#define AGORAMARSVIEWMARKETPLACE_AUDIOTRANSCRIPTPROVIDER_H

#include <string>
#include <list>

#include "../AgoraRtcKit/NGIAgoraExtensionControl.h"

#include "../AgoraRtcKit/AgoraRefPtr.h"
#include "../AgoraRtcKit/NGIAgoraMediaNode.h"
#include "../AgoraRtcKit/AgoraMediaBase.h"
#include "../socket.io-client-cpp/src/sio_client.h"
#include "AudioStreamHelper.h"
#include "../logutils.h"


namespace agora {
    namespace extension {
        class AudioTranscriptProvider  : public RefCountInterface {
        public:
            AudioTranscriptProvider();
            ~AudioTranscriptProvider();
            int processFrame(const agora::media::base::AudioPcmFrame &audioPcmFrame,
                             media::base::AudioPcmFrame& adaptedPcmFrame);

            void dataCallback(const char* data){};

            int setExtensionControl(rtc::IExtensionControl* control){
                control_ = control;
                return 0;
            };
            void setUserKeys(std::string key, std::string value);
            void startStreaming();
            void endStreaming();

        private:
            std::string API_KEY = "null";
            std::string SECRET_KEY = "null";
            std::string USER_ID = "null";
            std::string PROJECT_ID = "null";

            std::string socket_addr = "http://192.168.29.147:8081";

            uint32_t frameNumber  = 0;
            bool socket_started = false;
            bool socket_connection_init = false;
            bool socket_connection_running = false;
            bool waiting_for_accept = true;
            bool received_new_connection_request = false;
            bool ended_existing_connection = true;

            sio::client io;
            rtc::IExtensionControl* control_;
            std::string audio_metadata;
            std::list<int16_t> init_audio_data;

            void _resetVariables(){
                frameNumber = 0;
                socket_started = false;
                socket_connection_running = false;
                waiting_for_accept = true;
                received_new_connection_request = false;
            }

            void initSocketConection();
        };
    }
}


#endif //AGORAMARSVIEWMARKETPLACE_AUDIOTRANSCRIPTPROVIDER_H
