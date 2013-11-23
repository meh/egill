Egill - Wayland compositor as a library
=======================================
Egill is a library to implement window managers for Wayland in any language,
it's written in C and designed from the beginning to be easy to write bindings
using FFI.

What's wrong with Weston?
-------------------------
Nothing is wrong with Weston, but writing a shell for Weston requires you to be
able to produce a shared object using the module interface expected by Weston.

In addition you need bindings for the language you want to use into Wayland and
Weston, which isn't that easy to do.

In short you're limited to writing Weston shells in C or C++.

License
-------
Egill is licensed under GPL, because I want the window managers to be free
software as well, if you don't want to release your window manager under GPL
you can always write your own compositor.
