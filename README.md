# Csg
[![Build Status](https://travis-ci.com/vpuh1/csg.svg?branch=main)](https://travis-ci.com/vpuh1/csg)  
Static sites generator written in C. 
### Description
Csg is a very simple static sites generator. It converts files written in Markdown to HTML using `pandoc` and generates main page
with list of articles.
### Dependencies
`pandoc`
### Usage
```
csg src dst
```
where `src` is a source directory with Markdown files and `dst` is a destination directory, respectively.
### Installation
Install `pandoc`, clone this repo, after that.  
```
cd src && make && sudo make install
```
You can find your csg files under ~/.csg  
See `man csg` for more info.
