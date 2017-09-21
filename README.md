Fusuma Suid Wrapper
===================

This program runs [Fusuma](https://github.com/iberianpig/fusuma) with access to the input group even if the user lacks sudo and input permissions.

Fusuma needs access to the input group in order to work. The way Fusuma's readme recommends to do this is by making the user a member of input. However, making the user a member of the input group opens up a potential vulnerability. If the user is a member of input then all the programs they run will also be members of input. If a the user runs malware, or if malware infects a user program, it will have unrestricted access to the mouse and keyboard.

This wrapper solves the issue by using the suid file permissions flag. When and executable has it's suid set, it runs with the permissions of the executable's owner instead of the user who ran it. For example: if an executable in owned by root and has suid set, then whenever it is run, it runs as if it was run with `sudo`, even if the user who ran it lacks sudo privileges.

This allows for programs like `passwd` to do things that the user normally can't (like edit the `/etc/passwd` and `/etc/shadow` files). In this program it is used to add input to the process's groups. This allows Fusuma to listen to swiping events from the touchpad without using sudo and without risking a keylogger attack.

Installation
------------

```
git clone https://github.com/joshuatshaffer/fusuma-suid-wrapper
cd fusuma-suid-wrapper
sudo make install
```

Usage
-----

```
fusuma-s [-v | --verbose] [-c | --config=<path>]
```
All command arguments are passed straight through to Fusuma. This wrapper takes no arguments it self.

License
-------

This program is provided as free and open source software under the terms of the [MIT License](http://opensource.org/licenses/MIT).
