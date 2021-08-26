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

            io.set_open_listener([&]() {
                PRINTF_INFO("Established socket connection");
                initSocketConection();
            });

            io.socket()->on("connection-status", [&](
                    std::string const& name,
                    sio::message::ptr const& data,
                    bool isAck,
                    sio::message::list &ack_resp
                    ){
                 waiting_for_accept = false;
                 PRINTF_INFO("Received connection authentication status");
                 std::string connectionState = data->get_map()["connection"]->get_string();
                 socket_connection_running = (connectionState == "true") ? true : false;
                 if (control_ != nullptr) {
                     std::string connectionErr = data->get_map()["error"]->get_string();
                     std::string connectionMsg = data->get_map()["message"]->get_string();

                     JSONBuilder builder;
                     builder.addObject("connection_state", connectionState);
                     builder.addObject( "error", connectionErr);
                     builder.addObject("message", connectionMsg);
                     if(socket_connection_running){
                         std::string audioId = data->get_map()["audio_id"]->get_string();
                         builder.addObject("audio_id", audioId);
                     }
                     control_->fireEvent(VENDOR_NAME_STRING, AUDIO_FILTER_NAME, "connectionState", builder.getJSONObject().c_str());
                 }
                 if(!socket_connection_running){
                     PRINTF_INFO("Authentication failed please check API_SECRET and API_KEY");
                     io.socket()->close();
                     _resetVariables();
                 }
             });

            io.socket()->on("ended-connection", [&](
                    std::string const& name,
                    sio::message::ptr const& data,
                    bool isAck,
                    sio::message::list &ack_resp
            ){
                PRINTF_INFO("Audio stream ended");

                ended_existing_connection = true;
                if(received_new_connection_request){
                    initSocketConection();
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

            });

        }
        AudioTranscriptProvider::~AudioTranscriptProvider(){
            if(socket_connection_running){
                io.socket()->close();
                io.sync_close();
            }
        }

        int AudioTranscriptProvider::processFrame(const media::base::AudioPcmFrame& inAudioPcmFrame,
                                                     media::base::AudioPcmFrame& adaptedPcmFrame) {

            if(!socket_connection_init){
                io.connect("https://agorasockets.marsview.ai");
//                io.connect("http://192.168.29.147:3004");
//                io.connect("https://2343-2405-201-c014-80ba-8bec-7d1a-519e-e201.ngrok.io");
                audio_metadata = serializePCMFrameMetadata(inAudioPcmFrame);
                socket_connection_init = true;
            }

            if(socket_connection_running && !ended_existing_connection){

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
            }
            else if (key == "API_SECRET"){
                SECRET_KEY = value;
            }
        }

        void AudioTranscriptProvider::startStreaming() {
            PRINTF_INFO("Audio stream starting...");
            if(!ended_existing_connection){
                received_new_connection_request = true;
            }
            else{
                if(socket_connection_init){
                    initSocketConection();
                }
            }
            _resetVariables();
        }

        void AudioTranscriptProvider::endStreaming() {

            if(socket_connection_running){
                io.socket()->emit("end-connection", static_cast<sio::message::list>(""), [&](const sio::message::list &){
                });
                socket_connection_running = false;
                PRINTF_INFO("Audio Stream ending...");

            }
        }

        void AudioTranscriptProvider::initSocketConection() {
            PRINTF_INFO("Init Connection");
            JSONBuilder connection_data;
            _resetVariables();
            connection_data.addJsonObject("metadata", audio_metadata);
            connection_data.addObject("project_api_key", API_KEY);
            connection_data.addObject("project_api_secret", SECRET_KEY);
            io.socket()->emit("init-connection", connection_data.getJSONObject());
            init_audio_data.clear();
            socket_started = true;
            ended_existing_connection = false;
        }
    }
}

