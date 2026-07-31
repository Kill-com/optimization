Для запуска необходимо установить:
Cmake:



choco install llvm:


perf:
sudo apt install linux-tools-generic

libpfm4:
sudo apt-get install libpfm4-dev

PAPI:
sudo apt install libpapi-dev
    for wsl:
    sudo apt install linux-tools-common linux-tools-generic linux-tools-$(uname -r)