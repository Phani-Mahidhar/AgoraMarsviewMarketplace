#include <jni.h>
#include <string>
#include "plugin_source_code/ExtensionProvider.h"
#include "AgoraRtcKit/AgoraExtensionProviderEntry.h"
#include "logutils.h"
#include "plugin_source_code/JniHelper.h"
#include "plugin_source_code/Utils.h"

using namespace agora::extension;

//JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
//    PRINTF_INFO("JNI_OnLoad");
//    JniHelper* jniHelper = JniHelper::createJniHelper(vm);
//    JNIEnv *env = jniHelper->attachCurrentThread();
//    jobject globalContext = env->NewGlobalRef(jniHelper->getGlobalContext(&env));
//    AndroidContextHelper::setContext(globalContext);
//    return JNI_VERSION_1_6;
//}

JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved) {
    PRINTF_INFO("JNI_OnUnload");
//    vm->DetachCurrentThread();
//    vm->DestroyJavaVM();
//    JniHelper::release();
}
// Register extension provider
// No need to use quotation marks for input parameter of PROVIDER_NAME
REGISTER_AGORA_EXTENSION_PROVIDER( Marsview , agora::extension::ExtensionProvider);
//
