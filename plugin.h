
#ifndef PLUGIN_H
#define PLUGIN_H

#include <string>
#include <syslog.h>
#include <Security/AuthorizationPlugin.h>

#define AUTH_MECHANISM_ID "MyTestMechanism"

struct MyTestPlugin
{
    std::string m_mechanismsId;
    const AuthorizationCallbacks* m_authCallbacks;

    MyTestPlugin(const AuthorizationCallbacks* callbacks):
        m_authCallbacks(callbacks), m_mechanismsId(AUTH_MECHANISM_ID) {}

    static AuthorizationPluginRef createMyTestPlugin(const AuthorizationCallbacks* callbacks) {
        syslog(LOG_INFO, "create plugin");
        MyTestPlugin* plugin = new MyTestPlugin(callbacks);
        return plugin;
    }

    static OSStatus destroyMyTestPlugin(AuthorizationPluginRef inPlugin) {
        syslog(LOG_INFO, "destory plugin");
        MyTestPlugin* plugin = (MyTestPlugin*) inPlugin;
        delete plugin;
        return errAuthorizationSuccess;
    }
};

struct MyTestMechanism
{
    MyTestPlugin* m_plugin;
    AuthorizationEngineRef m_engineRef;

    MyTestMechanism(MyTestPlugin* plugin, AuthorizationEngineRef engineRef): 
        m_plugin(plugin), m_engineRef(engineRef) {}

    static OSStatus createMyTestMechanism(AuthorizationPluginRef inPlugin,
                                        AuthorizationEngineRef inEngine,
                                        AuthorizationMechanismId  mechanismId,
                                        AuthorizationMechanismRef *outMechanism) {
        syslog(LOG_INFO, "create mechanism");
        MyTestPlugin* plugin = (MyTestPlugin*) inPlugin;
        if (plugin->m_mechanismsId != std::string(mechanismId)) {
            syslog(LOG_ERR, "mechanism id '%s' not vaild", mechanismId);
            return errAuthorizationInternal;
        }
        try {
            *outMechanism = new MyTestMechanism(plugin, inEngine);
            return errAuthorizationSuccess;
        } catch (std::exception& e) {
            syslog(LOG_ERR, "create mechanism failed: %s", e.what());
            return errAuthorizationInternal;
        }
    }

    static OSStatus invokeMyTestMechanism(AuthorizationMechanismRef inMechanism) {
        MyTestMechanism* mechanism = (MyTestMechanism*) inMechanism;
        try {
            syslog(LOG_INFO, "invoke mechanism");
            return mechanism->m_plugin->m_authCallbacks->SetResult(mechanism->m_engineRef, kAuthorizationResultAllow);
        } catch (std::exception& e) {
            syslog(LOG_ERR, "invoke mechanism failed: %s", e.what());
            return mechanism->m_plugin->m_authCallbacks->SetResult(mechanism->m_engineRef, kAuthorizationResultDeny);
        }
    }

    static OSStatus deactivateMyTestMechanism(AuthorizationMechanismRef inMechanism) {
        MyTestMechanism* mechanism = (MyTestMechanism*) inMechanism;
        try {
            syslog(LOG_INFO, "deactivate mechanism");
            return mechanism->m_plugin->m_authCallbacks->DidDeactivate(mechanism->m_engineRef);
        } catch (std::exception& e) {
            syslog(LOG_ERR, "deactivate mechanism failed: %s", e.what());
            return errAuthorizationInternal;
        }
    }

    static OSStatus destroyMyTestMechanism(AuthorizationMechanismRef inMechanism) {
        syslog(LOG_INFO, "destroy mechanism");
        MyTestMechanism* mechanism = (MyTestMechanism*)inMechanism;
        delete mechanism;
        return noErr;
    }
};

#endif 
