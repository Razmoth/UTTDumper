#include <init.h>
#include <dlfcn.h>
#include <thread>
#include <filesystem>

static void main_thread() {
    Dl_info dl_info;
    if (dladdr((void*)main_thread, &dl_info) != 0) {
        std::filesystem::path path(dl_info.dli_fname);

        init(path);
    }
}

#ifdef ANDROID

#include <jni.h>
#include <iostream>
#include <android/log.h>

class StreamBuf2Log : public std::streambuf {
public:
    enum Type { OUT, ERR };
    StreamBuf2Log(Type t) {
        this->setp(buffer, buffer + bufsize - 2);
        switch (t) {
        case OUT:
            PRIORITY = ANDROID_LOG_INFO;
            strcpy(TAG, ANDROID_TAG);
            break;
        case ERR:
            PRIORITY = ANDROID_LOG_ERROR;
            strcpy(TAG, ANDROID_TAG);
            break;
    }
    }

private:
    int overflow(int c)
    {
        *this->pptr() = traits_type::to_char_type(c);
        pbump(1);
        sync();
        return 0;
    }

    int sync()
    {
        int n = pptr() - pbase();
        if (!n || (n == 1 && buffer[0] == '\n')) return 0;
        buffer[n] = '\0';
        __android_log_write(PRIORITY, TAG, buffer);
        pbump(-n);
        return 0;
    }

    static constexpr int bufsize = 2048;
    char TAG[10];
    android_LogPriority PRIORITY;
    char buffer[bufsize];
};

class StdStreamRedirector {
private:
    StreamBuf2Log outbuf;
    StreamBuf2Log errbuf;
public:
    StdStreamRedirector() : outbuf(StreamBuf2Log::OUT), errbuf(StreamBuf2Log::ERR) {
        std::cout.rdbuf(&outbuf);
        std::cerr.rdbuf(&errbuf);
    }
    ~StdStreamRedirector() {
        std::cout << std::flush;
        std::cerr << std::flush;
    }
};

StdStreamRedirector* redirector;

extern "C" jint JNIEXPORT JNI_OnLoad(JavaVM* vm, void* reserved)
{
    redirector = new StdStreamRedirector();

    JNIEnv* env = nullptr;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) == JNI_OK)
    {
        std::cout << "JavaEnv: " << env << std::endl;
    }

    std::thread(main_thread).detach();

    return JNI_VERSION_1_6;
}

extern "C" void JNIEXPORT JNI_OnUnload(JavaVM* vm, void* reserved)
{
    delete redirector;
}

#else

static void __attribute__((constructor)) onLoad() {
    std::thread(main_thread).detach();
}

static void __attribute__((destructor)) onUnload() { }

#endif