Egill - a Wayland compositor
============================
Egill is a library to implement window managers for Wayland in any language,
it's written in C and designed from the beginning to be easy to write bindings
using FFI.

What's wrong with Weston?
-------------------------
Nothing is wrong with Weston, but writing a shell for Weston requires you to be
able to produce a shared object using the module interface expected by Weston.

In addition you need bindings for the language you want to use into Wayland and
Weston, which isn't that easy to do.

In short you're limited to writing Weston shells in C or C++, or some weird
hacks.

Can't I write a compositor in any language anyway?
--------------------------------------------------
Yes, and no, it depends on few things, keep in mind you have to use headers
related to the kernel, and it's going to be a PITA to implement bindings for
them.

Egill abstracts those parts and makes it easy to bind.

What's up with the name?
------------------------
As the legend goes Egill is a marvelous archer brother of Wayland the Smith.

Yes, I know Wayland and Weston are cities in Massachusets, and I don't care.

License
-------
Egill is licensed under GPL, because I want the window managers to be free
software as well, if you don't want to release your window manager under GPL
you can always write your own compositor.
