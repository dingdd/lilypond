
\version "2.19.20"

\header {
  lsrtags = "rhythms, tweaks-and-overrides"

  texidoc = "
If there are ten or fewer measures of rests, a series of longa and
breve rests (called in German @qq{Kirchenpausen} - church rests) is
printed within the staff; otherwise a simple line is shown. This
default number of ten may be changed by overriding the
@code{expand-limit} property.

"
  doctitle = "Changing form of multi-measure rests"
}

\relative c'' {
  \compressMMRests {
    R1*2 | R1*5 | R1*9
    \override MultiMeasureRest.expand-limit = #3
    R1*2 | R1*5 | R1*9
  }
}
