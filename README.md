# ![logo](https://cdn.rawgit.com/minils/EasySlideshow/master/svg/easyslideshow.svg "Logo Title Text 1") EasySlideshow
[![build status](https://travis-ci.org/minils/EasySlideshow.svg?branch=master "build status")](https://travis-ci.org/minils/EasySlideshow/builds)
[![Github All Releases](https://img.shields.io/github/downloads/minils/EasySlideshow/total.svg?maxAge=3600)](https://github.com/minils/EasySlideshow/releases)
[![Github All Releases](https://img.shields.io/github/release/minils/EasySlideshow.svg?maxAge=3600)](https://github.com/minils/EasySlideshow/releases)
[![license](https://img.shields.io/github/license/minils/EasySlideshow.svg?maxAge=3600)](https://github.com/minils/EasySlideshow/blob/master/LICENSE)

This is a very easy slideshow based on Qt.

It uses the provided folder(s) to display random images.

![Screenshot of main window](https://raw.githubusercontent.com/minils/EasySlideshow/master/screenshot1.png)
![Screenshot of settings](https://raw.githubusercontent.com/minils/EasySlideshow/master/screenshot2.png)

### Installation

Grab the binaries [here](https://github.com/minils/EasySlideshow/releases)

#### Windows
No installation required. Just use the statically build `.exe` file.

#### Linux
Either:
- Install the `.deb`-package

Or:
- Compile from source and run `make install`. This will install the binary to `/usr/bin/easyslideshow` as well as a `.desktop` file and an icon.

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
- [x] Implement right-click menu
- [x] Add details view
- [ ] Error when only one image is found
- [x] Settings dialog can be opened multiple times
- [x] Sometimes the slideshow is not paused correctly

### v0.5
- [ ] Rescan after x seconds when no images were found
- [ ] Save rotation of images
- [x] Integrate details into main window
- [x] Update settings view
- [ ] Translate
- [ ] Build flatpak
- [ ] Error when only one image is found

License
----

GNU General Public License 3.0
