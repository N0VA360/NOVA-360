# NOVA-360

Futuristic independent homebrew dashboard for Xbox 360 RGH.

Built with [xsdk](https://github.com/xenosworks/xsdk) and [Dear ImGui (imgui-xenon)](https://github.com/xenosworks/imgui-xenon).

## Status

ImGui-based shell with sidebar navigation (Home / Library / Explorer / System / Customize / Profile / Media).

See [docs/DEVELOPMENT_STATUS.md](docs/DEVELOPMENT_STATUS.md) for the roadmap.

## Build

The GitHub Actions workflow builds the XEX automatically.

Locally (after building xsdk and checking out imgui-xenon into `third_party/imgui-xenon`):

```bash
cmake -S . -B build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=<path-to-xsdk>/cmake/xsdk.cmake \
  -DXSDK_TOOLCHAIN=<path-to-xsdk-gcc>
cmake --build build
```

## Controls

- D-pad / Left stick - navigate
- A - select / activate
- B - back (inside ImGui)
- Back - exit to dashboard
