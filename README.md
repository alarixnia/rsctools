rsctools
========

rsctools is a project to create tooling for various classic Jagex file
formats used in the 2001 MMORPG "RuneScape Classic" up until 2006.

libjag is a C language programming library for working with these file
formats, aiming to also document how these file formats work. A set of
tools is planned to allow direct manipulation and creation of .jag
archives from the Unix command line, similarly to "zip" and "unzip".

Goals
-----

A primary goal is to allow the wider RSC community to move away from
custom file formats in their projects, unifying around the file
formats used in the original clients. 

Supporting post-2009 file formats is not planned since no significant
changes to game content happened after this date.

License
-------

To encourage the adoption of open formats in the wider RSC community
with maximum license compatibility, all of this code is available
under the 2-clause BSD license.
