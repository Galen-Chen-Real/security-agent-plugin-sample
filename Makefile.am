PLUGIN_DIR		    = /Library/Security/SecurityAgentPlugins/MyTest.bundle/Contents

macosdir			= $(PLUGIN_DIR)/MacOS
macos_LTLIBRARIES		= MyTest.la
MyTest_la_SOURCES		= plugin.cpp
MyTest_la_LDFLAGS		= -module -Wl,-framework -Wl,Security

plugindir 			= $(PLUGIN_DIR)
plugin_DATA			= Info.plist version.plist

install-exec-hook:
	/usr/bin/install -c -o root -g wheel -m 0755 config.sh $(PLUGIN_DIR)/Resources