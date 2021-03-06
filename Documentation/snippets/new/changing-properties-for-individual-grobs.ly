\version "2.19.24"

\header {
  lsrtags = "really-cool, scheme-language, tweaks-and-overrides"

  texidoc = "
The @code{\\applyOutput} command allows the tuning of any layout
object, in any context. It requires a Scheme function with three
arguments.

"
  doctitle = "Changing properties for individual grobs"
}
#(define (mc-squared grob grob-origin context)
   (let ((sp (ly:grob-property grob 'staff-position)))
     (ly:grob-set-property!
      grob 'stencil
      (grob-interpret-markup grob
			     #{ \markup \lower #0.5
				#(case sp
				   ((-5) "m")
				   ((-3) "c ")
				   ((-2) #{ \markup \teeny \bold 2 #})
				   (else "bla")) #}))))

\relative c' {
  <d f g b>2
  \applyOutput Voice.NoteHead #mc-squared
  <d f g b>2
}
