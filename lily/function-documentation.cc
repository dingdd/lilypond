/*
  function-documentation.cc -- Scheme doc strings.

  source file of the GNU LilyPond music typesetter

  (c) 2004--2007 Han-Wen Nienhuys <hanwen@xs4all.nl>
*/

#include <cstring>
using namespace std;

#include "std-string.hh"
#include "lily-guile.hh"
#include "warn.hh"

/* type predicates. */
#include "spanner.hh"
#include "item.hh"


static SCM doc_hash_table;

void
ly_check_name (string cxx, string scm_name)
{
  string mangle = mangle_cxx_identifier (cxx);
  if (mangle != scm_name)
    {
      programming_error ("wrong cxx name: " + mangle + ", " + cxx +  ", " + scm_name);
    }
}


void
ly_add_function_documentation (SCM func,
				    string fname,
				    string varlist,
				    string doc)
{
  if (doc == "")
    return;

  if (!doc_hash_table)
    doc_hash_table = scm_permanent_object (scm_c_make_hash_table (59));

  string s = string (" - ") + "LilyPond procedure: " + fname + " " + varlist
    + "\n" + doc;

  scm_set_procedure_property_x (func, ly_symbol2scm ("documentation"),
				ly_string2scm (s));
  SCM entry = scm_cons (ly_string2scm (varlist), ly_string2scm (doc));
  scm_hashq_set_x (doc_hash_table, ly_symbol2scm (fname.c_str ()), entry);
}

LY_DEFINE (ly_get_all_function_documentation, "ly:get-all-function-documentation",
	   0, 0, 0, (),
	   "Get a hash table with all lilypond Scheme extension functions.")
{
  return doc_hash_table;
}


#include <map>

map<void *, string>  type_names;
  
void
ly_add_type_predicate (void *ptr,
		       string name)
{
  type_names[ptr] = name; 
}

string
predicate_to_typename (void *ptr)
{
  if (type_names.find (ptr) == type_names.end ())
    {
      programming_error ("Unknown type predicate");
      return "unknown type";
    }
  else
    return type_names[ptr];
}

static int
arg_dir (int a, int b)
{
  if (&a < &b)
    return 1;
  else
    return -1;
}


int function_argument_direction;
void
init_func_doc ()
{
  function_argument_direction = arg_dir (2,3);

  ly_add_type_predicate ((void*) &scm_is_integer, "integer");
  ly_add_type_predicate ((void*) &scm_is_bool, "boolean");
  ly_add_type_predicate ((void*) &scm_is_pair, "pair");
  ly_add_type_predicate ((void*) &is_number_pair, "number pair");
  ly_add_type_predicate ((void*) &scm_is_number, "number");
  ly_add_type_predicate ((void*) &scm_is_string, "string");
  ly_add_type_predicate ((void*) &ly_is_symbol, "symbol");
  ly_add_type_predicate ((void*) &scm_is_vector, "vector");
  ly_add_type_predicate ((void*) &is_axis, "axis");
  ly_add_type_predicate ((void*) &unsmob_spanner, "spanner");
  ly_add_type_predicate ((void*) &unsmob_item, "item");
}

ADD_SCM_INIT_FUNC (func_doc, init_func_doc);
