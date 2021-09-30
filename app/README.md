# ANDROID APPLICATION

**PREREQUISITES**: 
- Agora SDK: download the latest the SDK from the [agora SDK download Page](https://docs.agora.io/en/All/downloads?platform=All%20Platforms) add the .jar file into the libs directory(if it does not exist create one) in the "app" directory. Right now, It can be found in the [app/libs/agora-rtc-sdk.jar](./libs/agora-rtc-sdk.jar) location.


***DESCRIPTION*** : 

The **MainActivity.java** file is the entry point for the program execution and can be found n this location [app/src/main/java/agoramarketplace/marsview/test/MainActivity.java](./src/main/java/agoramarketplace/marsview/test/MainActivity.java). 


The [AndroidManifest.xml](./src/main/AndroidManifest.xml) file describes essential information about your app to the Android build tools, the Android operating system, and Google Play. For Example, say u want to use Bluetooth in your Android Application. The below line needs to be added in the AndroidManifest.xml

- Example:
   
        <manifest>
        ...
            <uses-permission android:name="android.permission.BLUETOOTH"/>
        ...
        </manifest>

in between the manifest tags.

The Page layout file for the Android application can be found in the [app/src/main/res/layout/activity_main.xml](./src/main/res/layout/activity_main.xml) file.

**io.agora.rtc2** is the package name for the agora sdk,

**agoramarketplace.marsview.extension** is the package name for the Marsview's extension.
-   *agoramarketplace.marsview.extension.ExtensionManager* class contains the extension details such as the VENDOR_NAME and the AUDIO_FILTER_NAME.
-    *agoramarketplace.marsview.extension.MarsviewRequestHelper* class contains the methods that help the developer in posting the Marsviw API requests such as in POSTing [Compute Request](https://docs.marsview.ai/speech-analytics-api/compute-metadata), GETting [Processing State](https://docs.marsview.ai/speech-analytics-api/compute-metadata), GETting [Request Metadata](https://docs.marsview.ai/speech-analytics-api/getting-metadata#get-request-metadata) from the transaction Id. This is implemented in the [extension/src/main/java/agoramarketplace/marsview/extension/MarsviewRequestHelper.java](../extension/src/main/java/agoramarketplace/marsview/extension/MarsviewRequestHelper.java) file.



To receive data from the extension in your MainActivity.java file u need to implement the  **io.agora.rtc2.IMediaExtensionObserver** interface. This interface implements the function
    *onEvent(String vendor, String extension, String key, String value)*. 

So, The MainActivity class line is going to become:

    public class MainActivity extends AppCompatActivity implements io.agora.rtc2.MediaExtensionObserver {
        ...
        // Method to receive events from the agora extensions. Implemented in the io.agora.rtc2.MediaExtensionObserver interface.
        @Override
        public void onEvent(String vendor, String extension, String key, String value) {
            ...
        }
    }