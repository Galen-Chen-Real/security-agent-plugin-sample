#!/bin/sh

usage() {
    echo "config.sh enable|disable"
}

if [ "$#" -ne 1 ]; then
    usage
    exit 1
fi

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit 1
fi

if [ "$1" == "enable" ]; then
    security authorizationdb read system.login.console > system.login.console.orig 2>/dev/null
    # check if MyTest has already been enabled
    grep -En "MyTest:MyTestMechanism,privileged" system.login.console.orig 2>&1 >/dev/null
    if [ $? -eq 0 ]; then echo "enabled"; exit 0; fi
    # insert MyTest
    line=`grep -En "builtin:login-success" system.login.console.orig 2>/dev/null | awk -F ":" '{print $1}'`
    awk 'NR=='${line}'{print "\t\t<string>MyTest:MyTestMechanism,privileged</string>"}1' system.login.console.orig > system.login.console.new
    security authorizationdb write system.login.console < system.login.console.new
    rm -f system.login.console.orig system.login.console.new
    echo "enabled"
    exit 0
fi

if [ "$1" == "disable" ]; then
    security authorizationdb read system.login.console > system.login.console.new 2>/dev/null
    # check if MyTest has already been disabled
    grep -En "MyTest:MyTestMechanism,privileged" system.login.console.new 2>&1 >/dev/null
    if [ $? -ne 0 ]; then echo "disabled"; exit 0; fi
    # delete MyTest
    line=`grep -En "MyTest:MyTestMechanism,privileged" system.login.console.new 2>/dev/null | awk -F ":" '{print $1}'`
    sed ${line}d system.login.console.new > system.login.console.orig
    security authorizationdb write system.login.console < system.login.console.orig
    rm -f system.login.console.orig system.login.console.new
    echo "disabled"
    exit 0
fi