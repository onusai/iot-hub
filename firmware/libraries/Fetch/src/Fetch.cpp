#include "Fetch.h"
#include "debug.h"
#include <WiFiClient.h>

Response fetch(const char *url, RequestOptions options)
{
	// Parsing URL.
	Url parsedUrl = parseUrl(url);

	WiFiClient client;

	// Connecting to server.
	while (!client.connect(parsedUrl.host.c_str(), parsedUrl.port))
	{
		delay(1000);
		Serial.print(".");
	}

	if (options.method == "POST")
	{
		options.headers["Content-Length"] = String(options.body.text().length());
	}
	// Forming request.
	String request =
		options.method + " " + parsedUrl.path + parsedUrl.afterPath + " HTTP/1.1\r\n" +
		"Host: " + parsedUrl.host + "\r\n" +
		options.headers.text() +
		options.body + "\r\n\r\n";

	DEBUG_FETCH("-----REQUEST START-----\n%s\n-----REQUEST END-----", request.c_str());

	// Sending request.
	client.print(request);

	// Getting response headers.
	Response response;
	for (int nLine = 1; client.connected(); nLine++)
	{
		// Reading headers line by line.
		String line = client.readStringUntil('\n');
		// Parse status and statusText from line 1.
		if (nLine == 1)
		{
			response.status = line.substring(line.indexOf(" ")).substring(0, line.indexOf(" ")).toInt();
			response.statusText = line.substring(line.indexOf(String(response.status)) + 4);
			response.statusText.trim();
			continue;
		}

		response.headers += line + "\n";
		// If headers end, move on.
		if (line == "\r")
			break;
	}

	DEBUG_FETCH("-----HEADERS START-----\n%s\n-----HEADERS END-----", response.headers.text().c_str());

	// Getting response body.
	while (client.available())
	{
		response.body += client.readStringUntil('\n');
	}

	// Stopping the client.
	client.stop();

	return response;
}

FetchClient fetch(const char *url, RequestOptions options, OnResponseCallback onResponseCallback)
{
	// Parsing URL.
	Url parsedUrl = parseUrl(url);
	WiFiClient client;
	client.setTimeout(15000);

	// Connecting to server.
	if (client.connect(parsedUrl.host.c_str(), parsedUrl.port))
	{
		// Forming request.
		String request =
			options.method + " " + parsedUrl.path + parsedUrl.afterPath + " HTTP/1.1\r\n" +
			"Host: " + parsedUrl.host + "\r\n" +
			options.headers.text() +
			options.body + "\r\n\r\n";

		DEBUG_FETCH("-----REQUEST START-----\n%s\n-----REQUEST END-----", request.c_str());

		// Sending request.
		client.print(request);
		return FetchClient(client, onResponseCallback, CONNECTED, parsedUrl, options);
	}
	else
	{
		DEBUG_FETCH("Connection failed.");
		return FetchClient(client, onResponseCallback, CONNECTING, parsedUrl, options);
	}
}

FetchClient::FetchClient() {}

FetchClient::FetchClient(WiFiClient &client, OnResponseCallback onResponseCallback, ConnectionStatus connectionStatus, Url url, RequestOptions options) : _client(client), _OnResponseCallback(onResponseCallback), _connectionStatus(connectionStatus), _connectRetries(0), _url(url), _requestOptions(options) {}

void FetchClient::loop()
{
	if (_connectionStatus == CONNECTING)
	{
		Serial.printf("Connection retry: %d\n", _connectRetries++);
		if (_client.connect(_url.host.c_str(), _url.port))
		{
			_connectionStatus = CONNECTED;
			// Forming request.
			String request =
				_requestOptions.method + " " + _url.path + _url.afterPath + " HTTP/1.1\r\n" +
				"Host: " + _url.host + "\r\n" +
				_requestOptions.headers.text() +
				_requestOptions.body + "\r\n\r\n";

			DEBUG_FETCH("-----REQUEST START-----\n%s\n-----REQUEST END-----", request.c_str());

			// Sending request.
			_client.print(request);
		}
		else
		{
			DEBUG_FETCH("Connection failed.");
			_connectionStatus = CONNECTING;
		}
	}
	else if (_connectionStatus == CONNECTED)
	{
		Response response;
		if (_client.connected())
		{
			DEBUG_FETCH("[Info] Receiving response.");
			// Getting response headers.

			for (int nLine = 1; _client.available(); nLine++)
			{
				// Reading headers line by line.
				String line = _client.readStringUntil('\n');
				// Parse status and statusText from line 1.
				if (nLine == 1)
				{
					DEBUG_FETCH(line);
					response.status = line.substring(line.indexOf(" ")).substring(0, line.indexOf(" ")).toInt();
					response.statusText = line.substring(line.indexOf(String(response.status)) + 4);
					response.statusText.trim();
					continue;
				}

				response.headers += line + "\n";
				// If headers end, move on.
				if (line == "\r")
					break;
			}

			DEBUG_FETCH("-----HEADERS START-----\n%s\n-----HEADERS END-----", response.headers.text().c_str());

			// Getting response body.
			while (_client.available())
			{
				response.body += _client.readStringUntil('\n');
			}
		}
		else
		{
			DEBUG_FETCH("Not connected");
		}
		// Stopping the client.
		_client.stop();
		_connectionStatus = IDLE;
		_OnResponseCallback(response);
	}
}

ResponseHeaders::ResponseHeaders() : _text("") {}

String ResponseHeaders::get(String headerName)
{
	String tillEnd = _text.substring(_text.lastIndexOf(headerName + ": "));
	return tillEnd.substring(tillEnd.indexOf(" ") + 1, tillEnd.indexOf("\n") - 1);
}

String &ResponseHeaders::text()
{
	return _text;
}

void ResponseHeaders::operator+=(const String &s)
{
	_text += s;
}

String ResponseHeaders::operator[](const String &headerName)
{
	return get(headerName);
}

Body::Body() : _text("") {}

String Body::text()
{
	return this->_text;
}

String Body::operator+(String str)
{
	return this->_text + str;
}

String Body::operator=(String str)
{
	return this->_text = str;
}

String operator+(String str, Body body)
{
	return str + body.text();
}

Response::Response() : ok(false), status(418), statusText("I'm a teapot"),
					   redirected(false), type(""), headers({}), body("") {}

String Response::text()
{
	return this->body;
}

size_t Response::printTo(Print &p) const
{
	size_t r = 0;

	r += p.printf(
		(String("{") +
		 "\n\t\"ok\": %d" +
		 "\n\t\"status\": %d" +
		 "\n\t\"statusText\": \"%s\"" +
		 "\n\t\"body\": \"%s\"" +
		 "\n}")
			.c_str(),
		ok, status, statusText.c_str(), body.c_str());

	return r;
}

RequestOptions::RequestOptions() : method("GET"), headers(Headers()), body(Body()),
#if defined(ESP8266)
								   fingerprint(""),
#endif
								   caCert("")
{
}