# AES-TOOL
a very basic c++ gtkmm gui app for encrypting and decrypting AES256 files and strings using gnupg

## FOR COMPILING AND RUNNING (apt)
```bash
sudo apt install g++ pkg-config libgtkmm-3.0-dev libgtkmm-3.0-1v5 gnupg
```
to compile simply run this command
```bash
g++ aestool.cpp -o aestool `pkg-config gtkmm-3.0 --cflags --libs`
```

## FOR RUNNING ONLY (apt)
```bash
sudo apt install gnupg libgtkmm-3.0-1v5
./aestool
```
