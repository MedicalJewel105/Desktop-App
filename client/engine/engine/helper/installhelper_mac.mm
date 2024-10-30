#include "installhelper_mac.h"

#include "../../../common/version/windscribe_version.h"
#include "names.h"
#include "utils/logger.h"

#import <Foundation/Foundation.h>
#import <ServiceManagement/ServiceManagement.h>
#import <Security/Authorization.h>

bool InstallHelper_mac::installHelper(bool &isUserCanceled)
{
    isUserCanceled = false;
    NSString *helperLabel = @HELPER_BUNDLE_ID;
    BOOL result = NO;

    NSDictionary *installedHelperJobData  = CFBridgingRelease(SMJobCopyDictionary(kSMDomainSystemLaunchd, (CFStringRef)helperLabel));
    if (installedHelperJobData) {
        NSString*       installedPath           = [[installedHelperJobData objectForKey:@"ProgramArguments"] objectAtIndex:0];
        NSURL*          installedPathURL        = [NSURL fileURLWithPath:installedPath];

        NSDictionary*   installedInfoPlist      = CFBridgingRelease(CFBundleCopyInfoDictionaryForURL((CFURLRef)installedPathURL));
        NSString*       installedBundleVersion  = [installedInfoPlist objectForKey:@"CFBundleVersion"];
        NSInteger       installedVersion        = [installedBundleVersion integerValue];

        qCDebug(LOG_BASIC) << "installed helper version: " << (long)installedVersion;

        NSBundle*       appBundle       = [NSBundle mainBundle];
        NSURL*          appBundleURL    = [appBundle bundleURL];

        NSURL*          currentHelperToolURL    = [appBundleURL URLByAppendingPathComponent:@HELPER_BUNDLE_ID_PATH_FROM_ENGINE];
        NSDictionary*   currentInfoPlist        = CFBridgingRelease(CFBundleCopyInfoDictionaryForURL((CFURLRef)currentHelperToolURL));
        NSString*       currentBundleVersion    = [currentInfoPlist objectForKey:@"CFBundleVersion"];
        NSInteger       currentVersion          = [currentBundleVersion integerValue];

        qCDebug(LOG_BASIC) << "current helper version: " << (long)currentVersion;

        if (installedVersion == currentVersion && isAppMajorMinorVersionSame()) {
            return true;
        } else if (installedVersion >= currentVersion) {
            // If we are downgrading, (or the helper version is the same but app version differs),
            // we need to uninstall the previous helper first (SMJobBless will not let us downgrade)
            uninstallHelper();
        }
    } else {
        qCDebug(LOG_BASIC) << "Not installed helper";
    }

    AuthorizationItem authItem      = { kSMRightBlessPrivilegedHelper, 0, NULL, 0 };
    AuthorizationRights authRights  = { 1, &authItem };
    AuthorizationFlags flags        = kAuthorizationFlagDefaults |
                                      kAuthorizationFlagInteractionAllowed |
                                      kAuthorizationFlagPreAuthorize |
                                      kAuthorizationFlagExtendRights;

    AuthorizationRef authRef = NULL;

    // Obtain the right to install privileged helper tools (kSMRightBlessPrivilegedHelper).
    OSStatus status = AuthorizationCreate(&authRights, kAuthorizationEmptyEnvironment, flags, &authRef);
    if (status != errAuthorizationSuccess) {
        qCDebug(LOG_BASIC) << "Failed to create AuthorizationRef. Error code:" << (int)status;
        isUserCanceled = true;
    } else {
        // This does all the work of verifying the helper tool against the application
        // and vice-versa. Once verification has passed, the embedded launchd.plist
        // is extracted and placed in /Library/LaunchDaemons and then loaded. The
        // executable is placed in /Library/PrivilegedHelperTools.
        //
        CFErrorRef outError = NULL;
        result = SMJobBless(kSMDomainSystemLaunchd, (__bridge CFStringRef)helperLabel, authRef, &outError);
        if (outError) {
            NSError *error = (NSError *)outError;
            qCDebug(LOG_BASIC) << QString::fromCFString((CFStringRef)[error localizedDescription]);
            CFRelease(outError);
        }
    }

    return result == YES;
}

bool InstallHelper_mac::uninstallHelper()
{
    qCDebug(LOG_BASIC) << "Uninstalling helper.";
    NSString *scriptContents = @"do shell script \"launchctl remove /Library/LaunchDaemons/com.windscribe.helper.macos.plist;"
                                                  "rm /Library/LaunchDaemons/com.windscribe.helper.macos.plist;"
                                                  "rm /Library/PrivilegedHelperTools/com.windscribe.helper.macos\" with administrator privileges";
    NSAppleScript *script = [[NSAppleScript alloc] initWithSource:scriptContents];
    NSAppleEventDescriptor *desc;
    desc = [script executeAndReturnError:nil];
    if (desc == nil) {
        qCDebug(LOG_BASIC) << "Couldn't remove the previous helper.";
        return true;
    } else {
        qCDebug(LOG_BASIC) << "Removed previous helper.";
        return false;
    }
}

bool InstallHelper_mac::isAppMajorMinorVersionSame()
{
    CFURLRef url = CFURLCreateWithString(NULL, CFSTR("/Applications/Windscribe.app"), NULL);
    if (url == NULL) {
        return false;
    }

    CFBundleRef bundle = CFBundleCreate(NULL, url);
    CFRelease(url);
    if (bundle == NULL) {
        return false;
    }

    CFStringRef version = (CFStringRef)CFBundleGetValueForInfoDictionaryKey(bundle, kCFBundleVersionKey);
    CFRelease(bundle);

    if (version && CFStringHasPrefix(version, CFSTR(WINDSCRIBE_MAJOR_MINOR_VERSION_STR))) {
        return true;
    }

    return false;
}
