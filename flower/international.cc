/*
  international.cc -- implement stuff for internationalisation

  source file of the GNU LilyPond music typesetter

  (c) 1997--2006 Jan Nieuwenhuizen <janneke@gnu.org>
*/

#include "config.hh"

#include "international.hh"
#include "string-convert.hh"

#if !HAVE_GETTEXT
inline char *
gettext (char const *s)
{
  return (char *)s;
}
#else
#include <libintl.h>
#endif

std::string
_ (char const *ch)
{
  return std::string (gettext (ch));
}

std::string
_f (char const *format, ...)
{
  va_list args;
  va_start (args, format);
  std::string str = String_convert::vform_string (gettext (format), args);
  va_end (args);
  return str;
}

std::string
_f (char const *format, std::string s, std::string s2, std::string s3)
{
  return String_convert::form_string (gettext (format), s.c_str (), s2.c_str (),
				      s3.c_str ());
}
