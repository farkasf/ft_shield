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

## project plan
- [x] convert matt_daemon base project from CPP to C
- [x] test the daemon and debug listening on port 4242
- [x] add auth with custom hashing
- [x] spawn a root shell using the daemon
- [x] copy the compiled daemon into `/bin`
- [x] create a `systemd` startup hook
