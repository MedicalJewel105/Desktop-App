#include "executable_signature.h"

#if defined _WIN32
    #include "executable_signature_win.h"
#elif defined __APPLE__
    #include "executable_signature_mac.h"
#elif defined(linux) || defined(__linux__)
    #include "executablesignature_linux.h"
#else
    #error platform detection macro not defined
#endif

ExecutableSignature::ExecutableSignature()
{
#ifdef USE_SIGNATURE_CHECK
    d_ptr = new ExecutableSignaturePrivate(this);
#else
    d_ptr = nullptr;
#endif
}

ExecutableSignature::~ExecutableSignature()
{
    if (d_ptr != nullptr) {
        delete d_ptr;
    }
}

bool ExecutableSignature::verify(const std::wstring &exePath)
{
#ifdef USE_SIGNATURE_CHECK
    return d_ptr->verify(exePath);
#else
    (void)exePath;
    return true;
#endif
}

bool ExecutableSignature::verifyWithSignCheck(const std::wstring &exePath)
{
#ifdef USE_SIGNATURE_CHECK
    #if defined __APPLE__
        return d_ptr->verifyWithSignCheck(exePath);
    #else
        return d_ptr->verify(exePath);
    #endif
#else
    (void)exePath;
    return true;
#endif
}

const std::wstring& ExecutableSignature::lastError() const
{
    return lastError_;
}

void ExecutableSignature::setLastError(const std::wstring& error)
{
    lastError_ = error;
}
