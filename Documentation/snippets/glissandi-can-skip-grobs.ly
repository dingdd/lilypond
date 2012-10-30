% DO NOT EDIT this file manually; it is automatically
% generated from Documentation/snippets/new
% Make any changes in Documentation/snippets/new/
% and then run scripts/auxiliar/makelsr.py
%
% This file is in the public domain.
%% Note: this file works from version 2.16.0
\version "2.17.6"

\header {
  lsrtags = "expressive-marks, staff-notation, tweaks-and-overrides"
  doctitle = "Glissandi can skip grobs"
  texidoc = "@code{NoteColumn} grobs can be skipped over by glissandi."
} % begin verbatim


\relative c' {
  a2 \glissando
  \once \override NoteColumn.glissando-skip = ##t
  f''4 d,
}
