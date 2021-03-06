package agoramarketplace.marsview.test;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.Toast;


import org.json.JSONArray;
import org.json.JSONObject;


// Imports from Marsview's extension.
// Extension Manager contains information about the extension such as the EXTENSION_VENDOR_NAME, EXTENSION_AUDIO_FILTER_NAME
import agoramarketplace.marsview.extension.ExtensionManager;
import agoramarketplace.marsview.extension.MarsviewRequestHelper;


// Agora SDK imports.
import io.agora.rtc2.Constants;
import io.agora.rtc2.IRtcEngineEventHandler;
import io.agora.rtc2.RtcEngine; 
import io.agora.rtc2.RtcEngineConfig;
import io.agora.rtc2.video.VideoCanvas;
import io.agora.rtc2.video.VideoEncoderConfiguration;


public class MainActivity extends AppCompatActivity implements io.agora.rtc2.IMediaExtensionObserver {

    private static final String[] REQUESTED_PERMISSIONS = {
            Manifest.permission.RECORD_AUDIO,
            Manifest.permission.CAMERA
    };

    private String appId ;
    private String channelName ;
    private final static String TAG = "Agora_Marsview_Java";
    private static final int PERMISSION_REQ_ID = 22;
    private FrameLayout localVideoContainer;
    private FrameLayout remoteVideoContainer;
    private EditText socketUrlContainer;
    private Button socketButton;
    private Button rtcOnOffButton;
    private boolean rtcEngineRunning = false;
    private RtcEngine mRtcEngine;
    private SurfaceView mRemoteView;
    // socketUrl is not being used now, or anything related to the socket connection cannot be configured in the front end.
    private String socketUrl;
    private SharedPreferences prefs;
    private MarsviewRequestHelper requestHelper;

//    private final String API_KEY = "d0c900e8-056c-44da-87eb-48177abf6b95";
//    private final String SECRET_KEY = "T34G1T1-0NP49PH-GZNMG5V-FAZPQ5A";
////    private final String USER_ID = "12345678910@agora.io";
//      private final String API_KEY = "43cd3991-4551-4b92-a324-2c5214f7e4b7";
//      private final String SECRET_KEY = "8F6KK4F-8N8MQ4Q-MCJ2RMG-2KVY9DW";
//      private final String USER_ID = "65750@agora.io";
//
//    private final String API_KEY = "fc6e7577-7401-4d9a-bacb-44b6dac8de77";
//    private final String SECRET_KEY = "ZHQ7AXX-EG0MV6M-QB5M9DH-VB4DWXX";
//    private final String USER_ID = "50000@agora.io";
//    private final String PROJECT_ID = "50000";


//    private final String API_KEY = "4a7903e9-8d63-4258-b84b-11aaa56e4362";
//    private final String SECRET_KEY = "99WG7TF-HNHM4P4-Q15H3AK-MNQ46RH";
//    private final String USER_ID = "50000@agora.io";
//    private final String PROJECT_ID = "50000";

    private final String API_KEY = "1cbb6faa-e87c-4fc7-8720-b1dfe2bcb9e2";
    private final String SECRET_KEY = "3JXPZAJ-X1Y4ZHS-GWGB3QX-WAYBKRG";


    private String audioId;


    private boolean extensionEnabled = true;
    @RequiresApi(api = Build.VERSION_CODES.M)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate");
        super.onCreate(savedInstanceState);
        appId = getString(R.string.agora_app_id);
        channelName = getString(R.string.channel_name);
        setContentView(R.layout.activity_main);
        prefs = getSharedPreferences("agora-marsivew-prefs", MODE_PRIVATE);
        requestHelper = new MarsviewRequestHelper(API_KEY, SECRET_KEY);
        initUI();

        // On listener for the button which enables and disables the extension.
        rtcOnOffButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(!extensionEnabled){
                    enableExtension();
                }
                else{
                    disableExtension();
                }
            }
        });

    }

    @Override
    protected void onDestroy() {
        Log.d(TAG, "onDestroy");
        super.onDestroy();
        destroyAgoraEngine();
    }

    @RequiresApi(api = Build.VERSION_CODES.M)
    private void checkPermission() {
        Log.d(TAG, "checkPermission");
        if (checkSelfPermission(REQUESTED_PERMISSIONS[0], PERMISSION_REQ_ID) &&
                checkSelfPermission(REQUESTED_PERMISSIONS[1], PERMISSION_REQ_ID)) {
            initAgoraEngine();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == PERMISSION_REQ_ID && grantResults.length > 0 &&
                grantResults[0] == PackageManager.PERMISSION_GRANTED){
            initAgoraEngine();
        }
    }

    private void initUI() {

        localVideoContainer = findViewById(R.id.view_container);
        remoteVideoContainer = findViewById(R.id.remote_video_view_container);
        rtcOnOffButton = findViewById(R.id.rtc_on_off);
        socketUrlContainer = findViewById(R.id.socket_url);
        socketButton = findViewById(R.id.socket_url_button);

        // the socket code is no longer being used. it was previously used for testing.
        socketUrl = prefs.getString("socket-url", "http://192.168.29.147:8081");
        socketUrlContainer.setText(socketUrl);
        socketButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                socketUrl = socketUrlContainer.getText().toString();
                prefs.edit().putString("socket-url", socketUrl);
                prefs.edit().apply();
                Log.d(TAG, socketUrl);
                Toast toast = Toast.makeText(getApplicationContext(), "succesfully set url to" + socketUrl, Toast.LENGTH_LONG);
                toast.show();
            }
        });
        
        // check android permissions.
        checkPermission();
    }

    private boolean checkSelfPermission(String permission, int requestCode) {
        if (ContextCompat.checkSelfPermission(this, permission) !=
                PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, REQUESTED_PERMISSIONS, requestCode);
            return false;
        }
        return true;
    }

    private void initAgoraEngine() {
        try {
            RtcEngineConfig config = new RtcEngineConfig();
            config.mContext = this;
            config.mAppId = appId;

            config.addExtension(ExtensionManager.EXTENSION_NAME);
            config.mExtensionObserver = this;
            config.mEventHandler = new IRtcEngineEventHandler() {
                @Override
                public void onJoinChannelSuccess(String s, int i, int i1) {
                    Log.d(TAG, "onJoinChannelSuccess");
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mRtcEngine.startPreview();
                        }
                    });
                }

                @Override
                public void onFirstRemoteVideoDecoded(final int i, int i1, int i2, int i3) {
                    super.onFirstRemoteVideoDecoded(i, i1, i2, i3);
                    Log.d(TAG, "onFirstRemoteVideoDecoded  uid = " + i);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            setupRemoteVideo(i);
                        }
                    });
                }

                @Override
                public void onUserJoined(int i, int i1) {
                    super.onUserJoined(i, i1);
                    Log.d(TAG, "onUserJoined  uid = " + i);
                }

                @Override
                public void onUserOffline(final int i, int i1) {
                    super.onUserOffline(i, i1);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            onRemoteUserLeft();
                        }
                    });
                }
            };
            mRtcEngine = RtcEngine.create(config);
            //extension is enabled by default
            Log.d(TAG, "Here");
            mRtcEngine.enableExtension(ExtensionManager.EXTENSION_VENDOR_NAME, ExtensionManager.EXTENSION_AUDIO_FILTER_NAME, true);
//            enableExtension();
            setupLocalVideo();

            VideoEncoderConfiguration configuration = new VideoEncoderConfiguration(640, 360,
                    VideoEncoderConfiguration.FRAME_RATE.FRAME_RATE_FPS_30,
                    VideoEncoderConfiguration.STANDARD_BITRATE,
                    VideoEncoderConfiguration.ORIENTATION_MODE.ORIENTATION_MODE_ADAPTIVE);
            mRtcEngine.setVideoEncoderConfiguration(configuration);
            mRtcEngine.setAudioProfile(Constants.AUDIO_PROFILE_MUSIC_STANDARD_STEREO, Constants.AUDIO_SCENARIO_DEFAULT);
            mRtcEngine.setChannelProfile(Constants.CHANNEL_PROFILE_COMMUNICATION);
            mRtcEngine.setClientRole(Constants.CLIENT_ROLE_BROADCASTER);

            mRtcEngine.enableLocalVideo(true);
            mRtcEngine.enableLocalAudio(true);
            mRtcEngine.enableVideo();
            mRtcEngine.enableAudio();
            Log.d(TAG, "api call join channel");

            mRtcEngine.setExtensionProperty(ExtensionManager.EXTENSION_VENDOR_NAME, ExtensionManager.EXTENSION_AUDIO_FILTER_NAME, "API_KEY", API_KEY); // login to app.marsivew.ai
            mRtcEngine.setExtensionProperty(ExtensionManager.EXTENSION_VENDOR_NAME, ExtensionManager.EXTENSION_AUDIO_FILTER_NAME, "API_SECRET", SECRET_KEY); // login to app.marsview.ai

            mRtcEngine.joinChannel(getString(R.string.agora_temp_token), channelName, "", 0);
            mRtcEngine.startPreview();
            rtcEngineRunning = true;

//            rtcOnOffButton.setText("disable extension");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void setupLocalVideo() {
        SurfaceView view = RtcEngine.CreateRendererView(this);
        view.setZOrderMediaOverlay(true);
        localVideoContainer.addView(view);
        mRtcEngine.setupLocalVideo(new VideoCanvas(view, VideoCanvas.RENDER_MODE_HIDDEN, 0));
        mRtcEngine.setLocalRenderMode(Constants.RENDER_MODE_HIDDEN);
    }

    private void setupRemoteVideo(int uid) {
        // Only one remote video view is available for this
        // tutorial. Here we check if there exists a surface
        // view tagged as this uid.
        int count = remoteVideoContainer.getChildCount();
        View view = null;
        for (int i = 0; i < count; i++) {
            View v = remoteVideoContainer.getChildAt(i);
            if (v.getTag() instanceof Integer && ((int) v.getTag()) == uid) {
                view = v;
            }
        }

        if (view != null) {
            return;
        }

        Log.d(TAG, " setupRemoteVideo uid = " + uid);
        mRemoteView = RtcEngine.CreateRendererView(getBaseContext());
        remoteVideoContainer.addView(mRemoteView);
        mRtcEngine.setupRemoteVideo(new VideoCanvas(mRemoteView, VideoCanvas.RENDER_MODE_HIDDEN, uid));
        mRtcEngine.setRemoteRenderMode(uid, Constants.RENDER_MODE_HIDDEN);
        mRemoteView.setTag(uid);
    }

    private void onRemoteUserLeft() {
        removeRemoteVideo();
    }

    private void removeRemoteVideo() {
        if (mRemoteView != null) {
            remoteVideoContainer.removeView(mRemoteView);
        }
        mRemoteView = null;
    }

    private void destroyAgoraEngine(){
        mRtcEngine.leaveChannel();
        mRtcEngine.destroy();
        mRtcEngine = null;
        rtcEngineRunning = false;
    }
    private void disableExtension(){
        mRtcEngine.enableExtension(ExtensionManager.EXTENSION_VENDOR_NAME, ExtensionManager.EXTENSION_AUDIO_FILTER_NAME, false);
        rtcOnOffButton.setText("enable extension");
        extensionEnabled = false;
    }
    private void enableExtension(){

        mRtcEngine.enableExtension(ExtensionManager.EXTENSION_VENDOR_NAME, ExtensionManager.EXTENSION_AUDIO_FILTER_NAME, true);
        rtcOnOffButton.setText("disable extension");
        extensionEnabled = true;
    }

    // Method to receive events from the agora extensions. Implemented in the io.agora.rtc2.MediaExtensionObserver interface.
    @Override
    public void onEvent(String vendor, String extension, String key, String value) {
        Log.d(TAG, "\nkey: " + key+ "\nValue: " + value);
        if( vendor.equalsIgnoreCase("marsview") && extension.equalsIgnoreCase("TranscriptProvider")){

            if(key.equalsIgnoreCase("transactionId")){
                // Marsview provides a helper class to facilitate the developer in posting compute models
                // and get processing state of each model and metadata afterwards.
                try {
                    JSONObject data = new JSONObject(value);
                    String txnId = data.getString("txnId");
                    JSONArray enableModels = new JSONArray("[{'modelType':'speech_to_text','modelConfig':{'automatic_punctuation':true,'custom_vocabulary':['Marsview','Communication'],'speaker_seperation':{'num_speakers':1},'enableKeywords':true}},{'modelType':'emotion_analysis'},{'modelType':'sentiment_analysis'},{'modelType':'speech_type_analysis'},{'modelType':'action_items','modelConfig':{'priority':1}},{'modelType':'question_response','modelConfig':{'quality':1}},{'modelType':'extractive_summary'},{'modelType':'meeting_topics'}]");
//                    JSONArray enableModels = new JSONArray("[{\n" +
//                            "        \"modelType\":\"speech_to_text\",\n" +
//                            "        \"modelConfig\": {\n" +
//                            "            \"speaker_seperation\":{\n" +
//                            "                \"enable\":true,\n" +
//                            "                \"num_speakers\":-1\n" +
//                            "            }\n" +
//                            "        }\n" +
//                            "}]");
//                    Log.d(TAG, enableModels.toString());
//                     This is to enable models that you require before producing transcription.
//                     remember that transcription is only produced after the required models are
//                     enabled.
                    String computeDataResponse = requestHelper.postComputeDataRequest(txnId, enableModels);
                    Log.d(TAG, computeDataResponse);
                    // To get the processing state of each model that is enabled u can use the method
                    // getProcessingState
                    String processingStateResponse = requestHelper.getProcessingState(txnId);
                    Log.d(TAG, processingStateResponse);

                    // to get the metadata of the audio processed provide the transaction id string
                    // to the get requestMetadata function.
                    String MetadataResponse = requestHelper.getRequestMetadata(txnId);
                    Log.d(TAG, MetadataResponse);
                }catch(Exception e){
                    e.printStackTrace();
                }
            }
            else if(key.equalsIgnoreCase("connectionState")){
                try{
                    JSONObject  reader = new JSONObject(value);
                    String connectionState = reader.getString("connection_state");
                    Log.d(TAG, connectionState);
                    if(!connectionState.equals("true")){
                        // provide proper api key, secret key , user ID and re enable the transcription
                        // service again.
                    }else{
                        audioId = reader.getString("audio_id"); // store audio_id
                    }
                }catch (Exception e){
                    e.printStackTrace();
                }
            }
        }
    }
}
