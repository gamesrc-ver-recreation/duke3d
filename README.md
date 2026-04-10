Modified Duke Nukem 3D sources with multiple reconstructed versions
===================================================================

This source tree covers a modification of the open-source release
of Enhanced Duke (EDuke) 2.00.21 for DOS, the main goal being a
reconstruction of sources matching multiple released binaries.

In case you try making binaries from this codebase, the usual caveat is
that even with what appear to be the right tools, you might not get an exact
binary as it was released back in the days, for multiple technical reasons.

What is *not* included
----------------------

There are no sources for the MACT library, i.e., MACT386.LIB, and also not
for a library related to Total Entertainment Network (TEN), BUTWCD4.LIB.
That said, the included BUTWCD4.LIB and MACT386.LIB files
(from the GPLed Duke3D and EDuke 2.00.21 sources as released in 2003)
should match Duke3D 1.4-1.5.

Regarding the Build Engine and the Apogee Sound System, these were
indeed open-sourced. That said, the binary builds of them bundled with the
aforementioned Duke3D and EDuke 2.00.21 sources were found to match 1.5
and later binaries (including EDuke 2.00.23), so they're also used here.
Different CACHE1D.OBJ and ENGINE.OBJ files are used for 1.4.
Regarding Duke3D's Build Editor, only A.OBJ is re-used in the case
of 1.4, while none of these objects is re-used as-is for 1.3d's editor.

List of covered versions
------------------------

With the right tools, this patched codebase can be used to reproduce any
of the executables coming from the following original releases, with some
caveats for the EXEs differing from NAM, WWII GI and EDuke 2.00.23's:

- BUILD.EXE for Duke Nukem 3D v1.3d.
- BUILD.EXE for Duke Nukem 3D - Atomic Edition v1.4 (also re-used for 1.5).
- Duke Nukem 3D: Atomic Edition versions 1.4-and 1.5.
- NAM (NAPALM) Full Version 1.0 (same EXE for NAM and NAPALM).
- World War II GI (WWII GI) Full Version 1.0.
- Enhanced Duke (EDuke) 2.00.23 (mostly known as EDuke 2.0,
not to be confused with the released sources appearing to match v2.00.21).

List of releases by directory names
-----------------------------------

- DN3D13B: Duke Nukem 3D v1.3d Build Editor.
- DN3D14B: Duke Nukem 3D: Atomic Edition v1.4 Build Editor.
- DN3D14G: Duke Nukem 3D: Atomic Edition v1.4 (the game).
- DN3D15: Duke Nukem 3D: Atomic Edition v1.5.
- NAM10: NAM (NAPALM) Full Version 1.0.
- WW2GI10: WWII GI Full Version 1.0.
- EDK20023: Enhanced Duke 2.00.23.

Technical details of the original EXEs (rather than any recreated one)
----------------------------------------------------------------------

Unless NAM or WWII GI is explicitly stated, the game in question is Duke3D.

|        Version       |  Bytes  |               MD5                |  CRC-32  |
|----------------------|---------|----------------------------------|----------|
| Duke3D 1.3d's editor |  526957 | 2f663fa4fabddcd808dd4ab73508c5bc | f7f2f14f |
| Duke3D 1.4's editor  |  588681 | ad529c7de3399fa3cd47f805f770f231 | fab19963 |
|  Atomic Edition 1.4  | 1240151 | ed82781e5f741987cb5780b55960cba3 | 27439fcd |
|  Atomic Edition 1.5  | 1246231 | 7f92da0bd971a64a1cfde828eb98899b | 0167875d |
|          NAM         | 1040467 | aa7180788372597da4fef8db1d7fe7ad | 28591a18 |
|        WWII GI       | 1078643 | bb005ea7366bd0541046541db457e46e | 4ba75278 |
|     EDuke 2.00.23    | 1142195 | dc7797ba3a0bf850383d1e972bfc53d0 | 329c48c7 |

Prerequisites for building each EXE
-----------------------------------

Required tools:

- For Duke3D versions 1.4-1.5 i.e., Duke Nukem 3D: Atomic Edition 1.4-1.5,
as well as the Build Editor as originally distributed with Duke3D 1.3d and
1.4/1.5: Watcom C 10.0 (and no other version, not even 10.0a or 10.0b).
- For any other EXE: Watcom C 10.6 (and no other version).
- (Optional) For Duke3D 1.4-1.5: DOS/4GW Professional Bind Utility version 1.3,
along with DOS/4GW Professional Protected Mode Run Time version 1.96.

Additionally, for Duke3D 1.4 and the Build Editor:
- You'll also need to build matching CACHE1D.OBJ and ENGINE.OBJ files
under the ..\build subdirectory. For the Build Editor,
BUILD.OBJ is further required.
- In the case of Duke3D 1.3d's Build Editor, you also need to build A.OBJ.

Notes before trying to build
----------------------------

- This may depend on luck. In the cases of the Build Editor, as well
as Duke Nukem 3D: Atomic Edition versions 1.4 and 1.5, you *will* get
different EXEs, especially if you don't use DOS/4GW Professional
(but also if you do). Even with other EXEs, luck may have its part.
- For Duke3D 1.4's Build Editor, the output Keys3d function is expected
to have a few unresolved differences from the original, but the code
should be equivalent in behaviors.
- Further note that any customized patch, like copy protection removal,
shall *not* be applied here. This may lead to an additional
minor difference of one byte, if not much more.
- You may use DOBUILD.BAT to build each EXE, and DOCLEAN.BAT to clean
any previously built object and executable files.

Building Duke Nukem 3D: Atomic Edition 1.4/1.5, or 1.3d/1.4's Build Editor
==========================================================================

1. If you want to build the 1.4 game EXE, don't forget to build the matching
CACHE1D.OBJ and ENGINE.OBJ files under the "build" repository first.
2. For the Build Editor, build the matching BUILD.OBJ, CACHE1D.OBJ and
ENGINE.OBJ files under "build", along with A.OBJ if this is 1.3d's editor.
3. Use DOBUILD.BAT, selecting DN3D13B, DN3D14B, DN3D14G or DN3D15 as the
output directory name, depending on the EXE you want to build.
4. Hopefully you should get an EXE essentially behaving like the original.

Expected differences from the original:

- A few unused gaps, mostly between C string literals, seem to be filled with
values depending on the environment while running the Watcom C compiler
(e.g., the exact contents of each compilation unit). This seems to be
related to Watcom C v10.0(a/b), and less to 10.6.
- The created BUILD.EXE or GAME.EXE file will require an external DOS4GW EXE
(or compatible). You may optionally use DOS/4GW Professional to bind
its loader to the EXE, but chances are there'll still be an *additional*
difference of, say, 6 bytes from the original EXE, if not more.
- As stated above, for the Duke3D 1.4 Build Editor, ASTUB.C:Keys3d is
expected to have technical differences from the code in the original EXE.

Building the NAM, WWII GI or EDuke EXE
======================================

1. Use DOBUILD.BAT, selecting the desired directory name.
2. Hopefully, you should get exactly the original EXE.
