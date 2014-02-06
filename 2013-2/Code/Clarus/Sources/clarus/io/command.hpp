/*
This file is part of Clarus.

Clarus is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Clarus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Clarus.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLARUS_IO_COMMAND_HPP
#define CLARUS_IO_COMMAND_HPP

#include <boost/shared_ptr.hpp>

#include <cstdio>
#include <string>

class Command: public boost::shared_ptr<FILE> {
public:
    Command(const std::string &command, const std::string &mode);
};

class InputCommand: private Command {
public:
    InputCommand(const std::string &command);

    bool operator () (std::string &line);
};

class OutputCommand: private Command {
    bool autoflush;

public:
    OutputCommand(const std::string &command, bool autoflush = false);

    void operator () (const std::string &format, ...);

    void flush();
};

#endif // PIPE_HPP
