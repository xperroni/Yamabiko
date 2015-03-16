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

#include <clarus/io/speech.hpp>
using clarus::speech;
using std::string;

#include <clarus/core/domain.hpp>

#include <clarus/io/http.hpp>
using clarus::http;

#include <clarus/io/microphone.hpp>
using clarus::List;
using clarus::Microphone;

#include <boost/regex.hpp>

#include <stdexcept>
using std::runtime_error;

speech::Language::Language(const string &_name, const string &_ISO):
    name(_name),
    ISO(_ISO)
{
    // Nothing to do.
}

const speech::Language speech::EN_US = Language("english", "en_US");

const speech::Language speech::ES_ES = Language("spanish", "es_ES");

const speech::Language speech::PT_BR = Language("portuguese", "pt_BR");

/*
References:

Google Speech API v2
https://github.com/gillesdemey/google-speech-v2
*/
string speech::listen(const Language &language) {
    static boost::regex e("transcript\":\"([^\"]+)\"");
    static string type("audio/x-flac; rate=44100");
    static string host("www.google.com");
    static string path(
        "/speech-api/v2/recognize?"
        "?client=chromium"
        "&maxresults=1"
        "&key=AIzaSyBOti4mM-6x9WDnZIjIeyEU21OpBXqWBgw"
    );

    Microphone mic;
    List<char> data = mic();
    string response = http::post(host, path + "&lang=" + language.ISO, type, data);

    boost::smatch what;
    return (boost::regex_search(response, what, e, boost::match_extra) ? what[1] : string(""));
}

void speech::say(const string &message, const Language &language) {
    string command = "echo '" + message + "' | festival --tts --language " + language.name;
    int status = system(command.c_str());
    if (status != 0) {
        throw runtime_error("Error (" + domain::to_string(status) + ") calling festival");
    }
}
