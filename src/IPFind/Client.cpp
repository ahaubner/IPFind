/*
 *   Copyright (C) 2016 Anton Haubner <anton.haubner@outlook.de>
 *
 *   This file is part of the IPFind library.
 *
 *   This software is provided 'as-is', without any express or implied
 *   warranty.  In no event will the authors be held liable for any damages
 *   arising from the use of this software.
 *
 *   Permission is granted to anyone to use this software for any purpose,
 *   including commercial applications, and to alter it and redistribute it
 *   freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *   claim that you wrote the original software. If you use this software
 *   in a product, an acknowledgment in the product documentation would be
 *   appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be
 *   misrepresented as being the original software.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 *
 *   4. Source versions may not be "relicensed" under a different license
 *   without my explicitly written permission.
 *
*/

#include <IPFind/Client.hpp>

#include <thread>
#include <atomic>

#include <boost/asio.hpp>

#include <IPFind/Utils.hpp>

using namespace std;
using namespace boost::asio::ip;

namespace IPFind
{
	Client::Client()
	{

	}

	std::string Client::find(Utils::Port port, unsigned int retries, std::chrono::seconds timeout)
	{
		boost::asio::io_service io_service;

		udp::socket Socket(io_service, udp::endpoint(udp::v4(), 0));
		Socket.set_option(boost::asio::socket_base::broadcast(true));

		udp::endpoint Endpoint(address_v4::broadcast(), port);

		atomic_bool NeedCancel(true);
		atomic_bool Success(false);

		for (decltype(retries) i = 0; i < retries; i++)
		{
			Utils::BroadcastPackage Buffer {1};
			Socket.send_to(boost::asio::buffer(Buffer), Endpoint);

			//Antwort empfangen
			udp::endpoint ServerEnd;
			Utils::BroadcastPackage RecvBuffer;

			NeedCancel = true;
			std::thread CancelThread([&io_service, &Socket, timeout, &NeedCancel]()
			{
				this_thread::sleep_for(timeout);

				if (NeedCancel)
				{
					Socket.cancel();
				}
			});

			Socket.async_receive_from(boost::asio::buffer(RecvBuffer), ServerEnd,
									  [&CancelThread, &NeedCancel, &ServerEnd, &Buffer, &Success](boost::system::error_code const & error, std::size_t received)
			{
				NeedCancel = false;
				if (CancelThread.joinable())
				{
					CancelThread.join();
				}

				if (received == Buffer.size() && !error)
				{
					//Etwas empfangen -> Server gefunden!
					Success = true;
				}
			});

			io_service.run();
			io_service.reset();
			if (CancelThread.joinable())
			{
				CancelThread.join();
			}

			if (Success)
			{
				return ServerEnd.address().to_string();
			}
		}

		return {};
	}
}
