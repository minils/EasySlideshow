# ![logo](https://cdn.rawgit.com/minils/EasySlideshow/master/svg/icon.svg "Logo Title Text 1") EasySlideshow
[![build status](https://travis-ci.org/minils/EasySlideshow.svg?branch=master "build status")](https://travis-ci.org/minils/EasySlideshow/builds)
[![Github All Releases](https://img.shields.io/github/downloads/minils/EasySlideshow/total.svg?maxAge=3600)](https://github.com/minils/EasySlideshow/releases)
[![Github All Releases](https://img.shields.io/github/release/minils/EasySlideshow.svg?maxAge=3600)](https://github.com/minils/EasySlideshow/releases)
[![license](https://img.shields.io/github/license/minils/EasySlideshow.svg?maxAge=3600)](https://github.com/minils/EasySlideshow/blob/master/LICENSE)

This is a very easy slideshow based on Qt.

It uses the provided folder(s) to display random images.

### Version
0.2

### Installation

...

### Compile from source

To compile from source you need to have the following dependencies installed:
- Qt5
- qmake

```sh
git clone https://github.com/minils/EasySlideshow.git
cd EasySlideshow
mkdir build
cd build
export CONFIG=release
qmake ../
make
```

To compile for windows:
- mxe
- qt5
- qtsvg
```
git clone https://github.com/mxe/mxe.git
cd mxe
make qt5 qtsvg
# add mxe/usr/bin to your PATH variable
cd ..
git clone https://github.com/minils/EasySlideshow.git
cd EasySlideshow
mkdir build
cd build
i686-w64-mingw32.static-qmake-qt5 ../
make
```

### Known Issues

* none

### Todos

 - Fix bugs ...
 - Fav button
 - Increase probability for images with older access time
 - Read EXIF data and rotate if necessary
 - Add error message if no images were found
 - show "Lading..." when the folder config was changed and the slideshow is reloading

License
----

GNU General Public License 3.0
