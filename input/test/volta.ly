\score { 
  \context Voice \notes\relative c {
    % Test of prima/secunda volta brackets in conjunction with 
	% line breaks and special bars.
	
	c' d e f |
	\repeat "volta" 2 { g a b c }
	\alternative { { c b a g a g a g} { f e d c } } 
	\repeat "volta" 2 { g a b c }
	\alternative { { c b a g \break a g a g} { f e d c } } 
	c' d e f |
	\repeat "volta" 2 { g a b c }
	\alternative { { c b a g \break a g a g \break a b a b } { f e d c } }
	c' d e f |
	\repeat "volta" 2 { g a b c }
	\alternative { { c b a g } { f e d c \break a g a g} }
	c' d e f |
	\repeat "volta" 2 { g a b c }
	\alternative { { c b a g a g a g} { f e d c \bar "|.";} }
	
  }
  \paper { }  
  \midi { }
}