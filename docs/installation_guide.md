# Installation Guide for ESP8266 Wi-Fi Voucher Captive Portal with Nodogsplash on OpenWrt

This guide explains how to set up Nodogsplash on OpenWrt and connect it to an ESP8266 (like Wemos D1 Mini Pro) for voucher-based captive portal authentication.

---

## 1. Connect to Your OpenWrt Router via SSH

Use this command (replace the IP address if needed):

```bash
ssh -oHostKeyAlgorithms=+ssh-rsa root@192.168.1.1
```

---

## 2. Install Nodogsplash

Cleanly reinstall Nodogsplash to ensure a fresh setup:

```bash
opkg update
opkg remove nodogsplash  # optional, if already installed
opkg install nodogsplash
```

---

## 3. Configure Nodogsplash

Open the main configuration file:

```bash
vi /etc/config/nodogsplash
```

### Allow Local IPs (like ESP8266) Without Authentication

Add the following block at the bottom of the file to whitelist local devices (e.g., ESP8266):

```text
config authenticated_users 'lan_access'
    option target 'ACCEPT'
    option dest_ip '192.168.1.0/24'
```

### Set Session and Timeout Settings

Find and adjust these settings as needed:

```text
# Client timeouts in minutes
option preauthidletimeout '30'     # Time before login timeout (30 min)
option authidletimeout '1440'      # Idle time after auth (24 hrs)
option sessiontimeout '1440'       # Max session length (24 hrs)
option checkinterval '300'         # Check interval in seconds (5 min)
```

---

## 4. Edit Splash Page HTML

Edit or replace the splash page file:

```bash
vi /etc/nodogsplash/htdocs/splash.html
```

Replace the content with the custom HTML file provided in this repository.

---

## 5. Restart Nodogsplash

Apply changes by restarting the service:

```bash
/etc/init.d/nodogsplash restart
```

---

## 6. Flash and Configure ESP8266

- Open the Arduino sketch file: `firmware/voucher_login.ino`.
- Upload it to your ESP8266 device using the Arduino IDE.
- Ensure the ESP8266 IP and port match your OpenWrt router’s network settings.

---

## 7. Add Vouchers

1. Find the ESP8266 IP from your router's "Connected Clients" list.
2. Visit the ESP8266 admin panel in your browser:

```
http://192.168.x.x/admin
```

3. Use the interface to add voucher codes.

---

## 8. Usage

- When a new client connects, they are redirected to the splash page.
- The user enters a voucher code.
- The ESP8266 validates the voucher and removes it from memory.
- If valid, Nodogsplash grants internet access.

---

## 9. Troubleshooting

- Make sure the token (`$tok`) is correctly included in the HTML form.
- Verify that the ESP8266 IP is accessible from the router.
- Check the ESP8266 serial monitor for validation logs.
- Always restart Nodogsplash after changing configs or HTML.

---

## ⭐ If You Found This Useful

Please consider starring the [GitHub repository](https://github.com/devsuyeb/Wi-Fi_Voucher_system_login_OpenWrt_Nodogsplash_ESP8266) and sharing the project!

---
