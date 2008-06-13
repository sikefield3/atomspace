/*
 * src/Util/StringTokenizer.cc
 *
 * Copyright (C) 2002-2007 Novamente LLC
 * All Rights Reserved
 *
 * Written by Carlos Lopes <dlopes@vettalabs.com>
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

#include "StringTokenizer.h"
#include "exceptions.h"
#include <cstring>

using namespace opencog;

StringTokenizer::StringTokenizer()
{
    reset();

    str.assign("");
    delimiter.assign("");
}

StringTokenizer::StringTokenizer(const std::string &str, const std::string &delimiter)
{
    reset();

    this->str = str;
    this->delimiter = delimiter;
}

StringTokenizer::~StringTokenizer()
{
}

std::string StringTokenizer::getString()
{
    return str;
}

void StringTokenizer::setString(const std::string &str)
{
    this->str = str;
}

const std::string & StringTokenizer::getDelimiter()
{
    return delimiter;
}

void StringTokenizer::setDelimiter(const std::string &str)
{
    this->delimiter = delimiter;
}

void StringTokenizer::reset()
{
    start = 0;
    end = 0;
}

const std::string StringTokenizer::nextToken()
{
    cassert(TRACE_INFO, str != "", "StringTokenizer - string should not be empty.");
    cassert(TRACE_INFO, delimiter != "", "StringTokenized - delimiter should not be empty.");

    // end of the string
    if (end == str.size()) {
        return "";
    }

    if (start ==  0 && end == 0) {
        end = str.find(delimiter);
        if ( end == std::string::npos ) {
            end = str.size();
        }
        return str.substr(start, end - start);
    }

    do {
        start = end + delimiterSize();
        if (start == str.size()) {
            end = start;
            return "";
        }
        end = str.find(delimiter, start);
        if (end == std::string::npos) {
            end = str.size();
        }
    } while ( str.substr(start, end - start) == delimiter || end == start);

    return str.substr(start, end - start);
}

std::string::size_type StringTokenizer::delimiterSize()
{
    return delimiter.size();
}
