This come from a seminar on Bayesian Networks by Marco Scutari.
His "bnlearn" R package is provided too -although not needed for the execution-,
along with some documentation.

INSTALL BNLEARN
===============
You don't need the provided `bnlearn` source code, just type

    install.packages("bnlearn")

INSTALL GRAPHVIZ
================
Graphviz cannot be found anymore in CRAN library, so add this repo

    source("http://bioconductor.org/biocLite.R")
    biocLite("Rgraphviz")
