/*
  notehead.cc -- implement Note_head

  source file of the GNU LilyPond music typesetter

  (c)  1997--2000 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/

#include "misc.hh"
#include "dots.hh"
#include "note-head.hh"
#include "debug.hh"
#include "lookup.hh"
#include "molecule.hh"
#include "musical-request.hh"
#include "dimension-cache.hh"
#include "staff-symbol-referencer.hh"



/*
  build a ledger line for small pieces.
 */
Molecule
Note_head::ledger_line (Interval xwid) const
{
  Drul_array<Molecule> endings;
  endings[LEFT] = lookup_l()->afm_find ("noteheads-ledgerending");
  Molecule *e = &endings[LEFT];
  endings[RIGHT] = *e;
  
  Real thick = e->dim_[Y_AXIS].length();
  Real len = e->dim_[X_AXIS].length () - thick;

  Molecule total;
  Direction d = LEFT;
  do {
    endings[d].translate_axis (xwid[d] - endings[d].dim_[X_AXIS][d], X_AXIS);
    total.add_molecule (endings[d]);    
  } while ((flip(&d)) != LEFT);

  Real xpos = xwid [LEFT] + len;

  while (xpos + len + thick /2 <= xwid[RIGHT])
    {
      e->translate_axis (len, X_AXIS);
      total.add_molecule (*e);
      xpos += len;
    }

  return total;
}


void
Note_head::before_line_breaking ()
{
  // 8 ball looks the same as 4 ball:
  
  if (balltype_i () > 2)
    set_elt_property ("duration-log", gh_int2scm (2));

  if (Dots *d = dots_l ())
    { // move into Rhythmic_head?
      Staff_symbol_referencer_interface si (d);
      Staff_symbol_referencer_interface me (this);      
      
      si.set_position(int (me.position_f ()));
    }
}




Molecule 
Note_head::do_brew_molecule() const 
{
  Staff_symbol_referencer_interface si (this);
  
  Real inter_f = si.staff_space ()/2;
  int sz = si.line_count ()-1;
  Real p = si.position_f ();
  int streepjes_i = abs (p) < sz 
    ? 0
    : (abs((int)p) - sz) /2;

  SCM style  = get_elt_property ("style");
  if (style == SCM_UNDEFINED)
    {
      style = ly_str02scm("");
    }
  
  Molecule out = lookup_l()->afm_find (String ("noteheads-") + 
		ly_scm2string (scm_eval (gh_list (ly_symbol2scm("noteheadsymbol"),
						  gh_int2scm(balltype_i ()),
						  style,
						  SCM_UNDEFINED))));

  Box ledgerless = out.dim_;

  if (streepjes_i) 
    {
      Direction dir = (Direction)sign (p);
      Interval hd = out.dim_[X_AXIS];
      Real hw = hd.length ()/4;
      
      Molecule ledger (ledger_line  (Interval (hd[LEFT] - hw,
					       hd[RIGHT] + hw)));
      
      int parity =  abs(int (p)) % 2;
      
      for (int i=0; i < streepjes_i; i++)
	{
	  Molecule s (ledger);
	  s.translate_axis (-dir * inter_f * (i*2 + parity),
			   Y_AXIS);
	  out.add_molecule (s);
	}
    }

  out.dim_ = ledgerless;
  return out;
}

