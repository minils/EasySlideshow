# ![logo](https://cdn.rawgit.com/minils/EasySlideshow/master/svg/easyslideshow.svg "Logo Title Text 1") EasySlideshow
[![build status](https://travis-ci.org/minils/EasySlideshow.svg?branch=master "build status")](https://travis-ci.org/minils/EasySlideshow/builds)
[![Github All Releases](https://img.shields.io/github/downloads/minils/EasySlideshow/total.svg?maxAge=3600)](https://github.com/minils/EasySlideshow/releases)
[![Github All Releases](https://img.shields.io/github/release/minils/EasySlideshow.svg?maxAge=3600)](https://github.com/minils/EasySlideshow/releases)
[![license](https://img.shields.io/github/license/minils/EasySlideshow.svg?maxAge=3600)](https://github.com/minils/EasySlideshow/blob/master/LICENSE)

This is a very easy slideshow based on Qt.

It uses the provided folder(s) to display random images.

### Version
0.3.2

### Installation

#### Windows
No installation required. Just use the statically build `.exe` file.

#### Linux
Either:
- Install the `.deb`-package

Or:
- Compile from source and run `make install`. This will install the binary to `/usr/easyslideshow` as well as a `.desktop` file and an icon.

### Compile from source

#### Linux
To compile from source you need to have the following dependencies installed:
- Qt5
- libqt-svg
- qmake

```sh
git clone https://github.com/minils/EasySlideshow.git
cd EasySlideshow
mkdir build
cd build
export CONFIG=release
qmake ../EasySlideshow.pro
make
```

#### Crosscompile for Windows
Required:
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
i686-w64-mingw32.static-qmake-qt5 ../EasySlideshow.pro
make
```

### Todos

- [x] Fix bugs
- [ ] Fav button?
- [x] Button to rotate images
- [ ] Increase probability for images with older access time
- [x] Save size of window after exit and restore it before start
- [x] Build deb-package
- [ ] Build rpm-package
- [x] Build for windows
- [ ] Translate into more languages
- [ ] Implement right-click menu

License
----

GNU General Public License 3.0
