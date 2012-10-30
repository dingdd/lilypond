% DO NOT EDIT this file manually; it is automatically
% generated from Documentation/snippets/new
% Make any changes in Documentation/snippets/new/
% and then run scripts/auxiliar/makelsr.py
%
% This file is in the public domain.
%% Note: this file works from version 2.16.0
\version "2.17.6"

\header {
  lsrtags = "rhythms"

  texidoc = "
The global defaults for grace notes are stored in the identifiers
@code{startGraceMusic}, @code{stopGraceMusic},
@code{startAcciaccaturaMusic}, @code{stopAcciaccaturaMusic},
@code{startAppoggiaturaMusic} and @code{stopAppoggiaturaMusic}, which
are defined in the file @code{ly/grace-init.ly}.  By redefining them
other effects may be obtained.

"
  doctitle = "Redefining grace note global defaults"
} % begin verbatim


startAcciaccaturaMusic = {
  <>(
  \override Flag.stroke-style = #"grace"
  \slurDashed
}

stopAcciaccaturaMusic = {
  \revert Flag.stroke-style
  \slurSolid
  <>)
}

\relative c'' {
  \acciaccatura d8 c1
}
