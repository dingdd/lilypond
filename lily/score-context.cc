/*   
score-context.cc -- implement

source file of the GNU LilyPond music typesetter

(c) 2004 Han-Wen Nienhuys <hanwen@xs4all.nl>

 */


#include "score-context.hh"
#include "score-translator.hh"


void
Score_context::prepare (Moment w)
{
  Translator*  t = unsmob_translator (implementation_);
  Score_translator * s = dynamic_cast<Score_translator *> (t);

  s->prepare (w);
}

void
Score_context::finish ()
{
  Translator*  t = unsmob_translator (implementation_);
  Score_translator * s = dynamic_cast<Score_translator *> (t);

  s->finish ();
}

void
Score_context::one_time_step ()
{
  Translator*  t = unsmob_translator (implementation_);
  Score_translator * s = dynamic_cast<Score_translator *> (t);
  s->one_time_step();
}


Music_output*
Score_context::get_output ()
{
  Translator*  t = unsmob_translator (implementation_);
  Score_translator * s = dynamic_cast<Score_translator *> (t);
  return s->get_output ();
}

