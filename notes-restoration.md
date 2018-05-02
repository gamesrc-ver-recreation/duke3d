*** WARNING: DO NOT TRY TOO HARD TO BUILD ANY OF THE ORIGINAL EXECUTABLES! ***

Please remember that any little difference, not matter how small it is,
can lead to a vastly different EXE layout. This includes differences in:  
- The development tools (or parts of such); For instance, a compiler, a linker,
an assembler, or even a support library or header. A version number is not
a promise for having the exact tool used to reproduce some executable.
- The order in which source code files are listed in a given project or
makefile.
- Project settings.
- Source file specific settings in such a project.
- The order in which source code files are passed to a linker.
- Any modification to a source code file (especially a header file).
- More than any of the above.

Following the warning, a description of the ways in which the executables were
reproduced is given.

With the right tools, this patched codebase can be used to reproduce any
of the executables coming from the following original releases:  
- NAM (NAPALM) Full Version 1.0 (same EXE for NAM and NAPALM).
- WW2GI Full Version 1.0.
- Enhanced Duke (EDuke) 2.00.23 (mostly known as EDuke 2.0, not to be
confused with the released sources, which appear to be v2.00.21).

The MAKEFILE bundled with the EDuke 2.00.21 sources (the initial release)
was used as a base. You shall *not* call "wmake" on
your own anymore. Instead, use BUILD.BAT.

List of releases by directory names
-----------------------------------

- NAM10: NAM (NAPALM) Full Version 1.0.
- WW2GI10: WW2GI Full Version 1.0.
- EDK20023: Enhanced Duke 2.00.23.

How to identify code changes (and what's this DN3DREV thing)?
-------------------------------------------------------------

Check out GAMEVER.H. Basically, for each EXE being built, the
macro APPVER_EXEDEF should be defined accordingly. For instance,
when building NAM10, APPVER_EXEDEF is defined to be NAM10.

Note that only C sources (and not ASM) are covered by the above,
although there are no actual ASM sources in the Duke3D game code,
so it's mostly irrelevant.

Other than GAMEVER.H, the APPVER_EXEDEF macro is not used *anywhere*.
Instead, other macros are used. Some of them are old macros added to
control specific features, like NAM, WW2 and EDUKE. As of this release,
the latter three macros are defined by passing arguments to the compiler,
as originally done by Matt Saettler and done here with APPVER_EXEDEF.

Any new macro may also be introduced if useful. Note that the WW2GI macro
does *not* have the same meaning as WW2 - apparently it's useless.
WW2 is generally the macro originally used for "WW2GI and later",
albeit not always in this way.

APPVER_DN3DREV is defined in all builds, with different values. It is
intended to represent a revision of development of the Duke Nukem 3D
codebase. Usually, this revision value is based on some evidenced
date (e.g., any date taken from Matt Saettler's EDuke changelog),
or a *guessed* date (say, an original modification date of the EXE).
Any other case is also a possibility.

These are two good reasons for using DN3DREV as described above, referring
to similar work done for Wolfenstein 3D EXEs (built with Borland C++):  
- WL1AP12 and WL6AP11 share the same code revision. However, WL1AP11
is of an earlier revision. Thus, the usage of WOLFREV can be
less confusing.
- WOLFREV is a good way to describe the early March 1992 build. While
it's commonly called just "alpha" or "beta", GAMEVER_WOLFREV
gives us a more explicit description.

Is looking for "#if (APPVER_DN3DREV <= AV_DR_...)" (or >) sufficient?
---------------------------------------------------------------------

Nope!

Examples from Wolf3D/SOD:

For a project with GAMEVER_WOLFREV == GV_WR_SDMFG10,
the condition GAMEVER_WOLFREV <= GV_WR_SDMFG10 holds.
However, so does GAMEVER_WOLFREV <= GV_WR_WJ6IM14,
and also GAMEVER_WOLFREV > GV_WR_WL1AP10.
Furthermore, PML_StartupXMS (ID_PM.C) has two mentions of a bug fix
dating 10/8/92, for which the GAMEVER_WOLFREV test was chosen
appropriately. The exact range of WOLFREV values from this test
is not based on any specific build/release of an EXE.

What is this based on
---------------------

This codebase is based on the EDuke 2.00.21 sources as originally released
by Matt Saettler on 2003. Apart from actual C sources, this includes the
MAKEFILE, as well as the following binary (non-source) OBJ/LIB files:  
- The Build Engine (A.OBJ, CACHE1D.OBJ, ENGINE.OBJ and MMULTI.OBJ).
- The Apogee Sound System (AUDIO_WF.LIB).
- The MACT library (MACT386.LIB).
- The Total Entertainment Network (TEN) library (BUTWCD4.LIB).

A few missing headers from the MACT library were copied from
the Duke Nukem 3D sources as released by 3D Realms earlier
on 2003. Their MAKEFILE.LNK file was also used as a base.

As a side-note on MAKEFILE.LNK, using the "file" directive even for the
libraries, and further issuing the "segment" directives, were all essential
for reproducing exactly the original EXEs as built by Matt.

How was the makefile (and a bit more) modified from the original
----------------------------------------------------------------

Compared to the EDuke 2.00.21 sources as released by Matt, there aren't
that many changes, although it's true that large chunks of code were
possibly, originally, added in-between certain games/EXEs.
There is a small amount of changes between versions 2.00.21 and 2.00.23,
backported from one of the EDuke 2.1 source archives that was released.

What is *not* included
----------------------

There are no sources for the MACT library (MACT386.LIB), as well
as the Total Entertainment Network i.e., TEN library (BUTWCD4.LIB).

In addition, while the Build Engine and the Apogee Sound System were
open sourced, currently the binary files from the EDuke 2.00.21 sources are
used. Based on the original modification date of Dec 9 1996 that all binary
files share, it is currently *guessed* (but *not* proven) that the exact
same binary library code was used for Duke Nukem 3D: Atomic Edition v1.5.

Building each EXE
=================

Required tool:  
- Watcom C 10.6a (and no other version), for NAM, WW2GI and EDuke.

Notes before trying to build anything:  
- This may depend on luck. Maybe you'll get a bit different EXE.
- You may use BUILD.BAT to build each EXE, and CLEAN.BAT to clean
any previously built object and executable files.

Building any of the EXEs
------------------------

1. Use BUILD.BAT, passing the corresponding output directory name
as an argument (e.g., NAM10 for NAM Full Version 1.0).
2. Hopefully you should get exactly the original EXE.
