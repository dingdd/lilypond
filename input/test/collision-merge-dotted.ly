\score { 
  \context Voice \notes\relative c {
    
	\relative c'' \context Staff <
	\context Voice = one {
		\property Staff.collisionMergeDotted = ##t
		\stemup
		[c8 f g a]
	}
	\context Voice = two {
		\stemdown c,4. f8
	
	}>
	
  }
  \paper { }  
  \midi { }
}