/*
 * opencog/atoms/grounded/LibraryRunner.cc
 *
 * Copyright (C) 2009, 2013, 2014, 2015, 2020 Linas Vepstas
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <opencog/atoms/atom_types/atom_types.h>
#include <opencog/atoms/truthvalue/TruthValue.h>
#include <opencog/atoms/value/Value.h>
#include <opencog/atomspace/AtomSpace.h>

#include <opencog/atoms/grounded/LibraryManager.h>
#include <opencog/atoms/grounded/LibraryRunner.h>

using namespace opencog;

LibraryRunner::LibraryRunner(std::string s)
	: _fname(s)
{
	// Extract the language, library and function from schema
	std::string lang, lib, fun;
	LibraryManager::parse_schema(_fname, lang, lib, fun);

	sym = LibraryManager::getFunc(lib,fun);
}

// ----------------------------------------------------------

static void throwSyntaxEx(bool silent, const char* message...)
{
	if (silent)
		throw NotEvaluatableException();
	va_list args;
	va_start(args, message);
	throw SyntaxException(TRACE_INFO, message, args);
	va_end(args);
}

// ----------------------------------------------------------

/// `execute()` -- evaluate a LibraryRunner with arguments.
///
/// Expects "args" to be a ListLink. These arguments will be
///     substituted into the predicate.
///
ValuePtr LibraryRunner::execute(AtomSpace* as,
                               const ValuePtr& vargs,
                               bool silent)
{
	if (not vargs->is_atom())
		throw SyntaxException(TRACE_INFO,
			"LibraryRunner: Expecting Handle; got %s",
			vargs->to_string().c_str());

	Handle cargs = HandleCast(vargs);
	Handle args(as->add_atom(cargs));

	// Convert the void* pointer to the correct function type.
	Handle* (*func)(AtomSpace*, Handle*);
	func = reinterpret_cast<Handle* (*)(AtomSpace *, Handle*)>(sym);

	ValuePtr result;

	// Execute the function
	Handle* res = func(as, &args);
	if (nullptr != res)
	{
		result = *res;
		free(res);
	}

	if (nullptr == result)
		throwSyntaxEx(silent,
	        "Invalid return value from grounded schema %s\nArgs: %s",
		        _fname.c_str(),
		        cargs->to_short_string().c_str());

	return result;
}

ValuePtr LibraryRunner::evaluate(AtomSpace* as,
                                const ValuePtr& vargs,
                                bool silent)
{
	if (not vargs->is_atom())
		throw SyntaxException(TRACE_INFO,
			"LibraryRunner: Expecting Handle; got %s",
			vargs->to_string().c_str());

	Handle cargs = HandleCast(vargs);
	Handle args(as->add_atom(cargs));

	// Convert the void* pointer to the correct function type.
	TruthValuePtr* (*func)(AtomSpace*, Handle*);
	func = reinterpret_cast<TruthValuePtr* (*)(AtomSpace *, Handle*)>(sym);

	// Evaluate the predicate
	TruthValuePtr* res = func(as, &args);
	TruthValuePtr result;
	if (nullptr != res)
	{
		result = *res;
		free(res);
	}

	if (nullptr == result)
		throwSyntaxEx(silent,
	        "Invalid return value from grounded predicate %s\nArgs: %s",
		        _fname.c_str(),
		        cargs->to_short_string().c_str());

	return CastToValue(result);
}
