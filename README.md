# AGORA MARSVIEW EXTENSION 

This project implements the Marsview's android extension that is present in the agora ExtensionMarketPlace. Agora provides SDK's for real time video, audio and messaging communication. Our extension technically according to the agora documentation is and Audio Filter. The general documentation for creating an agora Audio Extension is present [Audio Filter Plugins.docx](./docs/Audio%20Filter%20Plugins.docx). 

**HOW IT WORKS**: 
    
- Context: When a developer uses agora SDK for real-time communication in his android application he can download extensions from the ExtensionMarketPlace for additional functionality. These extensions can be enabled in run time in his application for additional processing of audio and video.

- Extension Description: When our extension is enabled in the android application audio received through agora SDK is routed through our extension then is sent to agora-backend through a socket.io connection which is then recorded until the extension is disabled, at which point the recording stops in our backend and the recorded audio file will be POSTed to marsview servers. And in return the POSTed audio file's transaction Id will be sent to the android, from which the developer can use the [Marsview APIs](https://docs.marsview.ai/)


**PREREQUISITES**:
  
* Latest Android Studio (at the time of writing, android studio 4.0).
* Android SDK
* Android (Native development Kit for working with c and c++).
* Android Phone

Once you open this Project in Android Studio change from **Android** to **Project** like here to see the entire file structure.

![project](./docs/README%20images/as_project.png)


There are two parts to particular android project. These are: 
* Test app ("app" folder) 
    - **Description**: This folder is created when a new Android project is created. And this the entrypoint for the project. THe relevant documentation for the application can be found [here](./app/README.md).
* Extension ("extension" folder)
    - **Description**: The extension module is created by going into File > New > New Module. and then new Android Library. This is the extension that is going to be used by the agora SDK. The relevent documentation for extension can be found [here](./extension/README.md).

The demo application for creating an extension using agora SDKs for android can be found at [(https://download.agora.io/demo/release/AgoraWithByteDanceAndroid.zip](https://download.agora.io/demo/release/AgoraWithByteDanceAndroid.zip)


The [Provision APIs](./docs/Provisioning%20API.docx) and [Usage & Billing APIs](./docs/Usage%20and%20Billing%20API.docx) respectively can be found here.
