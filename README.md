# security-agent-plugin-sample
This is a security agent plugin for macOS.

# How to use?
## 1. Prepare:
Install XCode, XCode command line and automake.
```
$ sudo brew install automake
```

## 2. Build and install:
```
$ autoreconf --install
$ ./configure
$ make
$ sudo make install
```

## 3. Enable this plugin:
```
$ sudo /Library/Security/SecurityAgentPlugins/MyTest.bundle/Contents/Resources/config.sh enable
```

## 4. Test this plugin:
Log out and log in, you can see this plugin log on the system.log.
```
$ sudo tail -f /var/log/system.log | grep -E "authorizationhosthelper"
```

## 5. Disable this plugin:
```
$ sudo /Library/Security/SecurityAgentPlugins/MyTest.bundle/Contents/Resources/config.sh disable
```
