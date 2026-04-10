Note: Certain details given in this document might or might not be out-of-date.

How to identify code changes (and what's this DN3DREV thing)?
-------------------------------------------------------------

Check out GAMEVER.H. Basically, for each EXE being built, the
macro `APPVER_EXEDEF` should be defined accordingly. For instance,
when building NAM10, `APPVER_EXEDEF` is defined to be NAM10.

Note that only C sources (and not ASM) are covered by the above,
although there are no actual ASM sources in the Duke3D game code,
so it's mostly irrelevant.

Other than GAMEVER.H, the `APPVER_EXEDEF` macro is not used *anywhere*.
Instead, other macros are used. Some of them are old macros added to
control specific features, like NAM, WW2 and EDUKE. As of this release,
the latter three macros are defined by passing arguments to the compiler,
as originally done by Matt Saettler and done here with `APPVER_EXEDEF`.

Any new macro may also be introduced if useful. Note that the WW2GI macro
does *not* have the same meaning as WW2 - apparently it's useless.
WW2 is generally the macro originally used for "WW2GI and later",
albeit not always in this way.

`APPVER_DN3DREV` is defined in all builds, with different values. It is
intended to represent a revision of development of the Duke Nukem 3D
codebase, as well as the source addendum for the Build Editor (ASTUB.C).
Usually, this revision value is based on some evidenced
date (e.g., any date taken from Matt Saettler's EDuke changelog),
or a *guessed* date (say, an original modification date of the EXE).
Any other case is also a possibility.

These are two good reasons for using DN3DREV as described above, referring
to similar work done for Wolfenstein 3D EXEs (built with Borland C++):

- WL1AP12 and WL6AP11 share the same code revision. However, WL1AP11
is of an earlier revision. Thus, the usage of WOLFREV can be
less confusing.
- WOLFREV is a good way to describe the early March 1992 build. While
it's commonly called just "alpha" or "beta", `GAMEVER_WOLFREV`
gives us a more explicit description.

Is looking for `#if (APPVER_DN3DREV <= AV_DR_...) (or >)` sufficient?
---------------------------------------------------------------------

Nope!

Examples from Wolf3D/SOD:

For a project with `GAMEVER_WOLFREV == GV_WR_SDMFG10`,
the condition `GAMEVER_WOLFREV <= GV_WR_SDMFG10` holds.
However, so does `GAMEVER_WOLFREV <= GV_WR_WJ6IM14`,
and also `GAMEVER_WOLFREV > GV_WR_WL1AP10`.
Furthermore, `PML_StartupXMS (ID_PM.C)` has two mentions of a bug fix
dating 10/8/92, for which the `GAMEVER_WOLFREV` test was chosen
appropriately. The exact range of `WOLFREV` values from this test
is not based on any specific build/release of an EXE.

What is this based on
---------------------

This codebase was originally based on the EDuke 2.00.21 sources
as released by Matt Saettler in 2003, while also using a few bits of
the EDuke 2.1.41 sources. As for the Duke Nukem 3D sources released
by 3D Realms beforehand, they were mostly used for reference,
say for attempting to recreate the behaviors of Duke3D v1.5.
Later, the same sources (as released by 3D Realms) were
used in order to recreate the behaviors of Duke3D v1.4,
before finally bringing the work to the usual repository.
Apart from actual C sources, usage of the EDuke 2.00.21 source tree
included the MAKEFILE, as well as the following binary (non-source)
OBJ/LIB files (also turning out to be present in 3DR's sources),
at least for Duke3D v1.5 and the later releases:

- The Build Engine (A.OBJ, CACHE1D.OBJ, ENGINE.OBJ and MMULTI.OBJ).
- The Apogee Sound System (`AUDIO_WF.LIB`).
- The MACT library (MACT386.LIB).
- BUTWCD4.LIB, related to the Total Entertainment Network (TEN) service.

A few missing headers from the MACT library were copied from 3DR's sources.
Their MAKEFILE.LNK file was also used as a base.

As a side-note on MAKEFILE.LNK, using the "file" directive even for the
libraries, and further issuing two "segment" directives, were all essential
for reproducing exactly the original EXEs as built by Matt.
It later turned out that omitting "system dos4g" was an alternative
to the aforementioned "segment" directive changes.

The idea to look into the EDuke 2.00.21 sources
(rather than just 3DR's sources) came from Evan Ramos.
Further thanks shall be sent to Barry Duncan, Jarmo Kylmäaho
and Richard Gobeille for any assistance and/or other way of showing support.
Special thanks go to Ken Silverman for his consent to release additional
Build engine sources under the same terms as for his 2000 release.
This also covers sources for the revision of ENGINE.OBJ used
in Shadow Warrior versions 1.1 and 1.2, as well as differing
1996 revisions, an early 1995 revision, and possibly also other files.
Additional credits go to Nuke.YKT for using the aforementioned SW revision
of the ENGINE.OBJ code in order to recreate the code used in Duke3D 1.5,
and later assisting with the recreation of the Duke3D 1.4 revision.
It's probably also a good chance to send my thanks to various other members
of the Duke4.net community for their great support during all of these years!

Interestingly, a short while after recreating Duke3D 1.5's code,
it was Nuke.YKT who suggested to check the "extras" directory from
3DR's released Duke3D source archive of 2003. Ignoring unused code
like SE40.C, it had turned out to indeed match 1.5's game code.
That said, Matt's initial release of EDuke 2.0 sources still had its use
for the included BUILD.H and MAKEFILE files, which were different, but
probably closer to the files used for the released 1.5 executable
(e.g., Watcom optimizations).

Later on, code for the Build Editor as distributed with Duke3D 1.4 and 1.5 was
recreated, followed by 1.3d. The only source file which isn't a direct part
of the Build Engine was ASTUB.C. A version of it was GPLed, which turned out
to match 1.3d, except for a bit different date in the version string,
as well as adaptations for compatibility with Duke3D 1.5's engine
(the latter being done for the GPLed release of 2003).
Almost all of 1.4's ASTUB.C code had to be re-added from scratch. In a few
cases of having differences in the output from the original, Shadow Warrior's
JNSTUB.C was very useful. It might actually be the closest to any known
source code which is still including code added for Duke3D 1.4's Build Editor.

When it comes to the engine, it turned out that the editor for Duke3D 1.4-1.5
uses a bit older engine than the 1.4 game EXE. As for 1.3d's editor, this
required greater changes, a great deal of them being done by Nuke.YKT
earlier (even before recreating 1.5's ENGINE.OBJ).

Later in 2026, thanks to more Build Engine revisions sent by Ken Silverman
and made public (mainly sources released in December 2024), the accuracy
of recreation of ENGINE.OBJ from 1.3d's editor was further improved,
essentially becoming binary-perfect in the engine.

How was the makefile (and a bit more) modified from the original
----------------------------------------------------------------

Compared to the EDuke 2.00.21 sources as released by Matt, there aren't
that many changes, although it's true that large chunks of code were
possibly, originally, added in-between certain games/EXEs.
There is a small amount of changes between versions 2.00.21 and 2.00.23,
backported from one of the EDuke 2.1 source archives that was released.
