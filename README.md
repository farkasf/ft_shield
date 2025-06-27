# ft_shield
ft_shield is a trojan horse implementation that operates as a stealth network daemon. The project consists of two main components:

1. **Installer (`ft_shield.c`)**: A deployment tool that requires root privileges to install the daemon binary into `/bin/ft_shield` and create a systemd service for automatic startup. The daemon binary is embedded as a payload created by compiling the daemon code and using `xxd` to generate a header file that is then injected during installation.

2. **Daemon (`daemon/`)**: A background service that:
   - Runs invisibly in the background as a proper Unix daemon
   - Listens on port 4242 for incoming network connections
   - Requires authentication via a custom hashing mechanism
   - Provides remote access to system commands and information
   - Can spawn a root shell for authenticated users
   - Maintains connection logs and I/O statistics

The daemon disguises itself as a legitimate system service and automatically starts on boot, providing persistent backdoor access to the compromised system through network connections.

## Project Plan
- [x] Convert matt_daemon base project from CPP to C
- [x] Test the daemon and debug listening on port 4242
- [x] Add auth with custom hashing
- [x] Spawn a root shell using the daemon
- [x] Copy the compiled daemon into `/bin`
- [x] Create a `systemd` startup hook

## Usage
1. Compile the project:
``` shell
make
```
2. Start ft_shield (root privileges required):
``` shell
$ id
uid=0(root) gid=0(root) groups=0(root)
$ ./ft_shield
ffarkas-edupless
```
OR
``` shell
$ sudo ./ft_shield
ffarkas-edupless
```
3. Connect to the daemon using netcat:
``` shell
$ nc localhost 4242
>_ passcode:
Authenticated! Type '?' for help.
>_ 
```
