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

#include <clarus/io/microphone.hpp>
using clarus::InputCommand;
using clarus::Microphone;
using std::string;

/*
References:

The SoX of Silence
http://digitalcardboard.com/blog/2009/08/25/the-sox-of-silence/

command-line utility to record from microphone
http://superuser.com/questions/384748/command-line-utility-to-record-from-microphone
*/
static string sox("sox");
static string sox_options(
    " --encoding signed-integer --bits 16 --channels 1 --rate 44100"
    " -t alsa default -t flac %s"
    " silence 1 0.1 5%% 1 1.0 5%%"
);

Microphone::Microphone(bool debug):
    InputCommand(sox + (debug ? " -q" : "") + sox_options)
{
    // Nothing to do.
}
