# ft_shield
This project will make you code a simple trojan horse.

## plan
- [x] convert matt_daemon base project from CPP to C
- [x] test the daemon and debug listening on port 4242
- [x] add auth with custom hashing
- [x] spawn a root shell using the daemon
- [ ] copy the compiled daemon into `/bin`
- [ ] create a startup hook (`init.d` or `systemd`)

## resources
- https://lloydrochester.com/post/c/unix-daemon-example/?utm_source=chatgpt.com
- https://akashrajpurohit.com/blog/running-a-service-in-the-background-with-systemv-in-linux-a-comprehensive-guide/?utm_source=chatgpt.com
- https://decal.ocf.berkeley.edu/archives/2010-spring/intermediate/08-lec-sysvinit.pdf?utm_source=chatgpt.com
