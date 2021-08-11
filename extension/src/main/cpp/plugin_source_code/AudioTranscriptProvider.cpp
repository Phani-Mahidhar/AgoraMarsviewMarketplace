//
// Created by kphan on 7/30/2021.
//

#include "AudioTranscriptProvider.h"
#include "../logutils.h"
#include "../socket.io-client-cpp/src/sio_client.h"
#include <string.h>
#include <mutex>
#include <iostream>
#include "Utils.h"
#include <map>

std::mutex _lock;
namespace agora {
    namespace extension {

        AudioTranscriptProvider::AudioTranscriptProvider():RefCountInterface(){

            PRINTF_INFO("transcriptprovider constructor");
            io.set_open_listener([&]() {
                PRINTF_INFO("websocket send");
                initSocketConection();
            });
            io.set_socket_open_listener([&](std::string const& nsp){
                PRINTF_INFO("socket opened");
            });

            io.socket()->on("connection-status", [&](
                    std::string const& name,
                    sio::message::ptr const& data,
                    bool isAck,
                    sio::message::list &ack_resp
                    ){
                 waiting_for_accept = false;
                 std::string connectionState = data->get_map()["connection"]->get_string();
                 PRINTF_INFO("connectionState = %s", connectionState.c_str());
                 socket_connection_running = (connectionState == "true") ? true : false;
                 if (control_ != nullptr) {
                     std::string connectionErr = data->get_map()["error"]->get_string();
                     std::string connectionMsg = data->get_map()["msg"]->get_string();
                     PRINTF_INFO("connectionErr = %s", connectionErr.c_str());
                     PRINTF_INFO("connectionMsg = %s", connectionMsg.c_str());

                     JSONBuilder builder;
                     builder.addObject("connection_state", connectionState);
                     builder.addObject( "error", connectionErr);
                     builder.addObject("msg", connectionMsg);
                     if(socket_connection_running){
                         std::string audioId = data->get_map()["audio_id"]->get_string();
                         builder.addObject("audio_id", audioId);
                     }
                     control_->fireEvent(VENDOR_NAME_STRING, AUDIO_FILTER_NAME, "connectionState", builder.getJSONObject().c_str());
                 }
                 if(!socket_connection_running){
                     io.socket()->close();
                     _resetVariables();
                     socket_connection_failed = true;
                 }else{
                     socket_connection_failed = false;
                 }
             });

            io.socket()->on("transactionId", [&](
                    std::string const& name,
                    sio::message::ptr const& data,
                    bool isAck,
                    sio::message::list &ack_resp
            ){
                std::string transactionId = data->get_map()["txnId"]->get_string();
                std::string audioId = data->get_map()["audio_id"]->get_string();
                JSONBuilder builder;
                builder.addObject("txnId", transactionId);
                builder.addObject( "audio_id", audioId);
                if (control_ != nullptr) {
                    control_->fireEvent(VENDOR_NAME_STRING, AUDIO_FILTER_NAME, "transactionId", builder.getJSONObject().c_str());
                }
//                io.socket()->close();
                if(received_new_connection_request){
                    _resetVariables();
                    initSocketConection();
                }
            });

        }
        AudioTranscriptProvider::~AudioTranscriptProvider(){
            PRINTF_INFO("transcriptprovider destructor");
            if(socket_connection_running){
                io.socket()->close();
                io.sync_close();
            }
        }

        int AudioTranscriptProvider::processFrame(const media::base::AudioPcmFrame& inAudioPcmFrame,
                                                     media::base::AudioPcmFrame& adaptedPcmFrame) {

            if(!socket_connection_init){
                std::map<std::string, std::string> connectOptions;
                connectOptions.insert(std::pair<std::string, std::string>("path", ""));
                io.connect("http://agorasockets.marsview.ai");
//                io.connect("http://192.168.29.147:3004");
                audio_metadata = serializePCMFrameMetadata(inAudioPcmFrame);
                socket_connection_init = true;
            }
            if((!socket_started || waiting_for_accept ) && !socket_connection_failed){
                size_t length = inAudioPcmFrame.samples_per_channel_ * inAudioPcmFrame.num_channels_;
                if(length >= inAudioPcmFrame.kMaxDataSizeSamples){
                    length = inAudioPcmFrame.kMaxDataSizeSamples;
                }
                for(int i=0; i < length; i++){
                    init_audio_data.push_back(inAudioPcmFrame.data_[i]);
                }
            }
            if(socket_connection_running){
                if(frameNumber == 0){
                    std::string initSerialData = serializePCMFrameAudioData(init_audio_data, frameNumber);
                    io.socket()->emit("pcm-frame", initSerialData);
                    frameNumber++;
                }
                std::string frameJson = serializePCMFrameAudioData(inAudioPcmFrame, frameNumber);
                io.socket()->emit("pcm-frame", frameJson);
                frameNumber++;
            }

            memcpy(&adaptedPcmFrame, &inAudioPcmFrame, sizeof(inAudioPcmFrame));
            return 0;
        }

        void AudioTranscriptProvider::setUserKeys(std::string key, std::string value){
            if(key == "API_KEY"){
                API_KEY = value;
            } else if (key == "SECRET_KEY") {
                SECRET_KEY = value;
            } else if(key == "USER_ID") {
                USER_ID = value;
            } else if(key == "socket_addr"){
                socket_addr = value;
            }
            else if(key == "PROJECT_ID"){
                PROJECT_ID = value;
            }
        }

        void AudioTranscriptProvider::startStreaming() {
//            PRINTF_INFO("Socket_connection_running %d",socket_connection_running);
//            PRINTF_INFO("Socket_connection_init %d",socket_connection_init);

            if(socket_connection_running){
                received_new_connection_request = true;
            }
            else{
                if(socket_connection_init){
                    initSocketConection();
                    socket_connection_failed = false;
                }
            }
        }

        void AudioTranscriptProvider::endStreaming() {
            if(socket_connection_running){
                io.socket()->emit("end-connection", static_cast<sio::message::list>(""), [&](const sio::message::list &){
                });
                _resetVariables();
            }
        }

        void AudioTranscriptProvider::initSocketConection() {
            JSONBuilder connection_data;
            connection_data.addJsonObject("metadata", audio_metadata);
            connection_data.addObject("api_key", API_KEY);
            connection_data.addObject("secret_key", SECRET_KEY);
            connection_data.addObject("user_id", USER_ID);
            connection_data.addObject("projectId", PROJECT_ID);
            io.socket()->emit("init-connection", connection_data.getJSONObject());
            init_audio_data.clear();
            socket_started = true;
        }
    }
}

