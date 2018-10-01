# Bertini_real: software for real algebraic sets

---

This software implements a numerical algorithm for decomposing real surfaces of arbitrary dimension, using Bertini as the computational engine.  

For documentation, please visit [bertinireal.com](https://bertinireal.com) and [doc.bertinireal.com](https://doc.bertinireal.com)


# Notes

Bertini_real is implemented in C++ and compiles against a number of libraries.  It has been developed and tested in Linux and OSX, and been tested in Cygwin.  It has never been compiled in Windows without another helper environment like MinGW or Cygwin.

The libraries against which Bertini_real compiles are:
* MPFR,
* GMP,
* Bertini, compiled from source into a library,
* Boost,
* MPI.

Bertini_real is built from source using standard methods, and requires the C++11 standard.



# Input and output formats

Input and output are through plain-text files written to disk.

## Input

 The necessary input files for Bertini_real are:

1. The same Bertini `input` file from which the user acquires a numerical irreducible decomposition;
2. The `witness_data` file created by Bertini after computing a numerical irreducible decomposition.


Many systems have multiple components.  Bertini_real therefore processes `witness_data` for each component, and offers the user a choice if there is one.  That is, if there is a single component of dimension one or two, Bertini_real will automatically and implicitly decompose that component.  If there are multiple components, Bertini_real will prompt the user for their choice.  They may only decompose a single dimension at a time, but they may elect to decompose as many components as they wish simultaneously, provided that they all have the same deflation sequence.  For example, the system in \S\ref{sec:curveexamplefrompaper}  has multiple components of dimension one, some of which are nonsingular, and several of which must be deflated.  The images in this example were produced by decomposing several components at the same time.


Optionally, the user may supply:

* a sphere file, consisting of the radius and center; and/or
* a projection file, prefaced by the number of variables, and containing the coefficients of the linear projections $\pi_i$.

The user indicates the names of the files using flags to the command line; e.g., the calling sequence is:
```bertini_real -sphere mysphere -pi myprojection```


## Output

There are two kinds of output:

1. temporary files generated by the Bertini parser and produced input files, and
2. the end-result deposited into a subfolder of the current working directory.  

The temporaries are unavoidable but inconsequential, so they are not  discussed here.  The writing of files is incremental, and happens after each major stage, so that if the program fails for some reason, the user gets the most recent good state as a parsable set of data.

### Files common to all decompositions

* `vertex_set`
The number of vertices, the number of projections, the number of natural variables, and the number of input files referenced.
Coefficients of the $\pi_i$ projections.  The input file names.  Then the vertices, appearing as the number of variables, the coordinates, the number of projection values stored, the projection values, and the integer index of the filename for which the point was originally found and committed.
* `input` file
Copied verbatim into the folder.  Needed for sampling and future reference.
* original `witness_data` file
Copied verbatim into the folder.  This file is part of the generating data, and so is deemed part of the decomposition.
* `decomp` file
The name of the input file, the number of variables, and the dimension of the decomposition.  All of the $\pi_i$ projections used.  The patches.  The center and radius of the sphere.


### Files specific to curve decompositions


* `E.edge`
Contains all edges of the curve, listed as integers into `vertex_set`, being line-delimited as `left mid right`.


### Files specific to surface decompositions


* `S.surf`
The number of faces, the number of critical slices, and the number of midslices, along with the number of singular curves, their `multiplicities', and the number of each multiplicity.
* All curve sub-decompositions, written to their own sub-folders  critical curve, sphere curve, singular curves, and all mid and critslices.


Full documentation of input and output for Bertini_real is in [the pdf manual](https://bertinireal.com/resources/bertini_real_manual.pdf), and detailed using Doxygen, and is available at [the Bertini_real documentation webpage](https://doc.bertinireal.com).
