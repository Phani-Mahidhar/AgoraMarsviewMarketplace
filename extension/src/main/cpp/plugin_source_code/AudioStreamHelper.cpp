//
// Created by kphan on 7/30/2021.
//

#include "AudioStreamHelper.h"

namespace agora {
    namespace extension {

        std::string serializePCMFrame(const media::base::AudioPcmFrame& audioPcmFrame){
            JSONBuilder jsonBuilder;
            jsonBuilder.addObject("capture_timestamp", audioPcmFrame.capture_timestamp);
            jsonBuilder.addObject("samples_per_channel", audioPcmFrame.samples_per_channel_);
            jsonBuilder.addObject("sample_rate_hz", audioPcmFrame.sample_rate_hz_);
            jsonBuilder.addObject("num_channels", audioPcmFrame.num_channels_);
            jsonBuilder.addObject("bytes_per_sample", audioPcmFrame.bytes_per_sample);
            jsonBuilder.addObject("data", (audioPcmFrame.data_),
                                  (audioPcmFrame.kMaxDataSizeSamples));
            return jsonBuilder.getJSONObject();
        }

        std::string serializePCMFrameMetadata(const media::base::AudioPcmFrame &audioPcmFrame){
            JSONBuilder jsonBuilder;
            jsonBuilder.addObject("capture_timestamp", audioPcmFrame.capture_timestamp);
            jsonBuilder.addObject("samples_per_channel", audioPcmFrame.samples_per_channel_);
            jsonBuilder.addObject("sample_rate_hz", audioPcmFrame.sample_rate_hz_);
            jsonBuilder.addObject("num_channels", audioPcmFrame.num_channels_);
            jsonBuilder.addObject("bytes_per_sample", audioPcmFrame.bytes_per_sample);
            return jsonBuilder.getJSONObject();
        }

        std::string serializePCMFrameAudioData(const media::base::AudioPcmFrame &audioPcmFrame, uint32_t frameNumber){
            JSONBuilder jsonBuilder;
            size_t length = audioPcmFrame.samples_per_channel_ * audioPcmFrame.num_channels_;
            if(length >= audioPcmFrame.kMaxDataSizeSamples){
                length = audioPcmFrame.kMaxDataSizeSamples;
            }
            jsonBuilder.addObject("frame_number",frameNumber);
            jsonBuilder.addObject("audio_data", (audioPcmFrame.data_), length);
            return jsonBuilder.getJSONObject();
        }

        std::string serializePCMFrameAudioData(std::list<int16_t> data, uint32_t frameNumber){
            uint16_t arr[data.size()];
            std::copy(data.begin(), data.end(), arr);
            JSONBuilder jsonBuilder;
            jsonBuilder.addObject("frame_number", frameNumber);
            jsonBuilder.addObject("audio_data", arr, data.size());
            return jsonBuilder.getJSONObject();
        }


        void JSONBuilder::addObject(std::string key, int value) {
            std::string baseString = "";
            baseString += ("\"" + key + "\"");
            baseString += ":";
            baseString += std::to_string(value);
            _insertObject(baseString);
        }
        void JSONBuilder::addObject(std::string key, std::string value) {
            std::string baseString = "";
            baseString += ("\"" + key + "\"");
            baseString += ":";
            baseString += ("\"" + value + "\"");
            _insertObject(baseString);
        }

        void JSONBuilder::addObject(std::string key, std::string *values, int length) {
            std::string baseString = "";
            baseString += ("\"" + key + "\"");
            baseString += ":[";
            for(int i = 0; i < length; i++)
            {
                baseString += "\"" + values[i] + "\"" + ",";
            }
            baseString.pop_back();
            baseString += "]";
            _insertObject(baseString);
        }
        void JSONBuilder::addObject(std::string key, int *values, int length) {
            std::string baseString = "";
            baseString += ("\"" + key + "\"");
            baseString += ":[";
            for (int i = 0; i < length; i++) {
                baseString += std::to_string(values[i]) + ",";
            }
            baseString.pop_back();
            baseString += "]";
            _insertObject(baseString);
        }
        void JSONBuilder::addObject(std::string key, short *values, int length) {
            std::string baseString = "";
            baseString += ("\"" + key + "\"");
            baseString += ":[";
            for (int i = 0; i < length; i++) {
                baseString += std::to_string(values[i]) + ",";
            }
            baseString.pop_back();
            baseString += "]";
            _insertObject(baseString);
        }

        template <typename T>
        void JSONBuilder::addObject(std::string key, T* values, int length) {
            std::string baseString = "";
            baseString += ("\"" + key + "\"");
            baseString += ":[";
            for (int i = 0; i < length; i++) {
                baseString += std::to_string(values[i]) + ",";
            }
            baseString.pop_back();
            baseString += "]";
            _insertObject(baseString);
        }

        void JSONBuilder::addJsonObject(std::string key, std::string value) {
            std::string baseString = "";
            baseString += ("\"" + key + "\"");
            baseString += ":";
            baseString += value ;
            _insertObject(baseString);
        }
    }
}
