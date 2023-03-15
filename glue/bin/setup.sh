#!/bin/sh

# To connect to the desktop
sudo snap connect confined-shell:x11

# To launch other snaps
sudo snap connect confined-shell:desktop-launch

# To Monitor battery, WiFi, etc
sudo snap connect confined-shell:hardware-observe

# For DM
sudo snap connect confined-shell:login-session-control
if [ -d "/usr/share/wayland-sessions/" ]
then sudo cp /snap/confined-shell/current/usr/share/wayland-sessions/confined-shell.desktop /usr/share/wayland-sessions/
fi