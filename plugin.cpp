
#include "plugin.h"

static const AuthorizationPluginInterface gPluginInterface =
{
    kAuthorizationPluginInterfaceVersion,
    &MyTestPlugin::destroyMyTestPlugin,
    &MyTestMechanism::createMyTestMechanism,
    &MyTestMechanism::invokeMyTestMechanism,
    &MyTestMechanism::deactivateMyTestMechanism,
    &MyTestMechanism::destroyMyTestMechanism,
};

OSStatus
AuthorizationPluginCreate(const AuthorizationCallbacks* callbacks,
        AuthorizationPluginRef* outPlugin,
        const AuthorizationPluginInterface** outPluginInterface)
{
    syslog(LOG_INFO, "call AuthorizationPluginCreate");
    try {
        *outPlugin = MyTestPlugin::createMyTestPlugin(callbacks);
        if (*outPlugin == 0) {
            syslog(LOG_ERR, "create plugin failed: unknown error");
            return errAuthorizationInternal;
        }
    } catch (std::exception& e) {
        syslog(LOG_ERR, "create plugin failed: %s", e.what());
        return errAuthorizationInternal;
    }
    *outPluginInterface = &gPluginInterface;
    return errAuthorizationSuccess;
}
