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

#include "http.hpp"
using clarus::http;
using clarus::List;

#include <boost/asio.hpp>
using boost::asio::ip::tcp;

#include <cstdlib>

#include <stdexcept>
using std::runtime_error;

#include <iostream>
#include <istream>
#include <ostream>
using std::ios;
using std::istream;
using std::ostream;

#include <fstream>
using std::ifstream;

#include <sstream>
using std::ostringstream;

#include <string>
using std::string;

string http::post(
    const string &host,
    const string &path,
    const string &type,
    const List<char> &data
) {
    boost::asio::io_service io_service;

    // Get a list of endpoints corresponding to the server name.
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(host, "http");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    // Try each endpoint until we successfully establish a connection.
    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    boost::asio::streambuf request;
    ostream request_stream(&request);
    request_stream << "POST " << path << " HTTP/1.1\r\n";
    request_stream
        << "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_8) "
        << "AppleWebKit/535.7 (KHTML, like Gecko)"
        << " Chrome/16.0.912.77 Safari/535.7\r\n";

    request_stream << "Host: " << host << "\r\n";
    request_stream << "Content-Type: " << type << "\r\n";
    request_stream << "Content-Length: " << data.size() << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    request_stream.write(&(data[0]), data.size());

    // Send the request.
    boost::asio::write(socket, request);

    // Read the response status line. The response streambuf will automatically
    // grow to accommodate the entire line. The growth may be limited by passing
    // a maximum size to the streambuf constructor.
    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n");

    // Check that response is OK.
    istream response_stream(&response);

    string http_version;
    response_stream >> http_version;
    if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
        throw runtime_error("Invalid response");
    }

    int status_code;
    string status_message;
    response_stream >> status_code;
    getline(response_stream, status_message);
    if (status_code != 200) {
        throw runtime_error(status_message); // + " (" + status_code + ")");
    }

    // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, response, "\r\n\r\n");

    // Do away with the response headers.
    string header;
    while (std::getline(response_stream, header) && header != "\r") {
        // cout << header << endl;
    }

    ostringstream out;

    // Write whatever content we already have to output.
    if (response.size() > 0) {
        out << &response;
    }

    // Write the remaining content to output.
    boost::system::error_code error;
    while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) {
        out << &response;
    }

    if (error != boost::asio::error::eof) {
        throw boost::system::system_error(error);
    }

    return out.str();
}

string http::post(
    const string &host,
    const string &path,
    const string &type,
    const string &file
) {
    ifstream ifs(file.c_str(), ios::in | ios::binary | ios::ate);
    int n = ifs.tellg();

    List<char> data(n, '\0');
    ifs.seekg(0, ios::beg);
    ifs.read(&data[0], n);
    ifs.close();

    return post(host, path, type, data);
}
