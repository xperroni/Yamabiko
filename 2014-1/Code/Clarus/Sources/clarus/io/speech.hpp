/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

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
along with Clarus. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLARUS_IO_SPEECH_HPP
#define CLARUS_IO_SPEECH_HPP

#include <string>

namespace clarus {
    class speech;
}

class clarus::speech {
public:
    struct Language {
        /* Human-readable name of the language. */
        const std::string name;

        /* ISO code for the language. */
        const std::string ISO;

        Language(const std::string &name, const std::string &ISO);
    };

    static const Language EN_US;

    static const Language ES_ES;

    static const Language PT_BR;

    static std::string listen(const Language &language);

    static void say(const std::string &message, const Language &language);
};

#endif
