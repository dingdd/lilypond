#include "grob.hh"
#include "warn.hh"
#include "spanner.hh"
#include "item.hh"
#include "paper-def.hh"
#include "system.hh"


LY_DEFINE (ly_set_grob_property,"ly:set-grob-property!", 3, 0, 0,
  (SCM grob, SCM sym, SCM val),
  "Set @var{sym} in grob @var{grob} to value @var{val}")
{
  Grob * sc = unsmob_grob (grob);
  SCM_ASSERT_TYPE (sc, grob, SCM_ARG1, __FUNCTION__, "grob");
  SCM_ASSERT_TYPE (gh_symbol_p (sym), sym, SCM_ARG2, __FUNCTION__, "symbol");  

  if (!type_check_assignment (sym, val, ly_symbol2scm ("backend-type?")))
    error ("typecheck failed");
      
  sc->internal_set_grob_property (sym, val);
  return SCM_UNSPECIFIED;
}

LY_DEFINE (ly_get_grob_property,
	  "ly:get-grob-property", 2, 0, 0, (SCM grob, SCM sym),
	  "Get the value of a value in grob @var{g} of property @var{sym}. It\n"
"will return @code{'()} (end-of-list) if @var{g} doesn't have @var{sym} set.\n"
"\n"
"Grob properties are stored as GUILE association lists, with symbols as\n"
"keys. All lookup functions identify undefined properties with\n"
"end-of-list (i.e. @code{'()} in Scheme or @code{SCM_EOL} in C)\n"
"\n")
{
  Grob * sc = unsmob_grob (grob);
  SCM_ASSERT_TYPE (sc, grob, SCM_ARG1, __FUNCTION__, "grob");
  SCM_ASSERT_TYPE (gh_symbol_p (sym), sym, SCM_ARG2, __FUNCTION__, "symbol");  

  return sc->internal_get_grob_property (sym);
}

LY_DEFINE (spanner_get_bound, "ly:get-spanner-bound", 2 , 0, 0,
	   (SCM slur, SCM dir),
	   "Get one of the bounds of @var{spanner}. @var{dir} is @code{-1} "
	   "for left, and @code{1} for right.")
{
  Spanner * sl = dynamic_cast<Spanner*> (unsmob_grob (slur));
  SCM_ASSERT_TYPE (sl, slur, SCM_ARG1, __FUNCTION__, "spanner grob");
  SCM_ASSERT_TYPE (is_direction (dir), slur, SCM_ARG2, __FUNCTION__, "dir");
  return sl->get_bound (to_dir (dir))->self_scm ();
}

/* TODO: make difference between scaled and unscalead variable in
   calling (i.e different funcs.) */
LY_DEFINE (ly_get_paper_var,"ly:get-paper-variable", 2, 0, 0,
  (SCM grob, SCM sym),
  "Get a variable from the \\paper block.")
{
  Grob * sc = unsmob_grob (grob);
  SCM_ASSERT_TYPE (sc, grob, SCM_ARG1, __FUNCTION__, "grob");
  SCM_ASSERT_TYPE (gh_symbol_p (sym), sym, SCM_ARG2, __FUNCTION__, "symbol");  

  return sc->get_paper () ->lookup_variable (sym);
}

/* TODO: make difference between scaled and unscalead variable in
   calling (i.e different funcs.) */
LY_DEFINE (ly_grob_paper,"ly:grob-paper", 1, 0, 0,
  (SCM grob),
  "Get \\paper definition from a grob.")
{
  Grob * sc = unsmob_grob (grob);
  SCM_ASSERT_TYPE (sc, grob, SCM_ARG1, __FUNCTION__, "grob");

  return sc->get_paper ()->self_scm ();
}




LY_DEFINE (ly_get_extent, "ly:get-extent", 3, 0, 0,
	   (SCM grob, SCM refp, SCM axis),
	   "Get the extent in @var{axis} direction of @var{grob} relative to "
	   "the grob @var{refp}")
{
  Grob * sc = unsmob_grob (grob);
  Grob * ref = unsmob_grob (refp);
  SCM_ASSERT_TYPE (sc, grob, SCM_ARG1, __FUNCTION__, "grob");
  SCM_ASSERT_TYPE (ref, refp, SCM_ARG2, __FUNCTION__, "grob");
  
  SCM_ASSERT_TYPE (is_axis (axis), axis, SCM_ARG3, __FUNCTION__, "axis");

  return ly_interval2scm ( sc->extent (ref, Axis (gh_scm2int (axis))));
}

LY_DEFINE (ly_get_parent, "ly:get-parent", 2, 0, 0, (SCM grob, SCM axis),
	   "Get the parent of @var{grob}.  @var{axis} is 0 for the X-axis, "
	   "1 for the Y-axis.")
{
  Grob * sc = unsmob_grob (grob);
  SCM_ASSERT_TYPE (sc, grob, SCM_ARG1, __FUNCTION__, "grob");
  SCM_ASSERT_TYPE (is_axis (axis), axis, SCM_ARG2, __FUNCTION__, "axis");

  Grob * par = sc->get_parent (Axis (gh_scm2int (axis)));
  return par ? par->self_scm () : SCM_EOL;
}

LY_DEFINE (ly_get_system,
	   "ly:get-system",
	   1, 0, 0, (SCM grob),
	   "Return the System Grob of @var{grob}.")
{
  Grob *me = unsmob_grob (grob);
  SCM_ASSERT_TYPE (me, grob, SCM_ARG1, __FUNCTION__, "grob");
  
  if (System *g = me->get_system ())
    return g->self_scm ();
    
  return SCM_EOL;
}

LY_DEFINE (ly_get_original,
	   "ly:get-original",
	   1, 0, 0, (SCM grob),
	   "Return the unbroken original Grob of @var{grob}.")
{
  Grob *me = unsmob_grob (grob);
  SCM_ASSERT_TYPE (me, grob, SCM_ARG1, __FUNCTION__, "grob");
  return me->original_ ? me->original_->self_scm () : me->self_scm ();
}


/* ly prefix? spanner in name? */
/* TODO: maybe we should return a vector -- random access is more
  logical for this list? */

LY_DEFINE (get_broken_into,
	  "ly:get-broken-into", 1, 0, 0, (SCM spanner),
	   "Return broken-into list for @var{spanner}.")
{
  ///  Spanner *me = unsmob_spanner (spanner);
  Spanner *me = dynamic_cast<Spanner*> (unsmob_grob (spanner));
  SCM_ASSERT_TYPE (me, spanner, SCM_ARG1, __FUNCTION__, "spanner");

  SCM s = SCM_EOL;
  for (int i = me->broken_intos_.size (); i; i--)
    s = gh_cons (me->broken_intos_[i-1]->self_scm (), s);
  return s;
}


LY_DEFINE (ly_grob_suicide,
	  "ly:grob-suicide", 1, 0, 0, (SCM g),
	   "Kill @var{g}.")
{
  Grob *me = unsmob_grob (g);
  SCM_ASSERT_TYPE (me, g, SCM_ARG1, __FUNCTION__, "grob");

  me->suicide ();
  return SCM_UNDEFINED;
}



LY_DEFINE (ly_grob_translate_axis_x,
	  "ly:grob-translate-axis!", 3, 0, 0,
	   (SCM g, SCM d, SCM a),
	   "Translate @var{g} on axis @var{a} over distance @var{d}.")
{
  Grob *me = unsmob_grob (g);
  SCM_ASSERT_TYPE (me, g, SCM_ARG1, __FUNCTION__, "grob");
  SCM_ASSERT_TYPE (gh_number_p (d), d, SCM_ARG2, __FUNCTION__, "dimension");
  SCM_ASSERT_TYPE (is_axis (a), a, SCM_ARG3, __FUNCTION__, "axis");

  me->translate_axis (gh_scm2double (d),
		      Axis (gh_scm2int (a)));
  return SCM_UNDEFINED;
}



LY_DEFINE (ly_spanner_p,
	  "ly:spanner?", 1, 0, 0,
	   (SCM g),
	   "Is  @var{g} a spanner object?")
{
  Grob *me = unsmob_grob (g);
  bool b = dynamic_cast<Spanner*> (me);

  return gh_bool2scm (b);
}

LY_DEFINE (ly_item_p,
	  "ly:item?", 1, 0, 0,
	   (SCM g),
	   "Is  @var{g} a item object?")
{
  Grob *me = unsmob_grob (g);
  bool b = dynamic_cast<Item*> (me);

  return gh_bool2scm (b);
}


LY_DEFINE (ly_item_break_dir,
	  "ly:item-break-dir", 1, 0, 0,
	   (SCM it),
	   "The break status dir of  @var{it}.")
{
  Item * me = dynamic_cast<Item*> ( unsmob_grob (it));
  SCM_ASSERT_TYPE (me, it, SCM_ARG1, __FUNCTION__, "Item");
  
   return gh_int2scm (me->break_status_dir ());
}

