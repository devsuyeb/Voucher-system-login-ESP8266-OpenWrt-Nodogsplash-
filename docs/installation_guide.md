# Installation Guide for ESP8266 Wi-Fi Voucher Captive Portal with Nodogsplash on OpenWrt

This guide explains how to set up a voucher-based captive portal using Nodogsplash on OpenWrt and an ESP8266 module (e.g., Wemos D1 Mini Pro) as a local voucher validator.

---

## 1. Flash and Configure ESP8266

- Open the Arduino sketch file: `firmware/voucher_login.ino`.
- Upload it to your ESP8266 using the Arduino IDE.
- After uploading, power it up it should automatically connect to your Openwrt router via Wi-Fi.
- Make sure you edited and entered your ssid in the code.
- You can assign a static ip to the ESP8266 from your router

---

## 2. Add Vouchers via ESP8266

1. Find the ESP8266 IP from your router’s “Connected Clients” list.
2. Visit the admin panel to add vouchers:

```
http://192.168.x.x/admin
```

Replace `x.x` with the actual IP shown for your ESP8266.

---

## 3. Connect to Your OpenWrt Router via SSH

Use this command (replace the IP address if needed):

```bash
ssh -oHostKeyAlgorithms=+ssh-rsa root@192.168.1.1
```

---

## 4. Install Nodogsplash

Cleanly reinstall Nodogsplash:

```bash
opkg update
opkg remove nodogsplash  # optional, if already installed
opkg install nodogsplash
```

---

## 5. Configure Nodogsplash

Open the main configuration file:

```bash
vi /etc/config/nodogsplash
```

### Allow Local IPs (like ESP8266) Without Authentication

Add the following block at the bottom of the file:

```text
config authenticated_users 'lan_access'
    option target 'ACCEPT'
    option dest_ip '192.168.1.0/24'
```

This allows the ESP8266 to be accessed even before the client logs in.

### Set Session and Timeout Settings

Find and edit the following lines:

```text
# Client timeouts in minutes
option preauthidletimeout '30'     # Time before login timeout (30 min)
option authidletimeout '1440'      # Idle time after auth (24 hrs)
option sessiontimeout '1440'       # Max session length (24 hrs)
option checkinterval '300'         # Check interval in seconds (5 min)
```

---

## 6. Edit Splash Page HTML

Replace the default splash page with your custom HTML:

```bash
vi /etc/nodogsplash/htdocs/splash.html
```

Paste the full working HTML provided in this repository.

---

## 7. Restart Nodogsplash

Apply all changes:

```bash
/etc/init.d/nodogsplash restart
```

---



## 8. Test/Usage

- Clients are redirected to the splash page when connecting to Wi-Fi.
- The user enters their voucher code.
- The ESP8266 validates it and removes it from memory.
- If valid, internet access is granted for 24 hours.

---

## 9. Troubleshooting

- Ensure the ESP8266 is reachable from OpenWrt.
- Confirm the splash page form contains the correct token (`$tok`).
- Check the ESP8266 serial monitor for validation logs.
- Restart Nodogsplash after any changes to config or HTML.
- Make sure timeouts are correctly set to avoid auto logouts.

---

## ⭐ If You Found This Useful

Please consider starring the [GitHub repository](https://github.com/devsuyeb/Wi-Fi_Voucher_system_login_OpenWrt_Nodogsplash_ESP8266) and sharing the project with others!

---
