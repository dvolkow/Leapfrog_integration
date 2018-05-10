# Leapfrog N-Body Integrator
Here is soft for Leapfrog integration for numerically integration differential equations.

## Why?
With it you can get as picture as this:
![N Bodies Simulation with my realization of this integrator](https://github.com/dvolkow/Leapfrog_integration/blob/master/saves/line_start.gif)

Or more bodies:
![N Bodies Simulation with my realization of this integrator](https://github.com/dvolkow/Leapfrog_integration/blob/master/saves/more.gif)

But time to time you should wait little for result. Do you try integrate thousand or more bodies? Try with it!

<Picture with success integrate 1000 bodies>

## Requirements
- Any Linux OS (already include libc and gcc)
- MPFR + GMP libraries. Please, install it.

## Install IT
Step by step:

```git clone https://github.com/dvolkow/Leapfrog_integration```

```cd Leapfrog_integration```

```./install.sh```

Congratulations!


## Documentation and usage

Primary instructions can be obtained with ```man leapfrog``` after success installation (or man ./mp/leapfrog.1 if your system not supported standard pathes for man pages)

Simple run ```leapfrog -h```, or ```leapfrog``` with incorrect parameters. May be you will want to see into source, where is more useful information. Also, here must be sample's directory (not gifs, but self-test sctipts).
