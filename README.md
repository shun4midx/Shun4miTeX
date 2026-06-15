# Shun4miTeX
Shun's Discord bot for rendering LaTeX and natural math notation, into formal TeX-like images as outputs. Like most of Shun's projects, it's written in C++.

It's canonically pronounced as "Shunami TeX" or "Tsunami TeX".

Currently, the core renderer works from the command line: it writes a temporary LaTeX file, renders it inside a Dockerized TeX Live environment, converts the result to PNG (with white text on a black background), and returns the generated image. Discord bot integration is planned for the future.

# Current Status

* [x] C++ CLI renderer
* [x] Dockerized LaTeX rendering
* [x] PNG output with dark background
* [x] Basic math packages and `tcolorbox` support including some other minimal shortcuts [I use](https://github.com/shun4midx/LaTeX-Template)
* [ ] Discord bot message command
* [ ] Possible slash command support
* [ ] Natural math notation parser (Start with roughly high school and beginning of college math)

# Requirements

* Docker
* C++17 compiler
* TeX packages are installed inside the Docker image, so a local TeX Live installation is not required for rendering.

# Usage

## Clone the Repo
First, we clone this repo before proceeding
```bash
git clone https://github.com/shun4midx/Shun4miTeX
cd Shun4miTeX
```

## Build Docker Image

```bash
docker build -t shun4mitex-renderer .
```

## Build CLI Test

```bash
g++ -std=c++17 src/cli.cpp src/latex_render.cpp -o cli
```

## CLI Test Usage

```bash
./cli 'This is a fraction $\frac{x^2+1}{x-3}$'
```

Example with a custom color box based off of [Shun's usually used minimal shortcuts](https://github.com/shun4midx/latex-template):

```bash
./cli '\pinkbox{Note}{Hello from Shun4miTeX}'
```

Generated render jobs are stored under `jobs/`.

# Notes

This project is still in early development. The renderer is currently usable through the CLI, while the Discord bot layer is still being implemented.
