/*
  paper-score.cc -- implement Paper_score

  source file of the GNU LilyPond music typesetter

  (c) 1996--2004 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/

#include "score.hh"
#include "main.hh"
#include "warn.hh"
#include "font-metric.hh"
#include "spanner.hh"
#include "paper-def.hh"
#include "system.hh"
#include "paper-column.hh"
#include "paper-score.hh"
#include "paper-column.hh"
#include "scm-hash.hh"
#include "gourlay-breaking.hh"
#include "paper-outputter.hh"
#include "input-file-results.hh"
#include "misc.hh"
#include "all-font-metrics.hh"

Paper_score::Paper_score ()
{
  paper_ =0;
  outputter_ =0;
  system_ = 0;
  main_smob_ = SCM_EOL;
}

void
Paper_score::typeset_line (System *l)
{
  if (!system_)
    {
      system_ = l;		// ugh.
    }

  main_smob_ = gh_cons (l->self_scm (), main_smob_);
  l->pscore_ = this;

  scm_gc_unprotect_object (l->self_scm());
}


Paper_score::Paper_score (Paper_score const &s)
  : Music_output (s)
{
  assert (false);
}

Array<Column_x_positions>
Paper_score::calc_breaking ()
{
  Break_algorithm *algorithm=0;
  Array<Column_x_positions> sol;

  algorithm = new Gourlay_breaking ;
  algorithm->set_pscore (this);
  sol = algorithm->solve ();
  delete algorithm;

  return sol;
}

/*
  urg. clean me
 */
void
Paper_score::process (String outname)
{
  if (verbose_global_b)
    progress_indication (_f ("Element count %d (spanners %d) ",
			     system_->element_count (),
			     system_->spanner_count ()));

  
  progress_indication (_ ("Preprocessing graphical objects...") + " ");

  /*
    Be sure to set breakability on first & last column.
   */
  { /* doubly, also done in Score_engraver */
    Link_array<Grob> pc (system_->columns ());
  
    pc[0]->set_grob_property ("breakable", SCM_BOOL_T);
    pc.top ()->set_grob_property ("breakable", SCM_BOOL_T);
  }
  system_->pre_processing ();
 
  Array<Column_x_positions> breaking = calc_breaking ();
  system_->break_into_pieces (breaking);
  
  outputter_ = paper_->get_paper_outputter (outname);
  outputter_->output_header ();
  outputter_->output_version ();

  progress_indication ("\n");

  if (global_input_file->header_)
    {
      outputter_->output_scope (global_input_file->header_, "lilypond");
      outputter_->write_header_fields_to_file (global_input_file->header_);
    }
  
  if (header_)
    {
      outputter_->output_scope (header_, "lilypond");
      outputter_->write_header_fields_to_file (header_);
    }

  outputter_->output_scope (paper_->scope_, "lilypondpaper");

  SCM scm = scm_list_n (ly_symbol2scm ("header-end"), SCM_UNDEFINED);
  outputter_->output_scheme (scm);

  system_->output_lines ();

  scm = scm_list_n (ly_symbol2scm ("end-output"), SCM_UNDEFINED);
  outputter_->output_scheme (scm);

  progress_indication ("\n");

  // huh?
  delete outputter_;
  outputter_ = 0;
}
