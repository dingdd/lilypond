/*
  Sequential_music_iterator.cc -- implement Sequential_music_iterator

  source file of the GNU LilyPond music typesetter

  (c)  1997--1999 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/
#include "grace-iterator.hh"
#include "translator-group.hh"
#include "debug.hh"
#include "sequential-music-iterator.hh"
#include "music-list.hh"
#include "request-chord-iterator.hh"


void
Sequential_music_iterator::do_print() const
{
  if (iter_p_)
    iter_p_->print();
}

Sequential_music_iterator::Sequential_music_iterator ()
{
  cursor_ = 0;
  here_mom_ = 0;
  iter_p_ =0;
  per_elt_b_ = false;
}

void
Sequential_music_iterator::construct_children()
{
  cursor_ = dynamic_cast<Music_sequence const*> (music_l_)->music_p_list_p_->head_;
  
  while (cursor_)
    {
      start_next_element();
      if (!iter_p_->ok()) 
	{
	  leave_element();
	}
      else 
	{
	  set_sequential_music_translator();
	  break;
	}
    }
}

void 
Sequential_music_iterator::leave_element()
{
  delete iter_p_;
  iter_p_ =0;
  Moment elt_time = cursor_->car_->length_mom ();
  here_mom_ += elt_time;
  cursor_ =cursor_->next_;
}

void
Sequential_music_iterator::start_next_element()
{
  assert (!iter_p_);
  iter_p_ = get_iterator_p (cursor_->car_);
}

void
Sequential_music_iterator::set_sequential_music_translator()
{
  if (iter_p_->report_to_l()->depth_i () > report_to_l ()->depth_i ()
      && ! dynamic_cast<Grace_iterator*> (iter_p_)) // UGH.!
    set_translator (iter_p_->report_to_l());
}

Sequential_music_iterator::~Sequential_music_iterator()
{
  assert (! iter_p_);
}




void
Sequential_music_iterator::do_process_and_next (Moment until)
{
  if (!iter_p_)
    return;

  while (1) 
    {
      Moment local_until = until - here_mom_;
      while (iter_p_->ok()) 
	{
	  Moment here = iter_p_->next_moment();
	  if (here != local_until)
	    goto loopexit;
	    
	  iter_p_->process_and_next (local_until);
	}
      
      if (!iter_p_->ok()) 
	{
	  leave_element();
	  
	  if (cursor_)
	    {
	      start_next_element();
	      set_sequential_music_translator();

	      if (per_elt_b_)
		goto loopexit;	// ugh.
	    }
	  else 
	    {
	      goto loopexit;
	    }
	}
    }

loopexit:

  Music_iterator::do_process_and_next (until);
}

Moment
Sequential_music_iterator::next_moment() const
{
  return iter_p_->next_moment() + here_mom_;
}

Music*
Sequential_music_iterator::next_music_l ()
{
  if (!iter_p_)
    return 0;

  while (1) 
    {
      if (Music* m = iter_p_->next_music_l ())
	{
	  return m;
	}
      else
	{
	  // urg FIXME: sequential children should be iterated to finish
	  if (dynamic_cast<Request_chord_iterator*> (iter_p_))
	    delete iter_p_;
	  iter_p_ = 0;
	  leave_element ();
	  
	  if (cursor_)
	    {
	      start_next_element ();
	      set_sequential_music_translator ();
	    }
	  else 
	    {
	      delete iter_p_;
	      iter_p_ = 0;
	      return 0;
	    }
	}
    }
}

bool
Sequential_music_iterator::ok() const
{
  return iter_p_;
}

