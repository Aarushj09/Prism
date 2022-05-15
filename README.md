# Prism

This program generates an n-sided prism and displays it using OpenGL. You can then perform various operations on it using the keyboard.

## Setup and Installation

Install these libraries (instructions for ubuntu):

```
sudo apt-get update
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```
To start the simulation, follow these commands first:
```
mkdir build
cd build
cmake ..
make -j
```

## Part A: Prism Generation

In order to generate the prism, while running the program, an input parameter `n` must be given as command-line input.
```bash
./app <n>
```
The program then generates an n-sided prism, which you can then perform various operations on. Each face of the prism is assigned a random colour, generated by randomizing the RGB values.

## Part B: Bringing the Scene to Life

### Flying Camera

Here, the prism stays in one position and we translate the camera along the world axes, while at all times ensuring that the camera faces the center of the prism.

<kbd>W</kbd> - Forward<br>
<kbd>S</kbd> - Backward<br>
<kbd>A</kbd> - Left<br>
<kbd>D</kbd> - Right<br>
<kbd>Q</kbd> - Up<br>
<kbd>E</kbd> - Down<br>

### Object Translation

The object gets translated, instead of the camera. This translation is done with respect to the camera axes.

<kbd>5</kbd> - Forward<br>
<kbd>6</kbd> - Backward<br>
<kbd>3</kbd> - Left<br>
<kbd>4</kbd> - Right<br>
<kbd>2</kbd> - Up<br>
<kbd>1</kbd> - Down<br>

### Prism Fashion Show

We set the camera to some predefined position and view the prism from there.

<kbd>8</kbd> - First position<br>
<kbd>9</kbd> - Second position<br>

### Prism, let's go for a spin!

On pressing <kbd>R</kbd>, the prism is rotated about an axis.

### Oh, how the turntables!

On pressing <kbd>T</kbd>, the camera is made to revolve around the prism, facing it at all times.