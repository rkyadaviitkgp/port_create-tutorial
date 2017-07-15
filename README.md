# port_create-tutorial
event handling on solaris platform
we can monitor mutiple set of polled file descriptors using dev/poll driver on solaris. dev/poll is depricated now, and for simmilar functinality, we can use port create.
This is a demontration of Port create where a server create a port file descriptor, registers different file descriptor for read event and waits for events on those descriptor using port_getn function. 
we have also demontrated that port fd is itself pollable. This is just a tutorial for begineers.
