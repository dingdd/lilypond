\score { 
  \context Voice \notes\relative c {
    
	\context Staff <
		\context Voice =VA  {\property Voice.verticalDirection=1 \property Voice.horizontalNoteShift = 0 g'' }
		\context Voice =VB  {\property Voice.verticalDirection=1 \property Voice.horizontalNoteShift = 1 e }
		\context Voice =VC  {\property Voice.verticalDirection=1 \property Voice.horizontalNoteShift = 2 c }
		\context Voice =VD  {\property Voice.verticalDirection=1 \property Voice.horizontalNoteShift = 3 a }
		\context Voice =VE  {\property Voice.verticalDirection=1 \property Voice.horizontalNoteShift = 4 f }
	>
	
	
	
  }
  \paper { }  
  \midi { }
}