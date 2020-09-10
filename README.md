<h1 align="center">oyster</h1>
<p align="center"><i>A new take on desktop shells</i></p>
<hr><p align="center">
  <img alt="Stars" src="https://img.shields.io/github/stars/buffet/oyster.svg?label=Stars&style=flat" />
  <a href="https://github.com/buffet/oyster/issues"><img alt="GitHub issues" src="https://img.shields.io/github/issues/buffet/oyster.svg"/></a>
  <a href="https://github.com/buffet/oyster/graphs/contributors"><img alt="GitHub contributors" src="https://img.shields.io/github/contributors/buffet/oyster"></a>
</p>

NOTE: This project is a work-in-progress and doesn't do much yet.


## Getting Started

The dependencies required are:

- [wlroots](https://github.com/swaywm/wlroots)
- meson (building)
- ninja (building)
- git (build, optional)

### Building

After cloning/downloading the project and ensuring all dependencies are installed, building is as easy as running

```
$ meson build
$ ninja -C build
```

Installing is accomplished with the following command:

```
# ninja -C build install
```
