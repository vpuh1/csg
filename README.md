# Csg
Static sites generator written in C.
### Description
Csg is a very simple static sites generator. It converts files written in Markdown to HTML using `pandoc`.
### Usage
```
csg src dst
```
where `src` is a source directory with Markdown files and `dst` is a destination directory, respectively.
### Installation
Clone this repo, after that
```
make && sudo make install
```
If you want to use csg outside of the repo, you need to change `confloc` to absolute path in `src/config.h`.
