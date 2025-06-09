# ESP8266 Voucher Login Captive Portal

An open-source project to manage internet access using ESP8266 + OpenWrt + Nodogsplash.

## 📦 Features
- Voucher-based captive portal
- OpenWrt + Nodogsplash integration
- Real-time voucher validation using ESP8266
- EEPROM-stored voucher list
- Compatible with Wemos D1 Mini, NodeMCU, and others

## 📁 File Structure
- `firmware/` - ESP8266 Arduino code
- `html/` - Custom splash page
- `nodogsplash/` - Nodogsplash config sample

## 🚀 How to Use
1. Flash ESP8266 with code from `firmware/`
2. Upload `splash.html` to OpenWrt: `/etc/nodogsplash/htdocs/`
3. Replace `/etc/nodogsplash/nodogsplash.conf` with the one provided
4. Reboot router and enjoy!

## 🛠️ Requirements
- ESP8266 (D1 Mini, NodeMCU, etc.)
- OpenWrt router with Nodogsplash installed

## 📃 License
MIT — use this project freely with credit.
