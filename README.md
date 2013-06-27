point-n-shoot
=============
Point and shoot is a simple but exciting shooting game.

Prerequisitions
=============
In Ubuntu Linux, install following packages first
```bash
$ sudo apt-get install build-essential git libgtk-3-dev cmake
```

Get the source code
=============
```bash
$ git clone git://github.com/derekdai/point-n-shoot.git
```

How to build?
=============
Output of source tree build is recommended way to build point-n-shoot.
```bash
$ mkdir point-n-shoot_build
$ cd point-n-shoot_build
$ ../point-n-shoot_build/autogen.sh
$ make
```

Run point-n-shoot
```bash
$ ./point-n-shoot
```
