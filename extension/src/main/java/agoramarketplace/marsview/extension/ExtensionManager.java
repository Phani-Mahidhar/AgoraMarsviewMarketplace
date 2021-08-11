package agoramarketplace.marsview.extension;

import androidx.annotation.Keep;

@Keep
public class ExtensionManager {
    public static final String EXTENSION_NAME = "agora-marsview"; // Name of target link library used in CMakeLists.txt
    public static final String EXTENSION_VENDOR_NAME = "Marsview"; // Provider name used for registering in agora-marsview.cpp
    public static final String EXTENSION_AUDIO_FILTER_NAME = "TranscriptProvider"; // Audio filter name defined in ExtensionProvider.h
}
