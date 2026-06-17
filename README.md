# Shun4miTeX
Shun's Discord bot for rendering LaTeX and natural math notation, into formal TeX-like images as outputs. Like most of Shun's projects, it's written in C++.

It's canonically pronounced as "Shunami TeX" or "Tsunami TeX".

Currently, the core renderer works from the command line: it writes a temporary LaTeX file, renders it inside a Dockerized TeX Live environment, converts the result to PNG (with white text on a black background), and returns the generated image. Then, this backend process is wrapped via a Discord bot.

# Current Status

* [x] C++ CLI renderer
* [x] Dockerized LaTeX rendering
* [x] PNG output with dark background
* [x] Basic math packages and `tcolorbox` support including some other minimal shortcuts [I use](https://github.com/shun4midx/LaTeX-Template)
* [x] Discord bot message command
* [x] Slash command support
* [x] Natural math notation parser (Start with roughly high school and beginning of college math)
* [ ] Documented list of supported natural math notation

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

## Hosting the Bot
1. Clone the DPP repo to build from source with CMake (If you don't have it, please check out how to install CMake):
```cmd
git clone https://github.com/brainboxdotcc/DPP
```

2. Create a `.env` file, and type in the following, but replace it with your relevant details without the <>:
```env
BOT_TOKEN=<Your bot token>
MESSAGE_PERMS=<Your username>,<Person 2 with message perms' username>,<etc>
BOT_USERNAME="<Your bot username including the discriminator>"
CONFLICTING_BOTS="<Any conflicting bot usernames including the discriminator>,<like this>"
```

3. Type the following CMake commands (Make sure to install `pkg-config` first if you don't have it):
```cmd
mkdir build && cd build
cmake ..
cmake --build . --target shun4mitex -j
./shun4mitex
```

And your bot should run properly, if given the correct perms!

# CLI Test Usage
## Build CLI Test

```bash
g++ -std=c++17 src/cli.cpp src/latex_render/latex_render.cpp -o cli
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

# Credit
DPP is directly found via [this](https://github.com/brainboxdotcc/DPP) repo, and is used for C++ Discord bot coding.