//
// Created by kphan on 7/30/2021.
//

#ifndef AGORAMARSVIEWMARKETPLACE_AUDIOSTREAMHELPER_H
#define AGORAMARSVIEWMARKETPLACE_AUDIOSTREAMHELPER_H

#include "AgoraRtcKit/AgoraMediaBase.h"
#include <iostream>
#include "../logutils.h"
#include <list>


namespace agora {
    namespace extension {
        std::string serializePCMFrame(const media::base::AudioPcmFrame &audioPcmFrame);
        std::string serializePCMFrameMetadata(const media::base::AudioPcmFrame &audioPcmFrame);
        std::string serializePCMFrameAudioData(const media::base::AudioPcmFrame &audioPcmFrame);
        std::string serializePCMFrameAudioData(std::list<int16_t> data, uint32_t frameNumber);
        std::string serializePCMFrameAudioData(const media::base::AudioPcmFrame &audioPcmFrame, uint32_t frameNumber);

        class JSONBuilder{
            public:
                JSONBuilder(){
                    JSONObject = "{}";
                }
                JSONBuilder(JSONBuilder& builder){
                    JSONObject = builder.getJSONObject();
                }
                void addObject(std::string key, std::string value);
                void addObject(std::string key, int value);
                void addObject(std::string key, std::string *values, int length);
                void addObject(std::string key, int *values, int length);
                void addObject(std::string key, short *values, int length);
                template <typename T>
                void addObject(std::string key, T* values, int length);
                void addJsonObject(std::string key, std::string value);
                std::string getJSONObject(){
//                    PRINTF_INFO("%s", JSONObject.c_str());
                    return JSONObject;
                }

            private:
                std::string JSONObject;
                void _insertObject(std::string str){
                    int almostLastIndex = JSONObject.length() -1;
                    if(almostLastIndex == 1){
                        JSONObject.insert(almostLastIndex, str);
                    }
                    else{
                        JSONObject.insert(almostLastIndex, "," + str);
                    }
                }
        };
    }
}




#endif //AGORAMARSVIEWMARKETPLACE_AUDIOSTREAMHELPER_H
