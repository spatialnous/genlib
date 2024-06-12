// Copyright (C) 2011-2012, Tasos Varoudis

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct xmlelement {
    std::string name;
    bool closetag;
    std::map<std::string, std::string> attributes;
    std::vector<xmlelement> subelements;
    xmlelement() { closetag = false; }
    bool parse(std::ifstream &stream, bool parsesubelements = false);
    friend std::ostream &operator<<(std::ostream &stream, const xmlelement &elem);

  protected:
    bool subparse(std::ifstream &stream);
    void badcharacter(char c, const std::string &location);
};

struct xmlerror {
    std::string error;
    xmlerror(const std::string &e = std::string()) { error = e; }
};
