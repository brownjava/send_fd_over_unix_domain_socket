# Sending a socket over a UNIX domain socket

This is a quickly-throw-together reference implementation of how to send a socket over a UNIX domain socket.  I threw this together pieced together from snippets of code online and looking at man page references because I wanted to understand how it worked, as it's similar to the basic way that isolating access to system resources works on many platforms (including macOS).

The makefile here generates two processes: client and server.  Run the server first.  Then run the client.  The server sends its standard output file descriptor to the client, then closes the connection to the client.  The client receives this and then reads from standard input and writes to this, causing the text to appear in the standard output of the server (probably a pseudo terminal of some sort).
