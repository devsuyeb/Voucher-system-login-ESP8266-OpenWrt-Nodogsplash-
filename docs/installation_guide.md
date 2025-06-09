 # Installation Guide for ESP8266 Wi-Fi Voucher Captive Portal with Nodogsplash on OpenWrt

This guide covers installing, configuring, and running Nodogsplash on OpenWrt with ESP8266 voucher validation.

## 1. Connect to your OpenWrt router via SSH

Use this command (replace IP if needed):
ssh -oHostKeyAlgorithms=+ssh-rsa root@192.168.1.1

2. Install Nodogsplash
Update package lists and install Nodogsplash cleanly:
opkg update
opkg install nodogsplash

3. Configure Nodogsplash
Edit the main config file:
 vi /etc/config/nodogsplash
Make necessary changes to match your network.

#Config for allow all local ip without without authentication. This for your ESP8266 validation server can access before coupon enter

add this bottom of the config

config authenticated_users 'lan_access'
option target 'ACCEPT'
option dest_ip '192.168.1.0/24'

Set timeouts to control session duration and idle times:

find and edit

# Client timeouts in minutes
option preauthidletimeout '30'     # Idle timeout before login (30 minutes)
option authidletimeout '1440'      # Authenticated idle timeout (1440 minutes = 24h)
option sessiontimeout '1440'       # Total session timeout (24h)
option checkinterval '300'         # Timeout check interval in seconds (5 minutes)

5. Setup splash page HTML
Edit or replace the splash page:
vi /etc/nodogsplash/htdocs/splash.html
Edit the HTML and replace all code with the given HTML file.

5. Restart Nodogsplash
Apply your changes by restarting the service:

/etc/init.d/nodogsplash restart


8. Flash and configure your ESP8266
Upload the Arduino sketch (firmware/voucher_login.ino) to your ESP8266 device.

Make sure the ESP8266 firmware matches your OpenWrt IP and Nodogsplash setup.

9. Usage
When a client connects, they are redirected to the splash page.

User inputs voucher code which is validated by the ESP8266.

Upon successful validation, internet access is granted.

Troubleshooting
Ensure the splash page contains the correct token and form action URL.

Confirm your ESP8266 and router IP addresses match those in configuration files.

Check the ESP8266 serial monitor for voucher validation logs.

Restart Nodogsplash after every configuration change.

If you find this useful, please ⭐ star the repository!
